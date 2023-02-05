#ifndef LOCKET_CLIENT_STREAM_SOCKET_HPP
#define LOCKET_CLIENT_STREAM_SOCKET_HPP

#include "inet_4_socket_addr.hpp"
#include "inet_6_socket_addr.hpp"
#include "inet_socket_addr.hpp"
#include "socket.hpp"
#include "socket_addr.hpp"
#include "stream_socket.hpp"
#include "unix_socket_addr.hpp"

#include <sys/socket.h>

#include <string>

namespace locket {
class client_stream_socket : public stream_socket {
private:
  socket_addr *m_connected_addr;

public:
  explicit client_stream_socket(socket_addr::sock_domain domain);
  client_stream_socket(socket::dummy_type_bind, const socket_addr *bound_addr);
  client_stream_socket(socket::dummy_type_connect,
                       const socket_addr *connected_addr);
  client_stream_socket(const socket_addr *bound_addr,
                       const socket_addr *connected_addr);
  explicit client_stream_socket(int sockfd);
  client_stream_socket(client_stream_socket &&other) noexcept;

  client_stream_socket(const client_stream_socket &other) = delete;

  ~client_stream_socket() override;

public:
  void connect(const socket_addr *connect_addr);
  std::string recv(int flags = 0) const override;
  void send(const std::string &message, int flags = 0) const override;

public:
  client_stream_socket &operator=(client_stream_socket &&other) noexcept;

  client_stream_socket &operator=(const client_stream_socket &other) = delete;
};
} // namespace locket

#endif
