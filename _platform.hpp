#pragma once

/*#if defined(__GNUC__)
#if __LP64__*/
#include "impl/_platform-linux-i64.hpp"/*
#else
#include "impl/_platform-linux-i32.hpp"
#endif
#else
#pragma error "Unknown or not-implemented platform"
#endif
*/

namespace __platform
{
    struct __file_handle;

    extern const __file_handle __stdin;
    extern const __file_handle __stdout;
    extern const __file_handle __stderr;

    void __exit(int rc) noexcept;
    __file_handle __open(const char* filename) noexcept;
    int __close(__file_handle handle) noexcept;
    int __read(__file_handle handle, const void* buffer, unsigned long size) noexcept;
    int __write(__file_handle handle, const void* data, unsigned long size) noexcept;
}
