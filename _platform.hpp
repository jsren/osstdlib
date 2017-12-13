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

    struct __open_options
    {
        bool read : 1;
        bool write : 1;
        bool truncate : 1;
        bool append : 1;
        bool create : 1;
        bool failExists : 1;
    };

    struct __file_acl
    {
        bool userRead : 1;
        bool userWrite : 1;
        bool userExecute : 1;
        bool groupRead : 1;
        bool groupWrite : 1;
        bool groupExecute : 1;
        bool othersRead : 1;
        bool othersWrite : 1;
        bool othersExecute : 1;
        bool set_uid : 1;
        bool set_gid : 1;
    };

    enum class __seek_whence
    {
        start,
        current,
        end
    };

    extern const __file_handle __stdin;
    extern const __file_handle __stdout;
    extern const __file_handle __stderr;

    [[noreturn]] void __exit(int rc) noexcept;
    __file_handle __open(const char* filename, __open_options, __file_acl) noexcept;
    ssize_t __close(__file_handle handle) noexcept;
    ssize_t __read(__file_handle handle, const void* buffer, size_t size) noexcept;
    ssize_t __write(__file_handle handle, const void* data, size_t size) noexcept;
    ssize_t __seek(__file_handle handle, ssize_t offset, __seek_whence whence) noexcept;
}
