#include "inet_4_socket_addr.hpp"

#include "addrinfo_error.hpp"
#include "errno_error.hpp"
#include "inet_socket_addr.hpp"
#include "socket_addr.hpp"

#include <arpa/inet.h>
#include <netdb.h>
#include <netinet/in.h>
#include <sys/socket.h>

#include <cstring>
#include <exception>
#include <stdexcept>
#include <string>

locket::inet4_socket_addr::inet4_socket_addr() : m_addr{} {
  inet4_socket_addr::clear();
  inet4_socket_addr::set_domain();
}

locket::inet4_socket_addr::inet4_socket_addr(in_port_t port) : m_addr{} {
  inet4_socket_addr::clear();
  inet4_socket_addr::set_domain();
  init(in_addr{INADDR_ANY}, port);
}

locket::inet4_socket_addr::inet4_socket_addr(in_addr addr, in_port_t port)
    : m_addr{} {
  inet4_socket_addr::clear();
  inet4_socket_addr::set_domain();
  init(addr, port);
}

locket::inet4_socket_addr::inet4_socket_addr(const std::string &saddr,
                                             in_port_t port)
    : m_addr{} {
  inet4_socket_addr::clear();
  inet4_socket_addr::set_domain();
  init(saddr, port);
}

locket::inet4_socket_addr::inet4_socket_addr(const sockaddr *other) : m_addr{} {
  if (other->sa_family != AF_INET) {
    throw std::invalid_argument{"sockaddr is not an inet address"};
  }

  std::memcpy(&m_addr, other, m_k_size);
}

locket::inet4_socket_addr::inet4_socket_addr(const socket_addr *other)
    : m_addr{} {
  if (other->domain() != sock_domain::INET4) {
    throw std::invalid_argument{"socket_addr is not an inet address"};
  }

  std::memcpy(&m_addr, other->socket_addr_ptr(), m_k_size);
}

locket::inet4_socket_addr::inet4_socket_addr(const sockaddr_in &other)
    : m_addr{other} {}

locket::inet4_socket_addr::inet4_socket_addr(const inet4_socket_addr &other)
    : m_addr{other.m_addr} {}

locket::inet4_socket_addr::inet4_socket_addr(inet4_socket_addr &&other) noexcept
    : m_addr{other.m_addr} {}

locket::inet4_socket_addr::~inet4_socket_addr() {}

locket::inet4_socket_addr *locket::inet4_socket_addr::create_new() const {
  return new inet4_socket_addr{};
}

locket::inet4_socket_addr *locket::inet4_socket_addr::create_clone() const {
  return new inet4_socket_addr{*this};
}

locket::socket_addr::sock_domain locket::inet4_socket_addr::domain() const {
  return sock_domain::INET4;
}

socklen_t locket::inet4_socket_addr::size() const { return m_k_size; }

bool locket::inet4_socket_addr::is_set() const {
  sockaddr_in empty_addr;
  std::memset(&empty_addr, 0, sizeof(sockaddr_in));
  empty_addr.sin_family = AF_INET;

  return ((std::memcmp(&m_addr, &empty_addr, m_k_size)) == 0);
}

std::string locket::inet4_socket_addr::to_string() const {
  char address_buf[INET_ADDRSTRLEN];
  std::string addr_str{};
  addr_str.append(inet_ntop(AF_INET, &m_addr.sin_addr,
                            static_cast<char *>(address_buf), INET_ADDRSTRLEN));
  addr_str.append(":");
  addr_str.append(std::to_string(ntohs(m_addr.sin_port)));
  return addr_str;
}

sockaddr *locket::inet4_socket_addr::socket_addr_ptr() {
  return reinterpret_cast<sockaddr *>(&m_addr);
}

const sockaddr *locket::inet4_socket_addr::socket_addr_ptr() const {
  return reinterpret_cast<const sockaddr *>(&m_addr);
}

sockaddr_in *locket::inet4_socket_addr::socket_addr_in4_ptr() {
  return &m_addr;
}

const sockaddr_in *locket::inet4_socket_addr::socket_addr_in4_ptr() const {
  return &m_addr;
}

in_port_t locket::inet4_socket_addr::port() const { return m_addr.sin_port; }

in_addr locket::inet4_socket_addr::address() const { return m_addr.sin_addr; }

locket::inet4_socket_addr &
locket::inet4_socket_addr::operator=(const inet4_socket_addr &other) {
  m_addr = other.m_addr;

  return *this;
}

locket::inet4_socket_addr &
locket::inet4_socket_addr::operator=(inet4_socket_addr &&other) noexcept {
  m_addr = other.m_addr;

  return *this;
}

void locket::inet4_socket_addr::clear() { std::memset(&m_addr, 0, m_k_size); }

void locket::inet4_socket_addr::set_domain() { m_addr.sin_family = AF_INET; }

void locket::inet4_socket_addr::init(in_addr addr, in_port_t port) {
  m_addr.sin_port = htons(port);
  m_addr.sin_addr.s_addr = htonl(addr.s_addr);
}

void locket::inet4_socket_addr::init(const std::string &saddr, in_port_t port) {
  init(resolve_name(saddr, byte_order::HOST), port);
}

in_addr locket::inet4_socket_addr::resolve_name(
    const std::string &saddr, byte_order return_order /*= byte_order::net*/) {
  addrinfo hints;
  std::memset(&hints, 0, sizeof(addrinfo));
  hints.ai_family = AF_INET;

  addrinfo *result{nullptr};
  in_addr resolved_address;

  try {
    const int getaddrinfo_result{
        getaddrinfo(saddr.c_str(), nullptr, &hints, &result)};
    if (getaddrinfo_result != 0) {
      throw addrinfo_error{addrinfo_error::func::getaddrinfo,
                           getaddrinfo_result};
    }

    resolved_address =
        (reinterpret_cast<sockaddr_in *>(result->ai_addr))->sin_addr;

    if (return_order == byte_order::HOST) {
      resolved_address.s_addr = ntohl(resolved_address.s_addr);
    }
  } catch (...) {
    if (result != nullptr) {
      freeaddrinfo(result);
    }

    throw;
  }

  if (result != nullptr) {
    freeaddrinfo(result);
  }

  return resolved_address;
}
