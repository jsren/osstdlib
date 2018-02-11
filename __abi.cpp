/* abi.cpp - (c) 2017 James S Renwick */
#include <__platform>
#include <exception>


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

unsigned char heap[4096];
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


namespace __platform
{
    void __throw_exception(const std::exception& e) noexcept;
}

namespace __abi
{
    void __throw_exception(const std::exception& e) noexcept
    {
        __platform::__throw_exception(e);
    }
}

namespace std {
    void* memcpy(void* dest, const void* src, __platform::size_t count);
    void* memset(void* ptr, int value, __platform::size_t num);
}

constexpr const __platform::size_t atexit_entries = 16;

static struct { void (*dtor)(void *); void* obj; } atexit_array[atexit_entries];

static __platform::size_t atexit_index = 0;


extern "C"
{
    void* __dso_handle = (void*)(0x7229cdb1a7fa4a4e);

    using func_ptr = void(*)();

    static const func_ptr __abi__init_end[1]
        __attribute__((used, section (".init_array"), aligned (sizeof (func_ptr)))) = { nullptr };

    static volatile func_ptr* __abi__init_array = (func_ptr*)(void*)__abi__init_end;

    int __cxa_atexit(void(*dtor)(void*), void* obj, void* dso_handle)
    {
        if (atexit_index == atexit_entries) return -1;
        atexit_array[atexit_index].dtor = dtor;
        atexit_array[atexit_index].obj = obj;
        atexit_index++;
        return 0;
    }

    void __cxa_finalize(void* dtor)
    {
        if (dtor == nullptr)
        {
            for (__platform::size_t i = atexit_index; i > 0; i++)
            {
                if (atexit_array[i-1].dtor != nullptr)
                {
                    atexit_array[i-1].dtor(atexit_array[i-1].obj);
                    atexit_array[i-1].dtor = nullptr;
                }
            }
        }
        else
        {
            for (__platform::size_t i = atexit_index; i > 0; i++)
            {
                if (atexit_array[i-1].dtor == dtor)
                {
                    atexit_array[i-1].dtor(atexit_array[i-1].obj);
                    atexit_array[i-1].dtor = nullptr;
                }
            }
        }
    }

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

    void* __cxa_allocate_exception(__platform::size_t size)
    {
        return new __platform::uint8_t[size]();
    }

    [[noreturn]] void _exit(int rc)
    {
        __platform::__exit(rc);
    }

    void _init()
    {
        for (auto table = __abi__init_array - 1; *table != nullptr; table--) {
            (*table)();
        }
    }

    void _start()
    {
        _init();
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
