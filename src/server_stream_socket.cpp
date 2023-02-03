#include "server_stream_socket.hpp"

#include "connected_stream_socket.hpp"
#include "errno_error.hpp"
#include "inet_4_socket_addr.hpp"
#include "inet_6_socket_addr.hpp"
#include "inet_socket_addr.hpp"
#include "socket_addr.hpp"
#include "socket_error.hpp"
#include "socket.hpp"
#include "stream_socket.hpp"
#include "unix_socket_addr.hpp"

#include <sys/socket.h>
#include <unistd.h>

#include <cerrno>
#include <exception>
#include <stdexcept>
#include <string>
#include <utility>

locket::server_stream_socket::server_stream_socket(socket_addr::sock_domain domain)
    : stream_socket{ domain },
      m_is_listening{ false }
{
}

locket::server_stream_socket::server_stream_socket(socket::dummy_type_bind, const socket_addr* bound_addr)
    : stream_socket{ socket::dummy_type_bind{}, bound_addr },
      m_is_listening{ false }
{
}

locket::server_stream_socket::server_stream_socket(socket_addr::sock_domain domain, socket::dummy_type_listen, int backlog/*= m_k_backlog*/)
    : stream_socket{ domain },
      m_is_listening{ false }
{
    listen(backlog);
}

locket::server_stream_socket::server_stream_socket(const socket_addr* bound_addr, int backlog/*= m_k_backlog*/)
    : stream_socket{ socket::dummy_type_bind{}, bound_addr },
      m_is_listening{ false }
{
    listen(backlog);
}

locket::server_stream_socket::server_stream_socket(int sockfd)
    : stream_socket{ sockfd },
      m_is_listening{ false }
{
}

locket::server_stream_socket::server_stream_socket(server_stream_socket&& other)
    : stream_socket{ std::move(other) }, m_is_listening{ std::move(other.m_is_listening) }
{
}

locket::server_stream_socket::~server_stream_socket()
{
}

void locket::server_stream_socket::listen(int backlog/*= m_k_backlog*/)
{
    if (m_is_listening == true)
    {
        throw std::runtime_error{ "socket is already listening" };
    }

    if (::listen(m_sockfd, backlog) == -1)
    {
        throw socket_error{ "listen()", errno };
    }
}

locket::connected_stream_socket locket::server_stream_socket::accept() const
{
    socket_addr* connected_addr{ nullptr };

    try
    {
        if (get_bound_addr() == nullptr)
        {
            throw std::runtime_error{ "socket is not bound" };
        }

        if (m_is_listening == false)
        {
            throw std::runtime_error{ "socket is not listening" };
        }

        switch (get_domain())
        {
            case socket_addr::sock_domain::UNIX:
                connected_addr = new unix_socket_addr{};
                break;
            case socket_addr::sock_domain::INET4:
                connected_addr = new inet4_socket_addr{};
                break;
            case socket_addr::sock_domain::INET6:
                connected_addr = new inet6_socket_addr{};
                break;
            default:
                throw std::runtime_error{ "this should be impossible..." };
                break;
        }

        socklen_t connected_addr_len{ connected_addr->size() };

        int new_sockfd{ ::accept(m_sockfd, connected_addr->socket_addr_ptr(), &connected_addr_len) };

        if (new_sockfd == -1)
        {
            throw socket_error{ "accept()", errno };
        }

        connected_stream_socket connected_stream_socket{ new_sockfd, connected_addr };

        if (connected_addr != nullptr)
        {
            delete connected_addr;
        }

        return connected_stream_socket;

    }
    catch (...)
    {
        if (connected_addr != nullptr)
        {
            delete connected_addr;
        }

        throw;
    }
}

locket::server_stream_socket& locket::server_stream_socket::operator=(server_stream_socket&& other)
{
    if (this == &other)
    {
        return *this;
    }

    m_is_listening = std::move(other.m_is_listening);

    stream_socket::operator=(std::move(other));

    return *this;
}
