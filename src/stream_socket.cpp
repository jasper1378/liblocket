#include "stream_socket.hpp"

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
#include <cstddef>
#include <exception>
#include <stdexcept>
#include <string>
#include <utility>

locket::stream_socket::stream_socket(socket_addr::sock_domain domain)
    : socket{domain} {
  stream_socket::init();
}

locket::stream_socket::stream_socket(socket::dummy_type_bind,
                                     const socket_addr *bound_addr)
    : socket{bound_addr->domain()} {
  stream_socket::init();
  bind(bound_addr);
}

locket::stream_socket::stream_socket(int sockfd) : socket{sockfd} {
  if (get_socket_option<int>(SO_TYPE) != SOCK_STREAM) {
    throw std::runtime_error{"socket is not a stream socket"};
  }
}

locket::stream_socket::stream_socket(stream_socket &&other) noexcept
    : socket{std::move(other)} {}

locket::stream_socket::~stream_socket() {}

std::string locket::stream_socket::recv(int flags /*= 0*/) const {
  char message_buffer[m_k_max_message_length];

  const ssize_t bytes_recvd{::recv(m_sockfd,
                                   static_cast<char *>(message_buffer),
                                   m_k_max_message_length, flags)};
  if (bytes_recvd == -1) {
    throw socket_error{"recv()", errno};
  }

  message_buffer[bytes_recvd] = '\0';

  return std::string{message_buffer};
}

void locket::stream_socket::send(const std::string &message,
                                 int flags /*= 0*/) const {
  const size_t message_len{message.size() + 1};
  const ssize_t bytes_sent{
      ::send(m_sockfd, message.c_str(), message_len, flags)};
  if (bytes_sent != static_cast<ssize_t>(message_len)) {
    throw socket_error{"send()", errno};
  }
}

locket::stream_socket &
locket::stream_socket::operator=(stream_socket &&other) noexcept {
  if (this == &other) {
    return *this;
  }

  socket::operator=(std::move(other));

  return *this;
}

void locket::stream_socket::init() {
  if (m_sockfd != -1) {
    throw std::runtime_error{"socket has already been initialized"};
  }

  m_sockfd = ::socket((static_cast<int>(get_domain())), SOCK_STREAM, 0);

  if (m_sockfd == -1) {
    throw socket_error{"socket()", errno};
  }
}
