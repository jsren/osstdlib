#pragma once
#include <__platform>

namespace std
{
    using int8_t = __platform::int8_t;
    using int16_t = __platform::int16_t;
    using int32_t = __platform::int32_t;
    using int64_t = __platform::int64_t;
    using uint8_t = __platform::uint8_t;
    using uint16_t = __platform::uint16_t;
    using uint32_t = __platform::uint32_t;
    using uint64_t = __platform::uint64_t;
}
