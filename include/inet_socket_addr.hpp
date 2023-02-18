#ifndef LIBLOCKET_INET_SOCKET_ADDR_HPP
#define LIBLOCKET_INET_SOCKET_ADDR_HPP

#include "socket_addr.hpp"

#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>

#include <string>

namespace liblocket {
class inet_socket_addr : public socket_addr {
public:
  enum class ip_version {
    IPv4,
    IPv6,
  };

  enum class byte_order {
    HOST,
    NET,
  };

public:
  virtual ~inet_socket_addr() override;

public:
  virtual inet_socket_addr *create_new() const override = 0;
  virtual inet_socket_addr *create_clone() const override = 0;
  virtual sock_domain domain() const override = 0;
  virtual socklen_t size() const override = 0;
  virtual bool is_set() const override = 0;
  virtual std::string to_string() const override = 0;
  virtual sockaddr *socket_addr_ptr() override = 0;
  virtual const sockaddr *socket_addr_ptr() const override = 0;
  virtual in_port_t port() const = 0;

protected:
  virtual void clear() override = 0;
  virtual void set_domain() override = 0;
};
} // namespace liblocket

#endif
