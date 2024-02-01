#include "server_stream_socket.hpp"

#include "connected_stream_socket.hpp"
#include "errno_error.hpp"
#include "inet_4_socket_addr.hpp"
#include "inet_6_socket_addr.hpp"
#include "inet_socket_addr.hpp"
#include "socket.hpp"
#include "socket_addr.hpp"
#include "socket_error.hpp"
#include "stream_socket.hpp"
#include "unix_socket_addr.hpp"

#include <sys/socket.h>
#include <unistd.h>

#include <cerrno>
#include <exception>
#include <memory>
#include <stdexcept>
#include <string>
#include <utility>

liblocket::server_stream_socket::server_stream_socket(
    socket_addr::sock_domain domain)
    : stream_socket{domain}, m_is_listening{false} {}

liblocket::server_stream_socket::server_stream_socket(
    socket::dummy_type_bind, const socket_addr *bound_addr)
    : stream_socket{socket::dummy_type_bind{}, bound_addr},
      m_is_listening{false} {}

liblocket::server_stream_socket::server_stream_socket(
    socket_addr::sock_domain domain, socket::dummy_type_listen,
    int backlog /*= m_k_backlog*/)
    : stream_socket{domain}, m_is_listening{false} {
  listen(backlog);
}

liblocket::server_stream_socket::server_stream_socket(
    const socket_addr *bound_addr, int backlog /*= m_k_backlog*/)
    : stream_socket{socket::dummy_type_bind{}, bound_addr},
      m_is_listening{false} {
  listen(backlog);
}

liblocket::server_stream_socket::server_stream_socket(int sockfd)
    : stream_socket{sockfd}, m_is_listening{false} {}

liblocket::server_stream_socket::server_stream_socket(
    server_stream_socket &&other) noexcept
    : stream_socket{std::move(other)}, m_is_listening{other.m_is_listening} {}

liblocket::server_stream_socket::~server_stream_socket() {}

void liblocket::server_stream_socket::listen(int backlog /*= m_k_backlog*/) {
  if (m_is_listening == true) {
    throw std::runtime_error{"socket is already listening"};
  }

  if (::listen(m_sockfd, backlog) == -1) {
    throw socket_error{"listen()", errno};
  }
}

liblocket::connected_stream_socket
liblocket::server_stream_socket::accept() const {
  std::unique_ptr<socket_addr> connected_addr{nullptr};

  if (get_bound_addr() == nullptr) {
    throw std::runtime_error{"socket is not bound"};
  }

  if (m_is_listening == false) {
    throw std::runtime_error{"socket is not listening"};
  }

  switch (get_domain()) {
  case socket_addr::sock_domain::unix:
    connected_addr = std::make_unique<unix_socket_addr>();
    break;
  case socket_addr::sock_domain::inet4:
    connected_addr = std::make_unique<inet4_socket_addr>();
    break;
  case socket_addr::sock_domain::inet6:
    connected_addr = std::make_unique<inet6_socket_addr>();
    break;
  default:
    throw std::runtime_error{"this should be impossible..."};
    break;
  }

  socklen_t connected_addr_len{connected_addr->size()};

  const int new_sockfd{::accept(m_sockfd, connected_addr->socket_addr_ptr(),
                                &connected_addr_len)};

  if (new_sockfd == -1) {
    throw socket_error{"accept()", errno};
  }

  connected_stream_socket connected_stream_socket{new_sockfd,
                                                  connected_addr.get()};

  return connected_stream_socket;
}

liblocket::server_stream_socket &liblocket::server_stream_socket::operator=(
    server_stream_socket &&other) noexcept {
  m_is_listening = other.m_is_listening;

  stream_socket::operator=(std::move(other));

  return *this;
}
