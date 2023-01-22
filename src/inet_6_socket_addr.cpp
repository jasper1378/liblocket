#include "inet_6_socket_addr.hpp"

#include "addrinfo_error.hpp"
#include "errno_error.hpp"
#include "inet_socket_addr.hpp"
#include "socket_addr.hpp"

#include <arpa/inet.h>
#include <netdb.h>
#include <netinet/in.h>
#include <sys/socket.h>

#include <cstring>
#include <exception>
#include <stdexcept>
#include <string>

locket::inet6_socket_addr::inet6_socket_addr()
{
    clear();
    set_domain();
}

locket::inet6_socket_addr::inet6_socket_addr(in_port_t port)
{
    clear();
    set_domain();
    init(in6addr_any, port);
}

locket::inet6_socket_addr::inet6_socket_addr(in6_addr addr, in_port_t port)
{
    clear();
    set_domain();
    init(addr, port);
}

locket::inet6_socket_addr::inet6_socket_addr(const std::string& saddr, in_port_t port)
{
    clear();
    set_domain();
    init(saddr, port);
}

locket::inet6_socket_addr::inet6_socket_addr(const sockaddr* other)
{
    if (other->sa_family != AF_INET6)
    {
        throw std::invalid_argument{ "sockaddr is no an inet6 address" };
    }

    std::memcpy(&m_addr, other, m_k_size);
}

locket::inet6_socket_addr::inet6_socket_addr(const socket_addr* other)
{
    if (other->domain() != sock_domain::INET6)
    {
        throw std::invalid_argument{ "socket_addr is not an inet6 address" };
    }

    std::memcpy(&m_addr, other->socket_addr_ptr(), m_k_size);
}

locket::inet6_socket_addr::inet6_socket_addr(const sockaddr_in6& other)
    : m_addr{ other }
{
}

locket::inet6_socket_addr::inet6_socket_addr(const inet6_socket_addr& other)
    : m_addr{ other.m_addr }
{
}

locket::inet6_socket_addr::inet6_socket_addr(inet6_socket_addr&& other)
    : m_addr{ std::move(other.m_addr) }
{
}

locket::inet6_socket_addr::~inet6_socket_addr()
{
}

locket::inet6_socket_addr* locket::inet6_socket_addr::create_new() const
{
    return new inet6_socket_addr{};
}

locket::inet6_socket_addr* locket::inet6_socket_addr::create_clone() const
{
    return new inet6_socket_addr{ *this };
}

locket::socket_addr::sock_domain locket::inet6_socket_addr::domain() const
{
    return sock_domain::INET6;
}

socklen_t locket::inet6_socket_addr::size() const
{
    return m_k_size;
}

bool locket::inet6_socket_addr::is_set() const
{
    sockaddr_in6 empty_addr;
    std::memset(&empty_addr, 0, sizeof(sockaddr_in6));
    empty_addr.sin6_family = AF_INET6;

    return ((std::memcmp(&m_addr, &empty_addr, m_k_size)) == 0);
}

std::string locket::inet6_socket_addr::to_string() const
{
    char address_buf[INET6_ADDRSTRLEN];
    std::string addr_str{};
    addr_str.append(inet_ntop(AF_INET6, &m_addr.sin6_addr, address_buf, INET6_ADDRSTRLEN));
    addr_str.append(":");
    addr_str.append(std::to_string(ntohs(m_addr.sin6_port)));
    return addr_str;
}

sockaddr* locket::inet6_socket_addr::socket_addr_ptr()
{
    return reinterpret_cast<sockaddr*>(&m_addr);
}

const sockaddr* locket::inet6_socket_addr::socket_addr_ptr() const
{
    return reinterpret_cast<const sockaddr*>(&m_addr);
}

sockaddr_in6* locket::inet6_socket_addr::socket_addr_in6_ptr()
{
    return reinterpret_cast<sockaddr_in6*>(&m_addr);
}

const sockaddr_in6* locket::inet6_socket_addr::socket_addr_in6_ptr() const
{
    return reinterpret_cast<const sockaddr_in6*>(&m_addr);
}

in_port_t locket::inet6_socket_addr::port() const
{
    return m_addr.sin6_port;
}

in6_addr locket::inet6_socket_addr::address() const
{
    return m_addr.sin6_addr;
}

locket::inet6_socket_addr& locket::inet6_socket_addr::operator=(const inet6_socket_addr& other)
{
    if (this == &other)
    {
        return *this;
    }

    m_addr = other.m_addr;

    return *this;
}

locket::inet6_socket_addr& locket::inet6_socket_addr::operator=(inet6_socket_addr&& other)
{
    if (this == &other)
    {
        return *this;
    }

    m_addr = std::move(other.m_addr);

    return *this;
}

void locket::inet6_socket_addr::clear()
{
    std::memset(&m_addr, 0, m_k_size);
}

void locket::inet6_socket_addr::set_domain()
{
    m_addr.sin6_family = AF_INET6;
}

void locket::inet6_socket_addr::init(in6_addr addr, in_port_t port)
{
    m_addr.sin6_port = htons(port);
    m_addr.sin6_addr = addr;
}

void locket::inet6_socket_addr::init(const std::string& saddr, in_port_t port)
{
    init(resolve_name(saddr), port);
}

in6_addr locket::inet6_socket_addr::resolve_name(const std::string& saddr)
{
    addrinfo hints;
    std::memset(&hints, 0, sizeof(addrinfo));
    hints.ai_family = AF_INET6;

    addrinfo* result{ nullptr };
    in6_addr resolved_address;

    try
    {
        int getaddrinfo_result{ getaddrinfo(saddr.c_str(), NULL, &hints, &result) };
        if (getaddrinfo_result != 0)
        {
            throw addrinfo_error{ addrinfo_error::func::getaddrinfo, getaddrinfo_result };
        }

        resolved_address = (reinterpret_cast<sockaddr_in6*>(result->ai_addr))->sin6_addr;
    }
    catch (...)
    {
        if (result != nullptr)
        {
            freeaddrinfo(result);
        }

        throw;
    }

    if (result != nullptr)
    {
        freeaddrinfo(result);
    }

    return resolved_address;
}
