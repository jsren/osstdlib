/* abi.cpp - (c) 2017-18 James S Renwick */
#include <__platform>
#include <exception>
#include <typeinfo>


namespace std
{
    void* memcpy(void* dest, const void* src, __platform::size_t count);
    void* memset(void* ptr, int value, __platform::size_t num);

    void abort();
    void terminate() noexcept;
    void exit(int);
}


static bool getInitFlag(const __platform::uint64_t& guard)
{
    return static_cast<__platform::uint8_t>(guard);
}
static void setInitFlag(__platform::uint64_t& guard)
{
    guard |= static_cast<__platform::uint8_t>(0x1);
}

static bool getBusyFlag(const __platform::uint64_t& guard)
{
    return static_cast<__platform::uint8_t>(guard >> 8);
}
static void setBusyFlag(__platform::uint64_t& guard, bool value)
{
    guard |= static_cast<__platform::uint8_t>((value ? 1 : 0) << 8);
}


int main();

static unsigned char heap[8192];
static unsigned char* heapStart = heap;
static unsigned char* heapEnd = heap + sizeof(heap);


void* operator new(__platform::size_t size)
{
    if (heapStart + size > heapEnd) {
        std::terminate();
    }
    auto ptr = heapStart;
    heapStart += size;
    return ptr;
}
void* operator new[](__platform::size_t size)
{
    if (heapStart + size > heapEnd) {
        std::terminate();
    }
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
void operator delete[](void*, unsigned long) noexcept {

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
    bool __uncaught_exception()
    {
        return false;
    }
}


constexpr const __platform::size_t atexit_entries = 16;

static struct { void (*dtor)(void *); void* obj; void* handle; } atexit_array[atexit_entries];

static __platform::size_t atexit_index = 0;


extern "C"
{
    void* __dso_handle = (void*)(0x7229cdb1a7fa4a4e);

    using func_ptr = void(*)();

    static func_ptr __abi__init_end[1]
        __attribute__((section (".init_array"), aligned (sizeof (func_ptr)))) = { nullptr };

    static volatile func_ptr* __abi__init_array = (func_ptr*)(void*)__abi__init_end;

    int __cxa_atexit(void(*dtor)(void*), void* obj, void* dso_handle)
    {
        if (atexit_index == atexit_entries) return -1;
        atexit_array[atexit_index].dtor = dtor;
        atexit_array[atexit_index].obj = obj;
        atexit_array[atexit_index].handle = dso_handle;
        atexit_index++;
        return 0;
    }

    void __cxa_finalize(void* handle)
    {
        if (handle == nullptr)
        {
            for (__platform::size_t i = atexit_index; i > 0; i--)
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
            for (__platform::size_t i = atexit_index; i > 0; i--)
            {
                if (atexit_array[i-1].handle == handle &&
                    atexit_array[i-1].dtor != nullptr)
                {
                    atexit_array[i-1].dtor(atexit_array[i-1].obj);
                    atexit_array[i-1].dtor = nullptr;
                }
            }
        }
    }

    int __cxa_guard_acquire(__platform::uint64_t* guard)
    {
        if (getInitFlag(*guard)) return 0;
        if (getBusyFlag(*guard)) return 1;

        setBusyFlag(*guard, true);
        return 1;
    }

    void __cxa_guard_release(__platform::uint64_t* guard)
    {
        setInitFlag(*guard);
    }

    void __cxa_guard_abort(__platform::uint64_t* guard)
    {
        setBusyFlag(*guard, false);
    }

    void __cxa_pure_virtual()
    {
        std::terminate();
    }

    void __cxa_deleted_virtual()
    {
        std::terminate();
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
        auto table = __abi__init_array - 1;
        for (; *table != nullptr; table--) {
        }
        table++;
        for (; *table != nullptr; table++) {
            (*table)();
        }
    }

    void _start()
    {
        __platform::__pre_start();
        ::_init();
        std::exit(main());
    }

    void* memcpy(void* dest, const void* src, __platform::size_t count)
    {
        return std::memcpy(dest, src, count);
    }
    void* memset(void* ptr, int value, __platform::size_t num)
    {
        return std::memset(ptr, value, num);
    }

    void __stack_chk_fail()
    {
        std::abort();
    }

}


namespace std
{
    type_info::type_info(const char* name)
        : __type_name(name) { }

    type_info::~type_info() { }

    bool type_info::operator==(const type_info& other) const noexcept
    {
        return __type_name == other.__type_name;
    }

    bool type_info::operator!=(const type_info& other) const noexcept
    {
        return !operator==(other);
    }

    bool type_info::before(const type_info& other) const noexcept
    {
        return __type_name < other.__type_name;
    }

    const char* type_info::name() const noexcept
    {
        return __type_name;
    }

    size_t type_info::hash_code() const
    {
        return reinterpret_cast<size_t>(__type_name);
    }
}
