#pragma once
#include <allocator>
#include <cstring>
#include <.ios-decl>
#include <cwchar>
#include <cstdint>

extern "C++" {
namespace std
{
    namespace __detail
    {
        inline static constexpr int memcmp(const void* lhs, const void* rhs, size_t count) noexcept
        {
            auto* p1 = static_cast<const unsigned char*>(lhs);
            auto* p2 = static_cast<const unsigned char*>(rhs);
            for (size_t i = 0; i < count; i++)
            {
                if (p1[i] < p2[i]) return -1;
                if (p1[i] > p2[i]) return 1;
            }
            return 0;
        }

        inline static constexpr void* memcpy(void* dest, const void* src, size_t count) noexcept
        {
            auto* _dest = static_cast<char*>(dest);
            auto* _src = static_cast<const char*>(src);

            for (size_t i = 0; i < count; i++) {
                _dest[i] = _src[i];
            }
            return dest;
        }

        inline static constexpr size_t strlen(const char* str) noexcept {
            for (size_t i = 0; true; i++) {
                if (str[i] == '\0') return i;
            }
        }
    }

    template<typename Char>
    struct char_traits;

    template<typename char_type>
    struct char_traits_comp_base
    {
        static constexpr bool eq(char_type lhs, char_type rhs) noexcept {
            return lhs == rhs;
        }
        static constexpr bool lt(char_type lhs, char_type rhs) noexcept  {
            return lhs < rhs;
        }
    };

    template<>
    struct char_traits_comp_base<char>
    {
        static constexpr bool eq(char lhs, char rhs) noexcept {
            return static_cast<unsigned char>(lhs) == static_cast<unsigned char>(rhs);
        }
        static constexpr bool lt(char lhs, char rhs) noexcept  {
            return static_cast<unsigned char>(lhs) < static_cast<unsigned char>(rhs);
        }
    };


    template<typename CharType, typename IntType, typename PosType>
    struct char_traits_base : char_traits_comp_base<CharType>
    {
        using char_type  = CharType;
        using int_type   = IntType;
        using off_type   = streamoff;
        using pos_type   = PosType;
        using state_type = mbstate_t;

        using char_traits_comp_base<CharType>::eq;
        using char_traits_comp_base<CharType>::lt;

        static constexpr void assign(char_type& lhs, const char_type& rhs) noexcept {
            lhs = rhs;
        }

        static char_type* assign(char_type* string, size_t count, char_type value)
        {
            for (size_t i = 0; i < count; i++) { string[i] = value; }
            return string;
        }

        static char_type* move(char_type* dest, const char_type* source,
            size_t size)
        {
            return copy(dest, source, size);
        }

        static char_type* copy(char_type* dest, const char_type* source,
            size_t size)
        {
            __detail::memcpy(dest, source, size * sizeof(char_type));
            return dest;
        }

        static constexpr int compare(const char_type* lhs,
            const char_type* rhs, size_t size)
        {
            return __detail::memcmp(lhs, rhs, size * sizeof(char_type));
        }

        static constexpr size_t length(const char_type* string) {
            return __detail::strlen(string);
        }

        static constexpr const char_type* find(const char_type* string,
            size_t size, const char_type& target)
        {
            for (size_t i = 0; i < size; i++) {
                if (string[i] == target) return &string[i];
            }
            return nullptr;
        }

        static constexpr char_type to_char_type(int_type value) noexcept {
            return static_cast<char_type>(value);
        }

        static constexpr int_type to_int_type(char_type value) noexcept {
            return static_cast<int_type>(value);
        }

        static constexpr bool eq_int_type(int_type lhs, int_type rhs) noexcept {
            return lhs == rhs;
        }
    };

    template<>
    struct char_traits<char> : char_traits_base<char, int, streampos>
    {
        using base = char_traits_base<char, int, streampos>;

        using base::char_type;
        using base::int_type;
        using base::off_type;
        using base::pos_type;
        using base::state_type;

        static constexpr int_type eof() noexcept
        {
            static_assert(sizeof(char_type) != sizeof(int_type),
                "Unsupported value for EOF on this platform");
            return -1;
        }

        static constexpr int_type not_eof(int_type e) noexcept {
            return e == eof() ? 0 : e;
        }
    };

    template<>
    struct char_traits<wchar_t> : char_traits_base<wchar_t, wint_t, wstreampos>
    {
        using base = char_traits_base<wchar_t, wint_t, wstreampos>;

        using base::char_type;
        using base::int_type;
        using base::off_type;
        using base::pos_type;
        using base::state_type;

        static constexpr int_type eof() noexcept {
            return __platform::WEOF;
        }

        static constexpr int_type not_eof(int_type e) noexcept {
            return e == eof() ? 0 : e;
        }
    };

    template<>
    struct char_traits<char16_t> : char_traits_base<char16_t, uint_least16_t, u16streampos>
    {
        using base = char_traits_base<char16_t, uint_least16_t, u16streampos>;

        using base::char_type;
        using base::int_type;
        using base::off_type;
        using base::pos_type;
        using base::state_type;

        static constexpr int_type eof() noexcept {
            return static_cast<char16_t>(-1);
        }

        static constexpr int_type not_eof(int_type e) noexcept {
            return e == eof() ? 0 : e;
        }
    };

    template<>
    struct char_traits<char32_t> : char_traits_base<char32_t, uint_least32_t, u32streampos>
    {
        using base = char_traits_base<char32_t, uint_least32_t, u32streampos>;

        using base::char_type;
        using base::int_type;
        using base::off_type;
        using base::pos_type;
        using base::state_type;

        static constexpr int_type eof() noexcept {
            return static_cast<char32_t>(-1);
        }

        static constexpr int_type not_eof(int_type e) noexcept {
            return e == eof() ? 0 : e;
        }
    };



    // Forward declare string
    template<typename Char, typename=char_traits<Char>, typename=allocator<Char>>
    class basic_string;

    typedef basic_string<char> string;
    typedef basic_string<wchar_t> wstring;
    typedef basic_string<char16_t> u16string;
    typedef basic_string<char32_t> u32string;
}
}