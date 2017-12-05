#include <system_error.hpp>
#include <type_traits.hpp>
#include <exception.hpp>
#include <ios.hpp>

namespace __abi
{
    int __ecode_from_enum(std::errc e) noexcept
    {
        return static_cast<int>(static_cast<std::underlying_type_t<decltype(e)>>(e));
    }
    int __ecode_from_enum(std::io_errc e) noexcept
    {
        return static_cast<int>(static_cast<std::underlying_type_t<decltype(e)>>(e));
    }

    void __throw_exception(const std::exception&) noexcept
    {
        // TODO: throw the exception
    }
}
