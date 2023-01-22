#ifndef LOCKET_ERRNO_ERROR_HPP
#define LOCKET_ERRNO_ERROR_HPP

#include <cerrno>
#include <cstring>

#include <exception>
#include <stdexcept>
#include <string>

namespace locket
{
    class errno_error : public std::runtime_error
    {
        private:

            int m_errno_num;
            mutable std::string m_errno_str;

        public:

            errno_error(const std::string& what_arg, int errno_num);
            errno_error(const char* what_arg, int errno_num);
            errno_error(const errno_error& other) noexcept;
            errno_error(errno_error&& other) noexcept;

            virtual ~errno_error();

        public:

            virtual const char* what() const noexcept;
            virtual int get_errno() const noexcept;
            virtual std::string get_errno_string() const noexcept;

        public:

            errno_error& operator=(const errno_error& other) noexcept;
            errno_error& operator=(errno_error&& other) noexcept;

        protected:

            virtual std::string errno_to_string(int errno_num) const;
    };
}

#endif
