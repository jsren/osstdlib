#pragma once
#include <type_traits>
#include <cstddef>
#include <__iterator>
#include <.ostreambuf-iterator>

extern "C++" {
namespace std
{
    template<typename Iterator>
    class move_iterator
    {
        using postfix = int;

    public:
        using value_type = typename iterator_traits<Iterator>::value_type;
        using difference_type = typename iterator_traits<Iterator>::difference_type;
        using pointer = Iterator;
        using iterator_category = typename iterator_traits<Iterator>::iterator_category;
        using iterator_type = Iterator;

    private:
        using i_reference = typename iterator_traits<Iterator>::reference;
    public:
        using reference = conditional_t<is_reference<i_reference>::value,
            i_reference, add_rvalue_reference_t<i_reference>>;

    private:
        Iterator current{};

    public:
        constexpr move_iterator() { }

        constexpr explicit move_iterator(Iterator iterator)
            : current(iterator) { }

        template<typename U>
        constexpr move_iterator(const move_iterator<U>& other)
            : current(other) { }

        template<typename U>
        constexpr move_iterator& operator=(const move_iterator<U>& other) {
            current = other.base();
        }

        constexpr Iterator base() const {
            return current;
        }
        constexpr reference operator*() const {
            return static_cast<reference>(*current);
        }
        constexpr pointer operator->() const {
            return current;
        }
        constexpr auto operator[](difference_type offset) const {
            return std::move(current[offset]);
        }
        constexpr move_iterator& operator++() {
            current++; return *this;
        }
        constexpr move_iterator& operator--() {
            current--; return *this;
        }
        constexpr move_iterator operator++(postfix) {
            move_iterator copy = *this; this->operator++(); return copy;
        }
        constexpr move_iterator operator--(postfix) {
            move_iterator copy = *this; this->operator--(); return copy;
        }
        constexpr move_iterator operator+(difference_type offset) const {
            return move_iterator(current + offset);
        }
        constexpr move_iterator operator-(difference_type offset) const {
            return move_iterator(current - offset);
        }
        constexpr move_iterator& operator+=(difference_type offset) {
            current += offset; return *this;
        }
        constexpr move_iterator& operator-=(difference_type offset) {
            current -= offset; return *this;
        }
    };

    template<typename Iterator1, typename Iterator2>
    inline constexpr bool operator==(const move_iterator<Iterator1>& lhs,
        const move_iterator<Iterator2>& rhs)
    {
        return lhs.base() == rhs.base();
    }
    template<typename Iterator1, typename Iterator2>
    inline constexpr bool operator!=(const move_iterator<Iterator1>& lhs,
        const move_iterator<Iterator2>& rhs)
    {
        return lhs.base() != rhs.base();
    }
    template<typename Iterator1, typename Iterator2>
    inline constexpr bool operator>(const move_iterator<Iterator1>& lhs,
        const move_iterator<Iterator2>& rhs)
    {
        return lhs.base() > rhs.base();
    }
    template<typename Iterator1, typename Iterator2>
    inline constexpr bool operator<(const move_iterator<Iterator1>& lhs,
        const move_iterator<Iterator2>& rhs)
    {
        return lhs.base() < rhs.base();
    }
    template<typename Iterator1, typename Iterator2>
    inline constexpr bool operator>=(const move_iterator<Iterator1>& lhs,
        const move_iterator<Iterator2>& rhs)
    {
        return lhs.base() >= rhs.base();
    }
    template<typename Iterator1, typename Iterator2>
    inline constexpr bool operator<=(const move_iterator<Iterator1>& lhs,
        const move_iterator<Iterator2>& rhs)
    {
        return lhs.base() <= rhs.base();
    }

    template<typename Iterator>
    inline constexpr move_iterator<Iterator> operator+(
        typename move_iterator<Iterator>::difference_type offset,
        const move_iterator<Iterator>& lhs)
    {
        return lhs + offset;
    }

    template<typename Iterator1, typename Iterator2>
    inline constexpr auto operator-(const move_iterator<Iterator1>& lhs,
        const move_iterator<Iterator2>& rhs)
    {
        return lhs.base() - rhs.base();
    }

    template<typename Iterator>
    inline constexpr move_iterator<Iterator> make_move_iterator(Iterator iterator)
    {
        return move_iterator<Iterator>(iterator);
    }

    // ---------------------------------------------------------------------------
    // TODO: Other iterator adapter types
}
}