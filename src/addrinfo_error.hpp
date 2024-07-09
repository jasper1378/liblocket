#ifndef LIBLOCKET_ADDRINFO_ERROR_HPP
#define LIBLOCKET_ADDRINFO_ERROR_HPP

#include "errno_error.hpp"
#include "socket_error.hpp"

#include <netdb.h>

#include <cerrno>
#include <cstring>
#include <exception>
#include <stdexcept>
#include <string>

namespace liblocket {
class addrinfo_error : public socket_error {
public:
  enum class func { getaddrinfo = 0, getnameinfo = 1 };

public:
  static std::string gai_strerror_wrapper(int errno_num);

private:
  static constexpr conversion_func m_k_default_errno_to_string{
      &gai_strerror_wrapper};

public:
  addrinfo_error(func function, int errno_num);
  addrinfo_error(const addrinfo_error &other) noexcept;
  addrinfo_error(addrinfo_error &&other) noexcept;

  virtual ~addrinfo_error() override;

protected:
  addrinfo_error(func function, int errno_num, conversion_func errno_to_string);

public:
  virtual const char *what() const noexcept override;
  virtual int get_errno() const noexcept override;
  virtual std::string get_errno_string() const noexcept override;

public:
  addrinfo_error &operator=(const addrinfo_error &other) noexcept;
  addrinfo_error &operator=(addrinfo_error &&other) noexcept;

private:
  static std::string func_to_string(func function);
};
} // namespace liblocket

#endif
