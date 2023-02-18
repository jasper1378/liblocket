#ifndef LIBLOCKET_ERRNO_ERROR_HPP
#define LIBLOCKET_ERRNO_ERROR_HPP

#include <cerrno>
#include <cstring>

#include <exception>
#include <stdexcept>
#include <string>

namespace liblocket {
class errno_error : public std::runtime_error {
protected:
  using conversion_func = std::string (*)(int);

public:
  static std::string strerror_threadsafe(int errno_num);

private:
  int m_errno_num;
  std::string m_errno_str;

private:
  static constexpr conversion_func m_k_default_errno_to_string{
      &strerror_threadsafe};

public:
  errno_error(const std::string &what_arg, int errno_num);
  errno_error(const char *what_arg, int errno_num);
  errno_error(const errno_error &other) noexcept;
  errno_error(errno_error &&other) noexcept;

  virtual ~errno_error();

protected:
  errno_error(const std::string &what_arg, int errno_num,
              conversion_func errno_to_string);
  errno_error(const char *what_arg, int errno_num,
              conversion_func errno_to_string);

public:
  virtual const char *what() const noexcept;
  virtual int get_errno() const noexcept;
  virtual std::string get_errno_string() const noexcept;

public:
  errno_error &operator=(const errno_error &other) noexcept;
  errno_error &operator=(errno_error &&other) noexcept;
};
} // namespace liblocket

#endif
