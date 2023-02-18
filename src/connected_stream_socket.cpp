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
#include <stdexcept>
#include <string>
#include <utility>

liblocket::connected_stream_socket::connected_stream_socket(
    int sockfd, const socket_addr *connected_addr)
    : stream_socket{sockfd}, m_connected_addr{nullptr} {
  if (connected_addr == nullptr) {
    throw std::runtime_error{"connected address is null"};
  }

  m_connected_addr = connected_addr->create_clone();
}

liblocket::connected_stream_socket::connected_stream_socket(
    connected_stream_socket &&other) noexcept
    : stream_socket{std::move(other)}, m_connected_addr{
                                           other.m_connected_addr} {
  other.m_connected_addr = nullptr;
}

liblocket::connected_stream_socket::~connected_stream_socket() {
  delete m_connected_addr;
  m_connected_addr = nullptr;
}

std::string liblocket::connected_stream_socket::recv(int flags /*= 0*/) const {
  return stream_socket::recv(flags);
}

void liblocket::connected_stream_socket::send(const std::string &message,
                                           int flags /*= 0*/) const {
  stream_socket::send(message, flags);
}

liblocket::connected_stream_socket &liblocket::connected_stream_socket::operator=(
    connected_stream_socket &&other) noexcept {
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

liblocket::socket_addr *liblocket::connected_stream_socket::get_bound_addr() const {
  throw std::runtime_error{
      "connected_stream_sockets can not be bound. Yes, I'm well aware that "
      "this violates the principle of having a common interface for all "
      "derived classes."};
  return nullptr;
}

void liblocket::connected_stream_socket::bind(__attribute__((unused))
                                           const socket_addr *bound_addr) {
  throw std::runtime_error{
      "connected_stream_sockets can not be bound. Yes, I'm well aware that "
      "this violates the principle of having a common interface for all "
      "derived classes."};
}
