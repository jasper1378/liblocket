#include "addrinfo_error.hpp"

#include "errno_error.hpp"
#include "socket_error.hpp"

#include <netdb.h>

#include <cerrno>
#include <cstring>
#include <exception>
#include <stdexcept>
#include <string>
#include <utility>

std::string locket::addrinfo_error::gai_strerror_wrapper(int errno_num) {
  return std::string{gai_strerror(errno_num)};
}

locket::addrinfo_error::addrinfo_error(func function, int errno_num)
    : addrinfo_error{function, errno_num, nullptr} {}

locket::addrinfo_error::addrinfo_error(const addrinfo_error &other) noexcept
    : socket_error{other} {}

locket::addrinfo_error::addrinfo_error(addrinfo_error &&other) noexcept
    : socket_error{std::move(other)} {}

locket::addrinfo_error::~addrinfo_error() {}

locket::addrinfo_error::addrinfo_error(func function, int errno_num,
                                       conversion_func errno_to_string)
    : socket_error{func_to_string(function), errno_num,
                   ((errno_to_string != nullptr)
                        ? (errno_to_string)
                        : (m_k_default_errno_to_string))} {}

const char *locket::addrinfo_error::what() const noexcept {
  return socket_error::what();
}

int locket::addrinfo_error::get_errno() const noexcept {
  return socket_error::get_errno();
}

std::string locket::addrinfo_error::get_errno_string() const noexcept {
  return socket_error::get_errno_string();
}

locket::addrinfo_error &
locket::addrinfo_error::operator=(const addrinfo_error &other) noexcept {
  if (this == &other) {
    return *this;
  }

  socket_error::operator=(other);

  return *this;
}

locket::addrinfo_error &
locket::addrinfo_error::operator=(addrinfo_error &&other) noexcept {
  if (this == &other) {
    return *this;
  }

  socket_error::operator=(std::move(other));

  return *this;
}

std::string locket::addrinfo_error::func_to_string(func function) {
  static constexpr const char *func_name_lut[]{"getaddrinfo()",
                                               "getnameinfo()"};

  return func_name_lut[static_cast<int>(function)];
}
