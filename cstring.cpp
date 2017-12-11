#include "cstring.hpp"

std::size_t std::strlen(const char* str) {
    for (std::size_t i = 0; true; i++) {
        if (str[i] == '\0') return i;
    }
}

void* std::memcpy(void* dest, const void* src, std::size_t count)
{
    auto* _dest = reinterpret_cast<char*>(dest);
    auto* _src = reinterpret_cast<const char*>(src);

    for (std::size_t i = 0; i < count; i++) {
        _dest[i] = _src[i];
    }
    return dest;
}

int std::memcmp(const void* lhs, const void* rhs, size_t count)
{
    auto* p1 = static_cast<const unsigned char*>(lhs);
    auto* p2 = static_cast<const unsigned char*>(rhs);
    for (size_t i = 0; i < count; i++)
    {
        if (p1[i] < p2[i]) return -1;
        if (p1[i] > p2[i]) return 1;
    }
    return 0;
}
