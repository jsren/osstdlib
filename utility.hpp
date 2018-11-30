/* utility.hpp - (c) 2018 James Renwick */
#pragma once
#include <cstddef>

namespace std
{
    struct in_place_t {
        explicit in_place_t() = default;
    };

    template<typename T>
    struct in_place_type_t {
        explicit in_place_type_t() = default;
    };

    template<std::size_t I>
    struct in_place_index_t {
        explicit in_place_index_t() = default;
    };

#if __cpp_inline_variables
#define __STD_INLINE inline
#else
#define __STD_INLINE static
#endif

    __STD_INLINE constexpr in_place_t in_place{};
    template<typename T>
    __STD_INLINE constexpr in_place_type_t<T> in_place_type{};
    template<std::size_t I>
    __STD_INLINE constexpr in_place_index_t<I> in_place_index{};
}
