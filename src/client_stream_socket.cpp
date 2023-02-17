#include "client_stream_socket.hpp"

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
#include <stdexcept>
#include <string>
#include <utility>

locket::client_stream_socket::client_stream_socket(
    socket_addr::sock_domain domain)
    : stream_socket{domain}, m_connected_addr{nullptr} {}

locket::client_stream_socket::client_stream_socket(
    socket::dummy_type_bind, const socket_addr *bound_addr)
    : stream_socket{socket::dummy_type_bind{}, bound_addr}, m_connected_addr{
                                                                nullptr} {}

locket::client_stream_socket::client_stream_socket(
    socket::dummy_type_connect, const socket_addr *connected_addr)
    : stream_socket{connected_addr->domain()}, m_connected_addr{nullptr} {
  connect(connected_addr);
}

locket::client_stream_socket::client_stream_socket(
    const socket_addr *bound_addr, const socket_addr *connected_addr)
    : stream_socket{socket::dummy_type_bind{}, bound_addr}, m_connected_addr{
                                                                nullptr} {
  connect(connected_addr);
}

locket::client_stream_socket::client_stream_socket(int sockfd)
    : stream_socket{sockfd}, m_connected_addr{nullptr} {}

locket::client_stream_socket::client_stream_socket(
    client_stream_socket &&other) noexcept
    : stream_socket{std::move(other)}, m_connected_addr{
                                           other.m_connected_addr} {
  other.m_connected_addr = nullptr;
}

locket::client_stream_socket::~client_stream_socket() {
  delete m_connected_addr;
  m_connected_addr = nullptr;
}

void locket::client_stream_socket::connect(const socket_addr *connect_addr) {
  if (get_domain() != connect_addr->domain()) {
    throw std::runtime_error{
        "domains of socket and connect address do not match"};
  }

  if (m_connected_addr != nullptr) {
    throw std::runtime_error{"socket is already connected"};
  }

  if (::connect(m_sockfd, connect_addr->socket_addr_ptr(),
                connect_addr->size()) == -1) {
    throw socket_error{"connect()", errno};
  }

  delete m_connected_addr;

  m_connected_addr = connect_addr->create_clone();
}

std::string locket::client_stream_socket::recv(int flags /*= 0*/) const {
  if (m_connected_addr == nullptr) {
    throw std::runtime_error{"socket is not connected"};
  }

  return stream_socket::recv(flags);
}

void locket::client_stream_socket::send(const std::string &message,
                                        int flags /*= 0*/) const {
  if (m_connected_addr == nullptr) {
    throw std::runtime_error{"socket is not connected"};
  }

  stream_socket::send(message, flags);
}

locket::client_stream_socket &
locket::client_stream_socket::operator=(client_stream_socket &&other) noexcept {
  if (this == &other) {
    return *this;
  }

  delete m_connected_addr;
  m_connected_addr = nullptr;

  m_connected_addr = other.m_connected_addr;

  other.m_connected_addr = nullptr;

  stream_socket::operator=(std::move(other));

  return *this;
}
