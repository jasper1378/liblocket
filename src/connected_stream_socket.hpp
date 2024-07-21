#ifndef LIBLOCKET_CONNECTED_STREAM_SOCKET_HPP
#define LIBLOCKET_CONNECTED_STREAM_SOCKET_HPP

#include "inet_4_socket_addr.hpp"
#include "inet_6_socket_addr.hpp"
#include "inet_socket_addr.hpp"
#include "socket.hpp"
#include "socket_addr.hpp"
#include "stream_socket.hpp"
#include "unix_socket_addr.hpp"

#include <sys/socket.h>

#include <string>

namespace liblocket {
class connected_stream_socket : public stream_socket {
private:
  socket_addr *m_connected_addr;

public:
  connected_stream_socket(int sockfd, const socket_addr *connected_addr);
  connected_stream_socket(connected_stream_socket &&other) noexcept;

  connected_stream_socket(const connected_stream_socket &other) = delete;

  ~connected_stream_socket() override;

public:
  std::string recv(int flags = 0) const override;
  void send(const std::string &message, int flags = 0) const override;

public:
  connected_stream_socket &operator=(connected_stream_socket &&other) noexcept;

  connected_stream_socket &
  operator=(const connected_stream_socket &other) = delete;
};
} // namespace liblocket

#endif
