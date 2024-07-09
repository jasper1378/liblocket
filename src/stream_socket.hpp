#ifndef LIBLOCKET_STREAM_SOCKET_HPP
#define LIBLOCKET_STREAM_SOCKET_HPP

#include "inet_4_socket_addr.hpp"
#include "inet_6_socket_addr.hpp"
#include "inet_socket_addr.hpp"
#include "socket.hpp"
#include "socket_addr.hpp"
#include "unix_socket_addr.hpp"

#include <sys/socket.h>

#include <string>

namespace liblocket {
class stream_socket : public socket {
protected:
  explicit stream_socket(socket_addr::sock_domain domain);
  stream_socket(socket::dummy_type_bind, const socket_addr *bound_addr);
  explicit stream_socket(int sockfd);
  stream_socket(stream_socket &&other) noexcept;

  stream_socket(const stream_socket &other) = delete;

public:
  virtual ~stream_socket() override;

public:
  stream_socket &operator=(stream_socket &&other) noexcept;

  stream_socket &operator=(const stream_socket &other) = delete;

protected:
  void init() override;
  virtual std::string recv(int flags = 0) const;
  virtual void send(const std::string &message, int flags = 0) const;
};
} // namespace liblocket

#endif
