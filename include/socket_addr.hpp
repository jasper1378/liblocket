#ifndef LOCKET_SOCKET_ADDR_HPP
#define LOCKET_SOCKET_ADDR_HPP

#include <sys/socket.h>

#include <string>

namespace locket
{
    class socket_addr
    {
        public:

            enum class sock_domain
            {
                UNK = 0,
                UNIX = AF_UNIX,
                INET4 = AF_INET,
                INET6 = AF_INET6,
            };


        public:

            virtual ~socket_addr();

        public:

            virtual socket_addr* create_new() const =0;
            virtual socket_addr* create_clone() const =0;
            virtual sock_domain domain() const =0;
            virtual socklen_t size() const =0;
            virtual bool is_set() const =0;
            virtual std::string to_string() const =0;
            virtual sockaddr* socket_addr_ptr() =0;
            virtual const sockaddr* socket_addr_ptr() const =0;

        protected:

            virtual void clear() =0;
            virtual void set_domain() =0;
    };
}

#endif
