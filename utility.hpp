/* utility - (c) 2018 James Renwick */
#pragma once
#include <type_traits>

extern "C++" {
namespace std
{
    template<typename T>
    constexpr inline typename remove_reference<T>::type&& move(T&& ref) noexcept
    {
        return static_cast<typename remove_reference<T>::type&&>(ref);
    }

    template<typename T>
    constexpr inline T&& forward(typename remove_reference<T>::type& ref) noexcept
    {
        return static_cast<T&&>(ref);
    }
    template<typename T>
    constexpr inline T&& forward(typename remove_reference<T>::type&& ref) noexcept
    {
        return static_cast<T&&>(ref);
    }

    template<typename T1, typename T2>
    inline void swap(T1& object1, T2& object2)
    {
        T1 tmp = std::move(object1);
        object1 = std::move(object2);
        object2 = std::move(tmp);
    }
}
}
