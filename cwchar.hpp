#pragma once
#include <__platform>

namespace std
{
    struct mbstate_t
    {
        unsigned char stage{};
        char bytes[4];
    };

    using wint_t = __platform::wint_t;
}
