#include "socket_error.hpp"

#include "errno_error.hpp"

#include <cerrno>
#include <cstring>
#include <exception>
#include <stdexcept>
#include <string>
#include <utility>

liblocket::socket_error::socket_error(const std::string &what_arg, int errno_num)
    : socket_error{what_arg, errno_num, nullptr} {}

liblocket::socket_error::socket_error(const char *what_arg, int errno_num)
    : socket_error{what_arg, errno_num, nullptr} {}

liblocket::socket_error::socket_error(const socket_error &other) noexcept
    : errno_error{other} {}

liblocket::socket_error::socket_error(socket_error &&other) noexcept
    : errno_error{std::move(other)} {}

liblocket::socket_error::~socket_error() {}

liblocket::socket_error::socket_error(const std::string &what_arg, int errno_num,
                                   conversion_func errno_to_string)
    : errno_error{what_arg, errno_num,
                  ((errno_to_string != nullptr)
                       ? (errno_to_string)
                       : (m_k_default_errno_to_string))} {}

liblocket::socket_error::socket_error(const char *what_arg, int errno_num,
                                   conversion_func errno_to_string)
    : errno_error{what_arg, errno_num,
                  ((errno_to_string != nullptr)
                       ? (errno_to_string)
                       : (m_k_default_errno_to_string))} {}

const char *liblocket::socket_error::what() const noexcept {
  return errno_error::what();
}

int liblocket::socket_error::get_errno() const noexcept {
  return errno_error::get_errno();
}

std::string liblocket::socket_error::get_errno_string() const noexcept {
  return errno_error::get_errno_string();
}

liblocket::socket_error &
liblocket::socket_error::operator=(const socket_error &other) noexcept {
  errno_error::operator=(other);

  return *this;
}

liblocket::socket_error &
liblocket::socket_error::operator=(socket_error &&other) noexcept {
  errno_error::operator=(std::move(other));

  return *this;
}
