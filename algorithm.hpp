#pragma once
#include <__iterator>

extern "C++" {
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

    namespace __detail
    {
        template<typename Iterator, typename Distance>
        constexpr void _advance(Iterator& iterator, Distance count, input_iterator_tag)
        {
            for (; count > 0; --count)
            {
                ++iterator;
            }
        }
        template<typename Iterator, typename Distance>
        constexpr void _advance(Iterator& iterator, Distance count, bidirectional_iterator_tag)
        {
            if (count > 0) _advance(iterator, count, input_iterator_tag{});
            else for (; count < 0; ++count)
            {
                --iterator;
            }
        }
        template<typename Iterator, typename Distance>
        constexpr void _advance(Iterator& iterator, Distance count, random_access_iterator_tag)
        {
            iterator += count;
        }
    }

    template<typename Iterator, typename Distance>
    constexpr void advance(Iterator& iterator, Distance count = 1)
    {
        __detail::_advance(iterator, count, iterator_traits<Iterator>::iterator_category);
    }

    template<typename InputIterator>
    constexpr InputIterator next(InputIterator iterator,
                                 typename std::iterator_traits<InputIterator>::difference_type count = 1)
    {
        std::advance(iterator, count);
        return iterator;
    }

    template<typename BidirectionalIterator>
    constexpr BidirectionalIterator prev(BidirectionalIterator iterator,
                                         typename std::iterator_traits<BidirectionalIterator>::difference_type count = 1)
    {
        std::advance(iterator, -count);
        return iterator;
    }
}
}
