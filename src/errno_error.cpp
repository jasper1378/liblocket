#include "errno_error.hpp"

#include <cerrno>
#include <cstring>
#include <exception>
#include <stdexcept>
#include <string>
#include <utility>

locket::errno_error::errno_error(const std::string& what_arg, int errno_num)
    : std::runtime_error{ what_arg },
      m_errno_num{ errno_num },
      m_errno_str{ "" }
{
}

locket::errno_error::errno_error(const char* what_arg, int errno_num)
    : std::runtime_error{ what_arg },
      m_errno_num{ errno_num },
      m_errno_str{ "" }
{
}

locket::errno_error::errno_error(const errno_error& other) noexcept
    : std::runtime_error{ other },
      m_errno_num{ other.m_errno_num },
      m_errno_str{ other.m_errno_str }
{
}

locket::errno_error::errno_error(errno_error&& other) noexcept
    : std::runtime_error{ std::move(other) },
      m_errno_num{ std::move(other.m_errno_num) },
      m_errno_str{ std::move(other.m_errno_str) }
{
}

locket::errno_error::~errno_error()
{
}

const char* locket::errno_error::what() const noexcept
{
    return std::runtime_error::what();
}

int locket::errno_error::get_errno() const noexcept
{
    return m_errno_num;
}

std::string locket::errno_error::get_errno_string() const noexcept
{
    if (m_errno_str == "")
    {
        m_errno_str = errno_to_string(m_errno_num);
    }

    return m_errno_str;
}

locket::errno_error& locket::errno_error::operator=(const errno_error& other) noexcept
{
    if (this == &other)
    {
        return *this;
    }

    m_errno_num = other.m_errno_num;
    m_errno_str = other.m_errno_str;
    std::runtime_error::operator=(other);

    return *this;
}

locket::errno_error& locket::errno_error::operator=(errno_error&& other) noexcept
{
    if (this == &other)
    {
        return *this;
    }

    m_errno_num = std::move(other.m_errno_num);
    m_errno_str = std::move(other.m_errno_str);
    std::runtime_error::operator=(std::move(other));

    return *this;
}

std::string locket::errno_error::errno_to_string(int errno_num) const
{
    return std::string{ std::strerror(errno_num) };
}
