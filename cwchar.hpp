#pragma once

namespace std
{
    struct mbstate_t
    {
        unsigned char stage{};
        char bytes[4];
    };
}
