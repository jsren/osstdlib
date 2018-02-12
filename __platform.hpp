#if !defined(__OSSTDLIB_PLATFORM_HEADER)
#define __OSSTDLIB_PLATFORM_HEADER

#include <__platform-impl>


namespace __platform
{
    struct __file_handle;

    struct __result_t;

    struct __open_options
    {
        bool read : 1;
        bool write : 1;
        bool truncate : 1;
        bool append : 1;
        bool create : 1;
        bool failExists : 1;
        bool binary : 1;
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

    void __align_stack() noexcept;
    [[noreturn]] void __exit(int rc) noexcept;
    __result_t __open(const char* filename, __open_options, __file_acl, __file_handle& handle_out) noexcept;
    __result_t __close(__file_handle handle) noexcept;
    __result_t __read(__file_handle handle, const void* buffer, size_t size, size_t& count_out) noexcept;
    __result_t __write(__file_handle handle, const void* data, size_t size, size_t& count_out) noexcept;
    __result_t __seek(__file_handle handle, ssize_t offset, __seek_whence whence, size_t& pos_out) noexcept;
}

#endif
