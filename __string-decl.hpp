#pragma once
#include <allocator>

namespace std
{
    template<typename Char>
    struct char_traits
    {
        using char_type = Char;
    };

    // Forward declare string
    template<typename Char, typename=char_traits<Char>, typename=allocator<Char>>
    class basic_string;

    typedef basic_string<char> string;
}
