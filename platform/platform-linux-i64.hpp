#pragma once

namespace __platform
{
    using size_t = unsigned long;
    using ssize_t = signed long;
    using ptrdiff_t = signed long long;

    using max_align_t = long double;

    typedef signed char int8_t;
    typedef short       int16_t;
    typedef long        int32_t;
    typedef long long   int64_t;

    typedef unsigned char      uint8_t;
    typedef unsigned short     uint16_t;
    typedef unsigned long      uint32_t;
    typedef unsigned long long uint64_t;

    struct __file_handle
    {
        ssize_t value;
    };

    struct __result_t
    {
        ssize_t rc{};

        operator bool() const noexcept {
            return rc == 0;
        }
    };
}
