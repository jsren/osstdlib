#include "stdexcept.hpp"
#include "string.hpp"

namespace std
{
	__detail::exception_base::exception_base(const std::string& what_arg)
        : message(new char[what_arg.length()])
    {
        std::memcpy(message.get(), what_arg.c_str(), what_arg.length());   
    }
}
