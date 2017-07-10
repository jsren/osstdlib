#pragma once

namespace std
{

    template<class T = void>
    struct less;

    template<class T>
    struct less
    {
        constexpr bool operator()(const T& lhs, const T& rhs) const {
            return lhs < rhs;
        }
    };

}
