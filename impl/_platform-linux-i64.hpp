#pragma once

namespace __platform
{
    using uint64_t = unsigned long long;
    using uint8_t = unsigned char;
    using size_t = unsigned long;
    using ssize_t = signed long;

    struct __file_handle
    {
        ssize_t value;
    };
}
