#include "../_platform.hpp"
#include "_platform-linux-i64.hpp"

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


    [[noreturn]] void __exit(int rc) noexcept
    {
        __asm__ volatile (
            "movq $60, %%rax\n"
            "movl %0, %%edi\n"
            "syscall"
            :
            : "r" (rc)
            : "rax", "rdi"
        );
    }

    __file_handle __open(const char* filename, __open_options cppflags, __file_acl cppmode) noexcept
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
            : "rax", "rdi", "rsi", "rdx"
        );
        return __file_handle{rc};
    }

    ssize_t __close(__file_handle file) noexcept
    {
        ssize_t rc;
        __asm__ volatile(
            "movq $3, %%rax\n"
            "movq %1, %%rdi\n"
            "syscall\n"
            "movq %%rax, %0"
            : "=r"(rc)
            : "g"(file.value)
            : "rax", "rdi"
        );
        return rc;
    }

    ssize_t __read(__file_handle handle, const void* buffer, unsigned long size) noexcept
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
            : "rax", "rdi", "rsi", "rdx"
        );
        return rc;
    }

    ssize_t __write(__file_handle handle, const void* data, unsigned long size) noexcept
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
            : "rax", "rdi", "rsi", "rdx"
        );
        return rc;
    }

    ssize_t __seek(__file_handle handle, ssize_t offset, __seek_whence cppwhence) noexcept
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
            : "rax", "rdi", "rsi", "rdx"
        );
        return rc;
    }

}
