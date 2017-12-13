/* abi.cpp - (c) 2017 James S Renwick */
#include "_platform.hpp"


inline static bool getInitFlag(const __platform::uint64_t& guard)
{
    return reinterpret_cast<const __platform::uint8_t&>(guard);
}
inline static void setInitFlag(__platform::uint64_t& guard)
{
    reinterpret_cast<__platform::uint8_t&>(guard) = true;
}

inline static bool getBusyFlag(const __platform::uint64_t& guard)
{
    return reinterpret_cast<const __platform::uint8_t*>(&guard)[1];
}
inline static void setBusyFlag(__platform::uint64_t& guard, bool value)
{
    reinterpret_cast<__platform::uint8_t*>(&guard)[1] = value;
}

int main();

unsigned char heap[1024];
unsigned char* heapStart = heap;


void* operator new(__platform::size_t size) {
    auto ptr = heapStart;
    heapStart += size;
    return ptr;
}
void* operator new[](__platform::size_t size) {
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

namespace std {
    void* memcpy(void* dest, const void* src, __platform::size_t count);
    void* memset(void* ptr, int value, __platform::size_t num);
}

extern "C"
{
    int __cxa_guard_acquire(__platform::uint64_t* guard)
    {
        if (getInitFlag(*guard) || getBusyFlag(*guard)) return 0;
        setBusyFlag(*guard, true);
        return 1;
    }

    void __cxa_guard_release(__platform::uint64_t* guard)
    {
        setInitFlag(*guard);
    }

    void __cxa_pure_virtual()
    {

    }

    [[noreturn]] void _exit(int rc)
    {
        __platform::__exit(rc);
    }

    void _start()
    {
        _exit(main());
    }

    void* memcpy(void* dest, const void* src, __platform::size_t count)
    {
        return std::memcpy(dest, src, count);
    }
    void* memset(void* ptr, int value, __platform::size_t num)
    {
        return std::memset(ptr, value, num);
    }
}
