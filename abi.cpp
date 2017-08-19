/* abi.cpp - (c) 2017 James S Renwick */
using uint64_t = unsigned long long;
using uint8_t = unsigned char;


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
}
