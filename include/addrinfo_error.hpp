#ifndef LOCKET_ADDRINFO_ERROR_HPP
#define LOCKET_ADDRINFO_ERROR_HPP

#include "errno_error.hpp"
#include "socket_error.hpp"

#include <netdb.h>

#include <cerrno>
#include <cstring>
#include <exception>
#include <stdexcept>
#include <string>

namespace locket {
class addrinfo_error : public socket_error {
public:
  enum class func { getaddrinfo, getnameinfo };

public:
  addrinfo_error(func function, int errno_num);
  addrinfo_error(const addrinfo_error &other) noexcept;
  addrinfo_error(addrinfo_error &&other) noexcept;

  virtual ~addrinfo_error() override;

public:
  virtual const char *what() const noexcept override;
  virtual int get_errno() const noexcept override;
  virtual std::string get_errno_string() const noexcept override;

public:
  addrinfo_error &operator=(const addrinfo_error &other) noexcept;
  addrinfo_error &operator=(addrinfo_error &&other) noexcept;

private:
  virtual std::string errno_to_string(int errno_num) const override;
  static std::string func_to_string(func function);
};
} // namespace locket

#endif
