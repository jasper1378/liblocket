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

locket::connected_stream_socket::connected_stream_socket(int sockfd, const socket_addr* connected_addr)
    : stream_socket{ sockfd },
      m_connected_addr{ nullptr }
{
    if (connected_addr == nullptr)
    {
        throw std::runtime_error{ "connected address is null" };
    }

    m_connected_addr = connected_addr->create_clone();
}

locket::connected_stream_socket::connected_stream_socket(connected_stream_socket&& other)
    : stream_socket{ std::move(other) },
      m_connected_addr{ other.m_connected_addr }
{
    other.m_connected_addr = nullptr;
}

locket::connected_stream_socket::~connected_stream_socket()
{
    if (m_connected_addr != nullptr)
    {
        delete m_connected_addr;
        m_connected_addr = nullptr;
    }
}

std::string locket::connected_stream_socket::recv(int flags/*= 0*/) const
{
    return stream_socket::recv(flags);
}

void locket::connected_stream_socket::send(const std::string& message, int flags/*= 0*/) const
{
    stream_socket::send(message, flags);
}

locket::connected_stream_socket& locket::connected_stream_socket::operator=(connected_stream_socket&& other)
{
    if (this == &other)
    {
        return *this;
    }

    if (m_connected_addr != nullptr)
    {
        delete m_connected_addr;
        m_connected_addr = nullptr;
    }

    m_connected_addr = other.m_connected_addr;

    other.m_connected_addr = nullptr;

    stream_socket::operator=(std::move(other));

    return *this;
}

locket::socket_addr* locket::connected_stream_socket::get_bound_addr() const
{
    throw std::runtime_error{ "connected_stream_sockets can not be bound. Yes, I'm well aware that this violates the principle of having a common interface for all derived classes." };
    return nullptr;
}

void locket::connected_stream_socket::bind(__attribute__ ((unused)) const socket_addr* bound_addr)
{
    throw std::runtime_error{ "connected_stream_sockets can not be bound. Yes, I'm well aware that this violates the principle of having a common interface for all derived classes." };
}