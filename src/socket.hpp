#ifndef LIBLOCKET_SOCKET_HPP
#define LIBLOCKET_SOCKET_HPP

#include "inet_4_socket_addr.hpp"
#include "inet_6_socket_addr.hpp"
#include "inet_socket_addr.hpp"
#include "socket_addr.hpp"
#include "socket_error.hpp"
#include "unix_socket_addr.hpp"

#include <sys/socket.h>

#include <cerrno>
#include <cstddef>
#include <string>

namespace liblocket {
class socket {
public:
  struct dummy_type_bind {};
  struct dummy_type_connect {};
  struct dummy_type_listen {};

  enum class shutdown_how {
    read = SHUT_RD,
    write = SHUT_WR,
    readwrite = SHUT_RDWR,
  };

protected:
  int m_sockfd;

private:
  socket_addr::sock_domain m_domain;
  socket_addr *m_bound_addr;

protected:
  static constexpr size_t m_k_max_message_length{1024};

public:
  socket(socket_addr::sock_domain domain);
  socket(int sockfd);
  socket(socket &&other) noexcept;

  socket(const socket &other) = delete;

  virtual ~socket();

public:
  int get_sock_fd();
  socket_addr::sock_domain get_domain() const;
  virtual socket_addr *get_bound_addr() const;
  virtual void bind(const socket_addr *bind_addr);
  int create_clone() const;
  void shutdown(shutdown_how how);

  template <typename T>
  void set_socket_option(int option_name, const T &option_value) {
    if (setsockopt(m_sockfd, SOL_SOCKET, option_name, &option_value,
                   sizeof(option_value)) == -1) {
      throw socket_error{"setsockopt()", errno};
    }
  }

  template <typename T> T get_socket_option(int option_name) const {
    T option_value{};
    socklen_t option_length{sizeof(T)};

    if (getsockopt(m_sockfd, SOL_SOCKET, option_name, &option_value,
                   &option_length) == -1) {
      throw socket_error{"getsockopt()", errno};
    }

    return option_value;
  }

public:
  socket &operator=(socket &&other) noexcept;

  socket &operator=(const socket &other) = delete;

private:
  void close(bool throw_on_error = true);
  virtual void init() = 0;
};
} // namespace liblocket

#endif
