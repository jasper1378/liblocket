#ifndef LOCKET_DGRAM_SOCKET_HPP
#define LOCKET_DGRAM_SOCKET_HPP

#include "inet_4_socket_addr.hpp"
#include "inet_6_socket_addr.hpp"
#include "inet_socket_addr.hpp"
#include "socket_addr.hpp"
#include "socket.hpp"
#include "unix_socket_addr.hpp"

#include <sys/socket.h>

#include <string>

namespace locket
{
    class dgram_socket : public socket
    {
        private:

            socket_addr* m_connected_addr;
            mutable socket_addr* m_last_sender_addr;

        public:

            explicit dgram_socket(socket_addr::sock_domain domain);
            dgram_socket(socket::dummy_type_bind, const socket_addr* bound_addr);
            dgram_socket(socket::dummy_type_connect, const socket_addr* connected_addr);
            dgram_socket(const socket_addr* bound_addr, const socket_addr* connected_addr);
            explicit dgram_socket(int sockfd);
            dgram_socket(dgram_socket&& other);

            dgram_socket(const dgram_socket& other) =delete;

            ~dgram_socket() override;

        public:

            void connect(const socket_addr* connect_addr);
            std::string recv(int flags = 0) const;
            void send(const std::string& message, const socket_addr* peer_addr = nullptr, int flags = 0) const;
            socket_addr* get_connected_peer_addr() const;
            socket_addr* get_last_sender_addr() const;

        public:

            dgram_socket& operator=(dgram_socket&& other);

            dgram_socket& operator=(const dgram_socket& other) =delete;

        private:

            void init() override;
    };
}

#endif
