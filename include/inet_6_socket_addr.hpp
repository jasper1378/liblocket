#ifndef LOCKET_INET_6_SOCKET_ADDR_HPP
#define LOCKET_INET_6_SOCKET_ADDR_HPP

#include "inet_socket_addr.hpp"
#include "socket_addr.hpp"

#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>

#include <string>

namespace locket
{
    class inet6_socket_addr : public inet_socket_addr
    {
        private:

            sockaddr_in6 m_addr;
            static constexpr socklen_t m_k_size{ sizeof(sockaddr_in6) };

        public:

            inet6_socket_addr();
            explicit inet6_socket_addr(in_port_t port);
            inet6_socket_addr(in6_addr addr, in_port_t port);
            inet6_socket_addr(const std::string& saddr, in_port_t port);
            explicit inet6_socket_addr(const sockaddr* other);
            explicit inet6_socket_addr(const socket_addr* other);
            explicit inet6_socket_addr(const sockaddr_in6& other);
            inet6_socket_addr(const inet6_socket_addr& other);
            inet6_socket_addr(inet6_socket_addr&& other);

            ~inet6_socket_addr() override;

        public:

            inet6_socket_addr* create_new() const override;
            inet6_socket_addr* create_clone() const override;
            sock_domain domain() const override;
            socklen_t size() const override;
            bool is_set() const override;
            std::string to_string() const override;
            sockaddr* socket_addr_ptr() override;
            const sockaddr* socket_addr_ptr() const override;
            sockaddr_in6* socket_addr_in6_ptr();
            const sockaddr_in6* socket_addr_in6_ptr() const;
            in_port_t port() const override;
            in6_addr address() const;

        public:

            inet6_socket_addr& operator=(const inet6_socket_addr& other);
            inet6_socket_addr& operator=(inet6_socket_addr&& other);

        private:

            void clear() override;
            void set_domain() override;
            void init(in6_addr addr, in_port_t port);
            void init(const std::string& saddr, in_port_t port);
            static in6_addr resolve_name(const std::string& saddr);
    };
}

#endif
