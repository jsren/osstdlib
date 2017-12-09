/* abi.cpp - (c) 2017 James S Renwick */
#include "abi.hpp"

using uint64_t = unsigned long long;
using uint8_t = unsigned char;
using size_t = unsigned long;


inline static bool getInitFlag(const uint64_t& guard)
{
    return reinterpret_cast<const uint8_t&>(guard);
}
inline static void setInitFlag(uint64_t& guard)
{
    reinterpret_cast<uint8_t&>(guard) = true;
}

inline static bool getBusyFlag(const uint64_t& guard)
{
    return reinterpret_cast<const uint8_t*>(&guard)[1];
}
inline static void setBusyFlag(uint64_t& guard, bool value)
{
    reinterpret_cast<uint8_t*>(&guard)[1] = value;
}

int main();

unsigned char heap[1024];
unsigned char* heapStart = heap;


void* operator new(size_t size) {
    auto ptr = heapStart;
    heapStart += size;
    return ptr;
}
void* operator new[](size_t size) {
    auto ptr = heapStart;
    heapStart += size;
    return ptr;
}

void operator delete(void*) noexcept {

}
void operator delete(void*, unsigned long) noexcept {

}
void operator delete[](void*) noexcept {

}

extern "C"
{
    int __cxa_guard_acquire(uint64_t* guard)
    {
        if (getInitFlag(*guard) || getBusyFlag(*guard)) return 0;
        setBusyFlag(*guard, true);
        return 1;
    }

    void __cxa_guard_release(uint64_t* guard)
    {
        setInitFlag(*guard);
    }

    void __cxa_pure_virtual()
    {

    }

    [[noreturn]] void _exit(int rc)
    {
        __asm__ volatile (
            "movq $60, %%rax\n"
            "movl %0, %%edi\n"
            "syscall" : : "r" (rc) : "rax", "rdi"
        );
    }

    void _start()
    {
        _exit(main());
    }
}


namespace abi
{
    __file_handle __stdin{0};
    __file_handle __stdout{1};
    __file_handle __stderr{2};


    int __write(__file_handle handle, const void* data, unsigned long size) noexcept
    {
        auto fd = handle.value;
        int rc;

        __asm__ volatile(
            "movq $1, %%rax\n"
            "movq %1, %%rdi\n"
            "movq %2, %%rsi\n"
            "movq %3, %%rdx\n"
            "syscall\n"
            "movl %%eax, %0"
            : "=r"(rc) : "g"(fd), "g"(data), "g"(size) : "eax", "ebx", "ecx", "edx"
        );
        return rc;
    }
}
