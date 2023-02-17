#include "unix_socket_addr.hpp"

#include "socket_addr.hpp"

#include <sys/socket.h>
#include <sys/un.h>

#include <cstring>
#include <exception>
#include <stdexcept>
#include <string>

locket::unix_socket_addr::unix_socket_addr() : m_addr{}, m_is_abstract{false} {
  unix_socket_addr::clear();
  unix_socket_addr::set_domain();
}

locket::unix_socket_addr::unix_socket_addr(const std::string &path_or_name,
                                           bool is_abstract /*= false*/)
    : m_addr{}, m_is_abstract{is_abstract} {
  unix_socket_addr::clear();
  unix_socket_addr::set_domain();

  if (is_abstract) {
    std::strncpy((&(m_addr.sun_path[1])), path_or_name.c_str(),
                 (sizeof(m_addr.sun_path) - 2));
  } else {
    std::strncpy(m_addr.sun_path, path_or_name.c_str(),
                 (sizeof(m_addr.sun_path) - 1));
  }
}

locket::unix_socket_addr::unix_socket_addr(const sockaddr *other)
    : m_addr{}, m_is_abstract{} {
  if (other->sa_family != AF_UNIX) {
    throw std::invalid_argument{"sockaddr is not a Unix address"};
  }

  std::memcpy(&m_addr, other, m_k_size);
  m_is_abstract = check_if_abstract();
}

locket::unix_socket_addr::unix_socket_addr(const socket_addr *other)
    : m_addr{}, m_is_abstract{} {
  if (other->domain() != sock_domain::UNIX) {
    throw std::invalid_argument{"socket_addr is not a Unix address"};
  }

  std::memcpy(&m_addr, other->socket_addr_ptr(), m_k_size);
  m_is_abstract = check_if_abstract();
}

locket::unix_socket_addr::unix_socket_addr(const sockaddr_un &other)
    : m_addr{other} {
  m_is_abstract = check_if_abstract();
}

locket::unix_socket_addr::unix_socket_addr(const unix_socket_addr &other)
    : m_addr{other.m_addr}, m_is_abstract{other.m_is_abstract} {}

locket::unix_socket_addr::unix_socket_addr(unix_socket_addr &&other) noexcept
    : m_addr{other.m_addr}, m_is_abstract{other.m_is_abstract} {}

locket::unix_socket_addr::~unix_socket_addr() {}

locket::unix_socket_addr *locket::unix_socket_addr::create_new() const {
  return new unix_socket_addr{};
}

locket::unix_socket_addr *locket::unix_socket_addr::create_clone() const {
  return new unix_socket_addr{*this};
}

locket::socket_addr::sock_domain locket::unix_socket_addr::domain() const {
  return sock_domain::UNIX;
}

socklen_t locket::unix_socket_addr::size() const { return m_k_size; }

bool locket::unix_socket_addr::is_set() const {

  if (m_is_abstract) {
    return (m_addr.sun_path[1] != '\0');
  } else {
    return (m_addr.sun_path[0] != '\0');
  }
}

std::string locket::unix_socket_addr::to_string() const {
  return std::string{m_addr.sun_path};
}

sockaddr *locket::unix_socket_addr::socket_addr_ptr() {
  return reinterpret_cast<sockaddr *>(&m_addr);
}

const sockaddr *locket::unix_socket_addr::socket_addr_ptr() const {
  return reinterpret_cast<const sockaddr *>(&m_addr);
}

sockaddr_un *locket::unix_socket_addr::socket_addr_un_ptr() { return &m_addr; }

const sockaddr_un *locket::unix_socket_addr::socket_addr_un_ptr() const {
  return &m_addr;
}

bool locket::unix_socket_addr::is_abstract() const { return m_is_abstract; }

locket::unix_socket_addr &
locket::unix_socket_addr::operator=(const unix_socket_addr &other) {
  m_addr = other.m_addr;
  m_is_abstract = other.m_is_abstract;

  return *this;
}

locket::unix_socket_addr &
locket::unix_socket_addr::operator=(unix_socket_addr &&other) noexcept {
  m_addr = other.m_addr;
  m_is_abstract = other.m_is_abstract;

  return *this;
}

void locket::unix_socket_addr::clear() { std::memset(&m_addr, 0, m_k_size); }

void locket::unix_socket_addr::set_domain() { m_addr.sun_family = AF_UNIX; }

bool locket::unix_socket_addr::check_if_abstract() const {
  if (m_addr.sun_path[0] != '\0') {
    return false;
  } else if ((m_addr.sun_path[0] == '\0') && (m_addr.sun_path[1] != '\0')) {
    return true;
  } else if ((m_addr.sun_path[0] == '\0') && (m_addr.sun_path[1] == '\0')) {
    return false;
  } else {
    return false;
  }
}
