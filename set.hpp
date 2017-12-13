#pragma once
#include <algorithm>
#include <type_traits>
#include <allocator>

namespace std
{
    template<typename Key, typename Comparer = less<Key>,
        typename Allocator = allocator<Key>>
    class set
    {
        typedef Key key_type;
        typedef Key value_type;

        using size_type = size_t;
        using difference_type = ptrdiff_t;
        using key_compare = Comparer;
        using value_compare = Comparer;
        using allocator_type = Allocator;
        using reference = value_type&;
        using const_reference = const value_type&;
        using pointer = Allocator::value_type*;
        using const_pointer = const Allocator::value_type*;
    };

}
