#pragma once

namespace std
{
#pragma warn "TODO: Make std::size_t platform-specific"
    typedef unsigned long long size_t;

    void* memcpy(void* dest, const void* src, std::size_t count);

    inline std::size_t strlen(const char* str) {
        for (std::size_t i = 0; true; i++) {
            if (str[i] == '\0') return i;
        }
    }

    inline char* strcpy(char* dest, const char* src) {
        memcpy(dest, src, strlen(src)); return dest;
    }
}
