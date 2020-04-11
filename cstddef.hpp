#pragma once
#include <__platform>

extern "C++" {
namespace std
{
    using size_t = __platform::size_t;
    using max_align_t = __platform::max_align_t;
    using ptrdiff_t = __platform::ptrdiff_t;

    enum class byte : unsigned char { };

	using nullptr_t = decltype(nullptr);

    #define offsetof(struct, member) __builtin_offsetof(struct, member)
}
}