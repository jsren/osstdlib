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

    using int_fast8_t = __platform::int_fast8_t;
    using int_fast16_t = __platform::int_fast16_t;
    using int_fast32_t = __platform::int_fast32_t;
    using int_fast64_t = __platform::int_fast64_t;

    using int_least8_t = __platform::int_least8_t;
    using int_least16_t = __platform::int_least16_t;
    using int_least32_t = __platform::int_least32_t;
    using int_least64_t = __platform::int_least64_t;

    using intmax_t = __platform::intmax_t;
    using intptr = __platform::intptr;

    using uint_fast8_t = __platform::uint_fast8_t;
    using uint_fast16_t = __platform::uint_fast16_t;
    using uint_fast32_t = __platform::uint_fast32_t;
    using uint_fast64_t = __platform::uint_fast64_t;

    using uint_least8_t = __platform::uint_least8_t;
    using uint_least16_t = __platform::uint_least16_t;
    using uint_least32_t = __platform::uint_least32_t;
    using uint_least64_t = __platform::uint_least64_t;

    using uintmax_t = __platform::uintmax_t;
    using uintptr = __platform::uintptr;
}
