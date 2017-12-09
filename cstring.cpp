#include "cstring.hpp"

void* std::memcpy(void* dest, const void* src, std::size_t count)
{
    auto* _dest = reinterpret_cast<char*>(dest);
    auto* _src = reinterpret_cast<const char*>(src);

    for (std::size_t i = 0; i < count; i++) {
        _dest[i] = _src[i];
    }
    return dest;
}