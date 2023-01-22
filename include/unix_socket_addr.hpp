#ifndef LOCKET_UNIX_SOCKET_ADDR_HPP
#define LOCKET_UNIX_SOCKET_ADDR_HPP

#include "socket_addr.hpp"

#include <sys/socket.h>
#include <sys/un.h>

#include <string>

namespace locket
{
    class unix_socket_addr : public socket_addr
    {
        private:

            sockaddr_un m_addr;
            bool m_is_abstract;
            static constexpr socklen_t m_k_size{ sizeof(sockaddr_un) };

        public:

            unix_socket_addr();
            explicit unix_socket_addr(const std::string& path_or_name, bool is_abstract = false);
            explicit unix_socket_addr(const sockaddr* other);
            explicit unix_socket_addr(const socket_addr* other);
            explicit unix_socket_addr(const sockaddr_un& other);
            unix_socket_addr(const unix_socket_addr& other);
            unix_socket_addr(unix_socket_addr&& other);

            ~unix_socket_addr() override;

        public:

            unix_socket_addr* create_new() const override;
            unix_socket_addr* create_clone() const override;
            socket_addr::sock_domain domain() const override;
            socklen_t size() const override;
            bool is_set() const override;
            std::string to_string() const override;
            sockaddr* socket_addr_ptr() override;
            const sockaddr* socket_addr_ptr() const override;
            sockaddr_un* socket_addr_un_ptr();
            const sockaddr_un* socket_addr_un_ptr() const;
            bool is_abstract() const;

        public:

            unix_socket_addr& operator=(const unix_socket_addr& other);
            unix_socket_addr& operator=(unix_socket_addr&& other);

        private:

            void clear() override;
            void set_domain() override;
            bool check_if_abstract() const;
    };
}

#endif
