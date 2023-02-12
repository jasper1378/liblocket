#ifndef LOCKET_SOCKET_ERROR_HPP
#define LOCKET_SOCKET_ERROR_HPP

#include "errno_error.hpp"

#include <cerrno>
#include <cstring>

#include <exception>
#include <stdexcept>
#include <string>

namespace locket {
class socket_error : public errno_error {
private:
  static constexpr conversion_func m_k_default_errno_to_string{
      &errno_error::strerror_threadsafe};

public:
  socket_error(const std::string &what_arg, int errno_num);
  socket_error(const char *what_arg, int errno_num);
  socket_error(const socket_error &other) noexcept;
  socket_error(socket_error &&other) noexcept;

  virtual ~socket_error();

protected:
  socket_error(const std::string &what_arg, int errno_num,
               conversion_func errno_to_string);
  socket_error(const char *what_arg, int errno_num,
               conversion_func errno_to_string);

public:
  virtual const char *what() const noexcept override;
  virtual int get_errno() const noexcept override;
  virtual std::string get_errno_string() const noexcept override;

public:
  socket_error &operator=(const socket_error &other) noexcept;
  socket_error &operator=(socket_error &&other) noexcept;
};
} // namespace locket

#endif
