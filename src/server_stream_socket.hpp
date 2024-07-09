#ifndef LIBLOCKET_SERVER_STREAM_SOCKET_HPP
#define LIBLOCKET_SERVER_STREAM_SOCKET_HPP

#include "connected_stream_socket.hpp"
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
class server_stream_socket : public stream_socket {
private:
  bool m_is_listening;

protected:
  static constexpr int m_k_backlog{4096};

public:
  explicit server_stream_socket(socket_addr::sock_domain domain);
  server_stream_socket(socket::dummy_type_bind, const socket_addr *bound_addr);
  server_stream_socket(socket_addr::sock_domain domain,
                       socket::dummy_type_listen, int backlog = m_k_backlog);
  explicit server_stream_socket(const socket_addr *bound_addr,
                                int backlog = m_k_backlog);
  explicit server_stream_socket(int sockfd);
  server_stream_socket(server_stream_socket &&other) noexcept;

  server_stream_socket(const server_stream_socket &other) = delete;

  ~server_stream_socket() override;

public:
  void listen(int backlog = m_k_backlog);
  connected_stream_socket accept() const;

public:
  server_stream_socket &operator=(server_stream_socket &&other) noexcept;

  server_stream_socket &operator=(const server_stream_socket &other) = delete;
};
} // namespace liblocket

#endif
