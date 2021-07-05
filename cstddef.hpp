#pragma once
#include <__platform>

extern "C++" {
namespace std
{
    using size_t = __platform::size_t;
    using max_align_t = __platform::max_align_t;
    using ptrdiff_t = __platform::ptrdiff_t;

    enum class byte : unsigned char { };

    namespace _detail {
        template<typename T, typename U>
        struct is_same {static constexpr bool value = false;};
        template<typename T>
        struct is_same<T, T> {static constexpr bool value = true;};

        template<typename T, typename U>
        constexpr bool is_same_v = is_same<T, U>::value;

        template<bool, class T = void> struct enable_if { };
        template<class T> struct enable_if<true, T> { typedef T type; };

        template<typename T>
        using is_integral_v = is_same_v<T, signed char>::value ||
                              is_same_v<T, signed short>::value ||
                              is_same_v<T, signed int>::value ||
                              is_same_v<T, signed long int>::value ||
                              is_same_v<T, signed long long int>::value ||
                              is_same_v<T, unsigned char>::value ||
                              is_same_v<T, unsigned short>::value ||
                              is_same_v<T, unsigned int>::value ||
                              is_same_v<T, unsigned long int>::value ||
                              is_same_v<T, unsigned long long int>::value;

        template<typename T, typename Y=void>
        using en_if_int = typename enable_if<std::is_integral<T>::value, Y>;

        constexpr auto to_int(std::byte value) noexcept {
            return static_cast<__underlying_type(std::byte)>(value);
        }
    }

    template<typename T>
    constexpr _detail::en_if_int<T, T> to_integer(std::byte value) noexcept
    {
        return _detail::to_int(value);
    }

    template<typename T>
    constexpr _detail::en_if_int<T, std::byte&> operator <<=(std::byte& lhs, T rhs) noexcept
    {
        return lhs = std::byte(_detail::to_int(lhs) << rhs);
    }
    template<typename T>
    constexpr _detail::en_if_int<T, std::byte&> operator >>=(std::byte& lhs, T rhs) noexcept
    {
        return lhs = std::byte(_detail::to_int(lhs) >> rhs);
    }

    template<typename T>
    constexpr _detail::en_if_int<T, std::byte> operator <<(std::byte lhs, T rhs) noexcept
    {
        return std::byte(_detail::to_int(lhs) << rhs);
    }
    template<typename T>
    constexpr _detail::en_if_int<T, std::byte> operator >>(std::byte lhs, T rhs) noexcept
    {
        return std::byte(_detail::to_int(lhs) >> rhs);
    }

    constexpr std::byte& operator |=(std::byte& lhs, std::byte rhs) noexcept
    {
        return lhs = std::byte(_detail::to_int(lhs) | _detail::to_int(rhs));
    }
    constexpr std::byte& operator &=(std::byte& lhs, std::byte rhs) noexcept
    {
        return lhs = std::byte(_detail::to_int(lhs) & _detail::to_int(rhs));
    }
    constexpr std::byte& operator ^=(std::byte& lhs, std::byte rhs) noexcept
    {
        return lhs = std::byte(_detail::to_int(lhs) ^ _detail::to_int(rhs));
    }

    constexpr std::byte operator |(std::byte lhs, std::byte rhs) noexcept
    {
        return std::byte(_detail::to_int(lhs) | _detail::to_int(rhs));
    }
    constexpr std::byte operator &(std::byte lhs, std::byte rhs) noexcept
    {
        return std::byte(_detail::to_int(lhs) & _detail::to_int(rhs));
    }
    constexpr std::byte operator ^(std::byte lhs, std::byte rhs) noexcept
    {
        return std::byte(_detail::to_int(lhs) ^ _detail::to_int(rhs));
    }
    constexpr std::byte operator ~(std::byte lhs) noexcept
    {
        return std::byte(~_detail::to_int(lhs));
    }


	using nullptr_t = decltype(nullptr);

    #define offsetof(struct, member) __builtin_offsetof(struct, member)
}
}
