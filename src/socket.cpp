#include "socket.hpp"

#include "errno_error.hpp"
#include "inet_4_socket_addr.hpp"
#include "inet_6_socket_addr.hpp"
#include "inet_socket_addr.hpp"
#include "socket.hpp"
#include "socket_addr.hpp"
#include "socket_error.hpp"
#include "unix_socket_addr.hpp"

#include <sys/socket.h>
#include <unistd.h>

#include <cerrno>
#include <exception>
#include <stdexcept>
#include <string>
#include <utility>

locket::socket::socket(socket_addr::sock_domain domain)
    : m_sockfd{-1}, m_domain{domain}, m_bound_addr{nullptr} {}

locket::socket::socket(int sockfd)
    : m_sockfd{sockfd}, m_domain{socket_addr::sock_domain::UNK}, m_bound_addr{
                                                                     nullptr} {
  m_domain =
      static_cast<socket_addr::sock_domain>(get_socket_option<int>(SO_DOMAIN));
}

locket::socket::socket(socket &&other)
    : m_sockfd{other.m_sockfd}, m_domain{std::move(other.m_domain)},
      m_bound_addr{other.m_bound_addr} {
  other.m_sockfd = -1;
  other.m_bound_addr = nullptr;
}

locket::socket::~socket() {
  if (m_bound_addr != nullptr) {
    delete m_bound_addr;
    m_bound_addr = nullptr;
  }

  close();
}

int locket::socket::get_sock_fd() { return m_sockfd; }

locket::socket_addr::sock_domain locket::socket::get_domain() const {
  return m_domain;
}

locket::socket_addr *locket::socket::get_bound_addr() const {
  if (m_bound_addr == nullptr) {
    return nullptr;
  }

  return m_bound_addr->create_clone();
}

void locket::socket::bind(const socket_addr *bind_addr) {
  if (m_bound_addr != nullptr) {
    throw std::runtime_error{"socket is already bound"};
  }

  if (bind_addr->domain() != m_domain) {
    throw std::runtime_error{"domains of socket and bind address do not match"};
  }

  if (::bind(m_sockfd, bind_addr->socket_addr_ptr(), bind_addr->size()) == -1) {
    throw socket_error{"bind()", errno};
  }

  if (m_bound_addr != nullptr) {
    delete m_bound_addr;
    m_bound_addr = nullptr;
  }
  m_bound_addr = bind_addr->create_clone();
}

int locket::socket::create_clone() {
  int new_sockfd{dup(m_sockfd)};

  if (new_sockfd == -1) {
    throw socket_error{"dup()", errno};
  }

  return new_sockfd;
}

void locket::socket::shutdown(shutdown_how how) {
  if (::shutdown(m_sockfd, static_cast<int>(how)) == -1) {
    throw socket_error{"shutdown()", errno};
  }
}

locket::socket &locket::socket::operator=(socket &&other) {
  if (this == &other) {
    return *this;
  }

  if (m_bound_addr != nullptr) {
    delete m_bound_addr;
    m_bound_addr = nullptr;
  }

  m_sockfd = other.m_sockfd;
  m_domain = std::move(other.m_domain);
  m_bound_addr = other.m_bound_addr;

  other.m_sockfd = -1;
  other.m_bound_addr = nullptr;

  return *this;
}

void locket::socket::close() {
  if (m_sockfd != -1) {
    if (::close(m_sockfd) == -1) {
      throw socket_error{"close()", errno};
    }
    m_sockfd = -1;
  }
}
