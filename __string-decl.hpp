#pragma once
#include <allocator>
#include <cstring>
#include <__ios_decl>

namespace std
{
    template<typename Char>
    struct char_traits;
    template<>
    struct char_traits<char>
    {
        using char_type = char;
        using int_type = int;
        using off_type = streamoff;
        using pos_type = streampos;
        using state_type = mbstate_t;

        static constexpr void assign(char_type& lhs, const char_type& rhs) noexcept {
            lhs = rhs;
        }

        static char_type* assign(char_type* string, size_t count, char_type value)
        {
            for (size_t i = 0; i < count; i++) { string[i] = value; }
            return string;
        }

        static constexpr bool eq(char_type lhs, char_type rhs) noexcept {
            return lhs == rhs;
        }
        static constexpr bool lt(char_type lhs, char_type rhs) noexcept  {
            return lhs < rhs;
        }

        static char_type* move(char_type* dest, const char_type* source,
            size_t size)
        {
            return copy(dest, source, size);
        }

        static char_type* copy(char_type* dest, const char_type* source,
            size_t size)
        {
            memcpy(dest, source, size * sizeof(char));
            return dest;
        }

        static constexpr int compare(const char_type* lhs,
            const char_type* rhs, size_t size)
        {
            return memcmp(lhs, rhs, size * sizeof(char));
        }

        static constexpr size_t length(const char_type* string) {
            return strlen(string);
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

        static constexpr int_type eof() noexcept
        {
            static_assert(sizeof(char) != sizeof(int),
                "Unsupported value for EOF on this platform");
            return -1;
        }

        static constexpr int_type not_eof(int_type e) noexcept {
            return e == -1 ? 0 : e;
        }
    };

    // Forward declare string
    template<typename Char, typename=char_traits<Char>, typename=allocator<Char>>
    class basic_string;

    typedef basic_string<char> string;
}
