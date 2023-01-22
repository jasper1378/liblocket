#include "dgram_socket.hpp"

#include "errno_error.hpp"
#include "inet_4_socket_addr.hpp"
#include "inet_6_socket_addr.hpp"
#include "inet_socket_addr.hpp"
#include "socket_addr.hpp"
#include "socket_error.hpp"
#include "socket.hpp"
#include "unix_socket_addr.hpp"

#include <sys/socket.h>
#include <unistd.h>

#include <cerrno>
#include <cstddef>
#include <exception>
#include <stdexcept>
#include <string>
#include <utility>

locket::dgram_socket::dgram_socket(socket_addr::sock_domain domain)
    : socket{ domain },
      m_connected_addr{ nullptr },
      m_last_sender_addr{ nullptr }
{
    init();
}

locket::dgram_socket::dgram_socket(socket::dummy_type_bind, const socket_addr* bound_addr)
    : socket{ bound_addr->domain() },
      m_connected_addr{ nullptr },
      m_last_sender_addr{ nullptr }
{
    init();
    bind(bound_addr);
}

locket::dgram_socket::dgram_socket(socket::dummy_type_connect, const socket_addr* connected_addr)
    : socket{ connected_addr->domain() },
      m_connected_addr{ nullptr},
      m_last_sender_addr{ nullptr }
{
    init();
    connect(connected_addr);
}

locket::dgram_socket::dgram_socket(const socket_addr* bound_addr, const socket_addr* connected_addr)
    : socket{ bound_addr->domain() },
      m_connected_addr{ nullptr },
      m_last_sender_addr{ nullptr }
{
    init();
    bind(bound_addr);
    connect(connected_addr);
}

locket::dgram_socket::dgram_socket(int sockfd)
    : socket{ sockfd },
      m_connected_addr{ nullptr },
      m_last_sender_addr{ nullptr }
{
    if (get_socket_option<int>(SO_TYPE) != SOCK_DGRAM)
    {
        throw std::runtime_error{ "socket is not a datagram socket" };
    }
}

locket::dgram_socket::dgram_socket(dgram_socket&& other)
    : socket{ std::move(other) },
      m_connected_addr{ other.m_connected_addr },
      m_last_sender_addr{ other.m_last_sender_addr }
{
    other.m_connected_addr = nullptr;
    other.m_last_sender_addr = nullptr;
}

locket::dgram_socket::~dgram_socket()
{
    if (m_connected_addr != nullptr)
    {
        delete m_connected_addr;
        m_connected_addr = nullptr;
    }

    if (m_last_sender_addr != nullptr)
    {
        delete m_last_sender_addr;
        m_last_sender_addr = nullptr;
    }
}

void locket::dgram_socket::connect(const socket_addr* connect_addr)
{
    if (m_connected_addr != nullptr)
    {
        throw std::runtime_error{ "socket is already connected" };
    }

    if (get_domain() != connect_addr->domain())
    {
        throw std::runtime_error{ "domains of socket and connect address do not match" };
    }

    if (::connect(m_sockfd, connect_addr->socket_addr_ptr(), connect_addr->size()) == -1)
    {
        throw socket_error{ "connect()", errno };
    }

    if (m_connected_addr != nullptr)
    {
        delete m_connected_addr;
    }

    m_connected_addr = connect_addr->create_clone();
}

std::string locket::dgram_socket::recv(int flags/*= 0*/) const
{
    char message_buffer[m_k_max_message_length];
    ssize_t bytes_revcd{};

    if (m_connected_addr != nullptr)
    {
        bytes_revcd = ::recv(m_sockfd, message_buffer, m_k_max_message_length, flags);
        if (bytes_revcd == -1)
        {
            throw socket_error{ "recv()", errno };
        }
    }
    else if (m_connected_addr == nullptr)
    {
        socket_addr* last_sender_addr{ nullptr };

        try
        {
            switch(get_domain())
            {
                case socket_addr::sock_domain::UNIX:
                    last_sender_addr = new unix_socket_addr;
                    break;
                case socket_addr::sock_domain::INET4:
                    last_sender_addr = new inet4_socket_addr;
                    break;
                case socket_addr::sock_domain::INET6:
                    last_sender_addr = new inet6_socket_addr;
                    break;
                default:
                    throw std::runtime_error{ "this should be impossible..." };
                    break;
            }

            socklen_t last_peer_addr_size{ last_sender_addr->size() };

            bytes_revcd = recvfrom(m_sockfd, message_buffer, m_k_max_message_length, flags, last_sender_addr->socket_addr_ptr(), &last_peer_addr_size);
            if (bytes_revcd == -1)
            {
                if (last_sender_addr != nullptr)
                {
                    delete last_sender_addr;
                }

                throw socket_error{ "recvfrom()", errno };
            }

            message_buffer[bytes_revcd] = '\0';
        }
        catch (...)
        {
            if (last_sender_addr != nullptr)
            {
                delete last_sender_addr;
            }

            throw;
        }

        if (m_last_sender_addr != nullptr)
        {
            delete m_last_sender_addr;
            m_last_sender_addr = nullptr;
        }

        m_last_sender_addr = last_sender_addr;
    }


    return std::string{ message_buffer };
}

void locket::dgram_socket::send(const std::string& message, const socket_addr* peer_addr/*= nullptr*/, int flags/*= 0*/) const
{
    if ((m_connected_addr != nullptr) && (peer_addr != nullptr))
    {
        throw std::runtime_error{ "socket is connected but peer address is specified" };
    }
    if ((m_connected_addr == nullptr) && (peer_addr == nullptr))
    {
        throw std::runtime_error{ "socket is not connected but no peer address is specified" };
    }

    if (peer_addr != nullptr)
    {
        if (get_domain() != peer_addr->domain())
        {
            throw std::runtime_error{ "domains of socket and peer address do not match" };
        }
    }

    size_t message_len{ message.size() + 1 };
    ssize_t bytes_sent{};

    if (m_connected_addr != nullptr)
    {
        bytes_sent = ::send(m_sockfd, message.c_str(), message_len, flags);
        if (bytes_sent != static_cast<ssize_t>(message_len))
        {
            throw socket_error{ "send()", errno };
        }
    }
    else if (m_connected_addr == nullptr)
    {
        bytes_sent = sendto(m_sockfd, message.c_str(), message_len, flags, peer_addr->socket_addr_ptr(), peer_addr->size());
        if (bytes_sent != static_cast<ssize_t>(message_len))
        {
            throw socket_error{ "sendto()", errno };
        }
    }
}

locket::socket_addr* locket::dgram_socket::get_connected_peer_addr() const
{
    if (m_connected_addr == nullptr)
    {
        return nullptr;
    }

    return m_connected_addr->create_clone();
}

locket::socket_addr* locket::dgram_socket::get_last_sender_addr() const
{
    if (m_last_sender_addr == nullptr)
    {
        return nullptr;
    }

    return m_last_sender_addr->create_clone();
}

locket::dgram_socket& locket::dgram_socket::operator=(dgram_socket&& other)
{
    if (this == &other)
    {
        return *this;
    }

    if (m_last_sender_addr != nullptr)
    {
        delete m_last_sender_addr;
        m_last_sender_addr = nullptr;
    }

    if (m_connected_addr != nullptr)
    {
        delete m_connected_addr;
        m_connected_addr = nullptr;
    }

    m_connected_addr = other.m_connected_addr;
    m_last_sender_addr = other.m_last_sender_addr;

    other.m_connected_addr = nullptr;
    other.m_last_sender_addr = nullptr;

    socket::operator=(std::move(other));

    return *this;
}

void locket::dgram_socket::init()
{
    m_sockfd = ::socket((static_cast<int>(get_domain())), SOCK_DGRAM, 0);
    if (m_sockfd == -1)
    {
        throw socket_error{ "socket()", errno };
    }
}
