#include "../__platform.hpp"
#include <system_error>
#include <type_traits>
#include <exception>
#include <ios>

namespace __platform
{
    extern const __file_handle __stdin{0};
    extern const __file_handle __stdout{1};
    extern const __file_handle __stderr{2};


    constexpr const int O_RDONLY = 00000;
    constexpr const int O_WRONLY = 00001;
    constexpr const int O_RDWR   = 00002;
    constexpr const int O_CREAT  = 00100;
    constexpr const int O_EXCL   = 00200;
    constexpr const int O_APPEND = 02000;
    constexpr const int O_TRUNC  = 01000;

    constexpr const int S_IRUSR = 00400;
    constexpr const int S_IWUSR = 00200;
    constexpr const int S_IXUSR = 00100;
    constexpr const int S_IRGRP = 00040;
    constexpr const int S_IWGRP = 00020;
    constexpr const int S_IXGRP = 00010;
    constexpr const int S_IROTH = 00004;
    constexpr const int S_IWOTH = 00002;
    constexpr const int S_IXOTH = 00001;

    constexpr const int SEEK_SET = 0;
    constexpr const int SEEK_CUR = 1;
    constexpr const int SEEK_END = 2;


    int __ecode_from_enum(std::errc e) noexcept
    {
        return static_cast<int>(static_cast<std::underlying_type_t<decltype(e)>>(e));
    }
    int __ecode_from_enum(std::io_errc e) noexcept
    {
        return static_cast<int>(static_cast<std::underlying_type_t<decltype(e)>>(e));
    }

    void __throw_exception(const std::exception&) noexcept
    {

    }

    void __align_stack() noexcept
    {
        // Linux-x64 compiler ABI requires 16-byte stack alignment
        // to an 8-byte boundary. So add 8 to offset stack.
        asm volatile (
            "pop %rdi\n"
            "sub $0x8, %rsp\n"
            "push %rdi"
        );
    }

    [[noreturn]] void __exit(int rc) noexcept
    {
        auto code = static_cast<uint64_t>(rc & 0xFF);
        __asm__ volatile (
            "movq $60, %%rax\n"
            "movq %0, %%rdi\n"
            "syscall"
            :
            : "g"(code)
            : "rax", "rcx", "r11", "rdi"
        );
    }


    __result_t __open(const char* filename, __open_options cppflags, __file_acl cppmode,
        __file_handle& handle_out) noexcept
    {
        // Get Linux file flags
        size_t flags = O_RDONLY;
        if (cppflags.write     ) flags |= (cppflags.read ? O_RDWR : O_WRONLY);
        if (cppflags.append    ) flags |= O_APPEND;
        if (cppflags.truncate  ) flags |= O_TRUNC;
        if (cppflags.create    ) flags |= O_CREAT;
        if (cppflags.failExists) flags |= O_EXCL;

        // Get Linux ACL mode if creating file
        size_t mode = 0;
        if (cppflags.create)
        {
            if (cppmode.userRead     ) mode |= S_IRUSR;
            if (cppmode.userWrite    ) mode |= S_IWUSR;
            if (cppmode.userExecute  ) mode |= S_IXUSR;
            if (cppmode.groupRead    ) mode |= S_IRGRP;
            if (cppmode.groupWrite   ) mode |= S_IWGRP;
            if (cppmode.groupExecute ) mode |= S_IXGRP;
            if (cppmode.othersRead   ) mode |= S_IROTH;
            if (cppmode.othersWrite  ) mode |= S_IWOTH;
            if (cppmode.othersExecute) mode |= S_IXOTH;

            // Set default mode
            if (mode == 0) {
                mode = S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH | S_IWOTH;
            }
        }

        ssize_t rc;
        __asm__ volatile(
            "movq $2, %%rax\n"
            "movq %1, %%rdi\n"
            "movq %2, %%rsi\n"
            "movq %3, %%rdx\n"
            "syscall\n"
            "movq %%rax, %0"
            : "=r"(rc)
            : "g"(filename), "g"(flags), "g"(mode)
            : "rax", "rcx", "r11", "rdi", "rsi", "rdx"
        );
        handle_out = __file_handle{rc};
        return __result_t{rc < 0 ? rc : 0};
    }

    __result_t __close(__file_handle file) noexcept
    {
        ssize_t rc;
        __asm__ volatile(
            "movq $3, %%rax\n"
            "movq %1, %%rdi\n"
            "syscall\n"
            "movq %%rax, %0"
            : "=r"(rc)
            : "g"(file.value)
            : "rax", "rcx", "r11", "rdi"
        );
        return __result_t{rc};
    }

    __result_t __read(__file_handle handle, const void* buffer, unsigned long size, size_t& count_out) noexcept
    {
        ssize_t rc;
        __asm__ volatile(
            "movq $0, %%rax\n"
            "movq %1, %%rdi\n"
            "movq %2, %%rsi\n"
            "movq %3, %%rdx\n"
            "syscall\n"
            "movq %%rax, %0"
            : "=r"(rc)
            : "g"(handle.value), "g"(buffer), "g"(size)
            : "rax", "rcx", "r11", "rdi", "rsi", "rdx"
        );
        if (rc > 0) { count_out = rc; return __result_t{}; }
        else return __result_t{rc};
    }

    __result_t __write(__file_handle handle, const void* data, unsigned long size, size_t& count_out) noexcept
    {
        ssize_t rc;
        __asm__ volatile(
            "movq $1, %%rax\n"
            "movq %1, %%rdi\n"
            "movq %2, %%rsi\n"
            "movq %3, %%rdx\n"
            "syscall\n"
            "movq %%rax, %0"
            : "=r"(rc)
            : "g"(handle.value), "g"(data), "g"(size)
            : "rax", "rcx", "r11", "rdi", "rsi", "rdx"
        );
        if (rc > 0) { count_out = rc; return __result_t{}; }
        else return __result_t{rc};
    }

    __result_t __seek(__file_handle handle, ssize_t offset, __seek_whence cppwhence, size_t& pos_out) noexcept
    {
        size_t whence;
        switch (cppwhence) {
            case __seek_whence::start: { whence = SEEK_SET; break; }
            case __seek_whence::current: { whence = SEEK_CUR; break; }
            case __seek_whence::end: { whence = SEEK_END; break; }
        }

        ssize_t rc;
        __asm__ volatile(
            "movq $8, %%rax\n"
            "movq %1, %%rdi\n"
            "movq %2, %%rsi\n"
            "movq %3, %%rdx\n"
            "syscall\n"
            "movq %%rax, %0"
            : "=r"(rc)
            : "g"(handle.value), "g"(offset), "g"(whence)
            : "rax", "rcx", "r11", "rdi", "rsi", "rdx"
        );
        if (rc > 0) { pos_out = rc; return __result_t{}; }
        else return __result_t{rc};
    }

}
