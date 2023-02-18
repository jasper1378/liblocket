#include "socket.hpp"

#include "errno_error.hpp"
#include "inet_4_socket_addr.hpp"
#include "inet_6_socket_addr.hpp"
#include "inet_socket_addr.hpp"
#include "socket_addr.hpp"
#include "socket_error.hpp"
#include "unix_socket_addr.hpp"

#include <sys/socket.h>
#include <unistd.h>

#include <cerrno>
#include <exception>
#include <iostream>
#include <stdexcept>
#include <string>
#include <utility>

liblocket::socket::socket(socket_addr::sock_domain domain)
    : m_sockfd{-1}, m_domain{domain}, m_bound_addr{nullptr} {}

liblocket::socket::socket(int sockfd)
    : m_sockfd{sockfd}, m_domain{socket_addr::sock_domain::UNK}, m_bound_addr{
                                                                     nullptr} {
  m_domain =
      static_cast<socket_addr::sock_domain>(get_socket_option<int>(SO_DOMAIN));
}

liblocket::socket::socket(socket &&other) noexcept
    : m_sockfd{other.m_sockfd}, m_domain{other.m_domain},
      m_bound_addr{other.m_bound_addr} {
  other.m_sockfd = -1;
  other.m_bound_addr = nullptr;
}

liblocket::socket::~socket() {
  delete m_bound_addr;
  m_bound_addr = nullptr;

  close(false);
}

int liblocket::socket::get_sock_fd() { return m_sockfd; }

liblocket::socket_addr::sock_domain liblocket::socket::get_domain() const {
  return m_domain;
}

liblocket::socket_addr *liblocket::socket::get_bound_addr() const {
  if (m_bound_addr == nullptr) {
    return nullptr;
  }

  return m_bound_addr->create_clone();
}

void liblocket::socket::bind(const socket_addr *bind_addr) {
  if (m_bound_addr != nullptr) {
    throw std::runtime_error{"socket is already bound"};
  }

  if (bind_addr->domain() != m_domain) {
    throw std::runtime_error{"domains of socket and bind address do not match"};
  }

  if (::bind(m_sockfd, bind_addr->socket_addr_ptr(), bind_addr->size()) == -1) {
    throw socket_error{"bind()", errno};
  }

  delete m_bound_addr;
  m_bound_addr = nullptr;
  m_bound_addr = bind_addr->create_clone();
}

int liblocket::socket::create_clone() const {
  const int new_sockfd{dup(m_sockfd)};

  if (new_sockfd == -1) {
    throw socket_error{"dup()", errno};
  }

  return new_sockfd;
}

void liblocket::socket::shutdown(shutdown_how how) {
  if (::shutdown(m_sockfd, static_cast<int>(how)) == -1) {
    throw socket_error{"shutdown()", errno};
  }
}

liblocket::socket &liblocket::socket::operator=(socket &&other) noexcept {
  if (this == &other) {
    return *this;
  }

  delete m_bound_addr;
  m_bound_addr = nullptr;

  m_sockfd = other.m_sockfd;
  m_domain = other.m_domain;
  m_bound_addr = other.m_bound_addr;

  other.m_sockfd = -1;
  other.m_bound_addr = nullptr;

  return *this;
}

void liblocket::socket::close(bool throw_on_error /*= true*/) {
  if (m_sockfd != -1) {
    if (::close(m_sockfd) == -1) {
      if (throw_on_error == true) {
        throw socket_error{"close()", errno};
      } else {
        std::cerr << "Error in function that was asked not to throw: "
                  << socket_error{"close()", errno}.what();
      }
    }
    m_sockfd = -1;
  }
}
