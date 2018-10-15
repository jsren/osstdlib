#pragma once
#include <cstdint>
#include <cstddef>

inline void* operator new(std::size_t size, void* address) {
    return address;
}
inline void* operator new[](std::size_t size, void* address) {
    return address;
}
