#include "errno_error.hpp"

#include <cerrno>
#include <cstring>
#include <exception>
#include <stdexcept>
#include <string>
#include <type_traits>
#include <utility>

std::string liblocket::errno_error::strerror_threadsafe(int errno_num) {
  constexpr size_t buf_size{256};
  char buf[buf_size];

  static_assert((std::is_same_v<char *, decltype(strerror_r(
                                            errno_num, static_cast<char *>(buf),
                                            buf_size))> == true),
                "GNU-specific strerror_r() not available");

  return std::string{strerror_r(errno_num, static_cast<char *>(buf), buf_size)};
}

liblocket::errno_error::errno_error(const std::string &what_arg, int errno_num)
    : errno_error{what_arg, errno_num, nullptr} {}

liblocket::errno_error::errno_error(const char *what_arg, int errno_num)
    : errno_error{what_arg, errno_num, nullptr} {}

liblocket::errno_error::errno_error(const errno_error &other) noexcept
    : std::runtime_error{other}, m_errno_num{other.m_errno_num},
      m_errno_str{other.m_errno_str} {}

liblocket::errno_error::errno_error(errno_error &&other) noexcept
    : std::runtime_error{std::move(other)}, m_errno_num{other.m_errno_num},
      m_errno_str{std::move(other.m_errno_str)} {}

liblocket::errno_error::~errno_error() {}

liblocket::errno_error::errno_error(const std::string &what_arg, int errno_num,
                                 conversion_func errno_to_string)
    : std::runtime_error{what_arg}, m_errno_num{errno_num},
      m_errno_str{((errno_to_string != nullptr)
                       ? (errno_to_string(errno_num))
                       : (m_k_default_errno_to_string(errno_num)))} {}

liblocket::errno_error::errno_error(const char *what_arg, int errno_num,
                                 conversion_func errno_to_string)
    : std::runtime_error{what_arg}, m_errno_num{errno_num},
      m_errno_str{((errno_to_string != nullptr)
                       ? (errno_to_string(errno_num))
                       : (m_k_default_errno_to_string(errno_num)))} {}

const char *liblocket::errno_error::what() const noexcept {
  return std::runtime_error::what();
}

int liblocket::errno_error::get_errno() const noexcept { return m_errno_num; }

std::string liblocket::errno_error::get_errno_string() const noexcept {
  return m_errno_str;
}

liblocket::errno_error &
liblocket::errno_error::operator=(const errno_error &other) noexcept {
  m_errno_num = other.m_errno_num;
  m_errno_str = other.m_errno_str;
  std::runtime_error::operator=(other);

  return *this;
}

liblocket::errno_error &
liblocket::errno_error::operator=(errno_error &&other) noexcept {
  m_errno_num = other.m_errno_num;
  m_errno_str = std::move(other.m_errno_str);
  std::runtime_error::operator=(std::move(other));

  return *this;
}
