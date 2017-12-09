#include "_platform-linux-i64.hpp"

namespace __platform
{
    extern const __file_handle __stdin{0};
    extern const __file_handle __stdout{1};
    extern const __file_handle __stderr{2};


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

    __file_handle __open(const char* filename) noexcept
    {

    }

    int __close(__file_handle file) noexcept
    {

    }

    int __read(__file_handle handle, const void* buffer, unsigned long size) noexcept
    {
        int rc;
        __asm__ volatile(
            "movq $0, %%rax\n"
            "movq %1, %%rdi\n"
            "movq %2, %%rsi\n"
            "movq %3, %%rdx\n"
            "syscall\n"
            "movl %%eax, %0"
            : "=r"(rc)
            : "g"(handle.value), "g"(buffer), "g"(size)
            : "eax", "ebx", "ecx", "edx"
        );
        return rc;
    }

    int __write(__file_handle handle, const void* data, unsigned long size) noexcept
    {
        int rc;
        __asm__ volatile(
            "movq $1, %%rax\n"
            "movq %1, %%rdi\n"
            "movq %2, %%rsi\n"
            "movq %3, %%rdx\n"
            "syscall\n"
            "movl %%eax, %0"
            : "=r"(rc)
            : "g"(handle.value), "g"(data), "g"(size)
            : "eax", "ebx", "ecx", "edx"
        );
        return rc;
    }
}
