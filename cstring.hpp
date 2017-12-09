#pragma once

namespace std
{
#pragma warn "TODO: Make std::size_t platform-specific"

#ifdef __GNUC__
    typedef unsigned long size_t;
#else
    typedef unsigned long long size_t;
#endif

    void* memcpy(void* dest, const void* src, size_t count);
    void* memset(void* ptr, int value, size_t num);

    inline std::size_t strlen(const char* str) {
        for (std::size_t i = 0; true; i++) {
            if (str[i] == '\0') return i;
        }
    }

    inline char* strcpy(char* dest, const char* src) {
        memcpy(dest, src, strlen(src)); return dest;
    }
}