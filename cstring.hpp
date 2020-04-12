#pragma once
#include <cstddef>

extern "C++" {
namespace std
{
    void* memcpy(void* dest, const void* src, size_t count);
    void* memset(void* ptr, int value, size_t num);
    int memcmp(const void* lhs, const void* rhs, size_t count);

    size_t strlen(const char* str);

    inline char* strcpy(char* dest, const char* src) {
        memcpy(dest, src, strlen(src)); return dest;
    }

    int strcmp(const char* lhs, const char* rhs);
}
}
