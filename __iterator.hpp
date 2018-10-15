#pragma once
#include <cstddef>
#include <type_traits>

namespace std
{
    struct input_iterator_tag { };
    struct output_iterator_tag { };
    struct forward_iterator_tag : input_iterator_tag { };
    struct bidirectional_iterator_tag : forward_iterator_tag { };
    struct random_access_iterator_tag : bidirectional_iterator_tag { };


    namespace __detail
    {
        static void implicit_bool(bool b);

        template<typename T>
        static auto _is_input_iterator(T&& t) -> decltype(
            implicit_bool(t != t), *t, ++t, (void)t++, std::true_type{});

        static std::false_type _is_input_iterator(...);

        template<typename T>
        constexpr bool is_input_iterator() noexcept {
            return decltype(_is_input_iterator(std::declval<T>()))::value;
        }
    }


    template<typename Iterator>
    struct iterator_traits;

    template<typename T>
    struct iterator_traits<T*>
    {
        using difference_type = ptrdiff_t;
        using value_type = T;
        using pointer = T*;
        using reference = T&;
        using iterator_category = random_access_iterator_tag;
    };

    template<typename T>
    struct iterator_traits<const T*>
    {
        using difference_type = ptrdiff_t;
        using value_type = T;
        using pointer = const T*;
        using reference = const T&;
        using iterator_category = random_access_iterator_tag;
    };


    namespace __detail
    {
        template<typename T>
        struct pointer_iterator : iterator_traits<T*>
        {
        private:
            using postfix = int;
            using typename iterator_traits<T*>::difference_type;
            using typename iterator_traits<T*>::reference;
            using pointer_type = typename iterator_traits<T*>::pointer;

        private:
            T* pointer{};

        public:
            constexpr pointer_iterator() noexcept = default;

            constexpr pointer_iterator(T* pointer) noexcept : pointer(pointer) { }

            constexpr pointer_iterator(const pointer_iterator& other) noexcept
                : pointer(other.pointer) { }

            constexpr pointer_iterator& operator=(const pointer_iterator& other) noexcept {
                pointer = other.pointer; return *this;
            }

        public:
            constexpr pointer_iterator& operator++() noexcept {
                pointer++; return *this;
            }
            constexpr pointer_iterator operator++(postfix) noexcept {
                pointer_iterator copy = *this; pointer++; return copy;
            }
            constexpr pointer_iterator& operator--() noexcept {
                pointer--; return *this;
            }
            constexpr pointer_iterator operator--(postfix) noexcept {
                pointer_iterator copy = *this; pointer--; return copy;
            }
            constexpr reference operator*() const noexcept {
                return *pointer;
            }
            constexpr pointer_type operator->() const noexcept {
                return pointer;
            }
            constexpr reference operator[](difference_type offset) const noexcept {
                return *(pointer + offset);
            }
            constexpr bool operator!=(const pointer_iterator& other) const noexcept {
                return pointer != other.pointer;
            }
            constexpr bool operator==(const pointer_iterator& other) const noexcept {
                return pointer == other.pointer;
            }
            constexpr pointer_iterator& operator+=(difference_type offset) noexcept {
                pointer += offset; return *this;
            }
            constexpr pointer_iterator& operator-=(difference_type offset) noexcept {
                pointer -= offset; return *this;
            }
            constexpr pointer_iterator operator+(difference_type offset) const noexcept {
                return pointer_iterator(pointer + offset);
            }
            constexpr pointer_iterator operator-(difference_type offset) const noexcept {
                return pointer_iterator(pointer - offset);
            }

        };


        template<typename T, typename Y>
        inline constexpr bool operator==(const pointer_iterator<T>& lhs,
            const pointer_iterator<Y>& rhs) noexcept
        {
            return lhs.base() == rhs.base();
        }
        template<typename T, typename Y>
        inline constexpr bool operator!=(const pointer_iterator<T>& lhs,
            const pointer_iterator<Y>& rhs) noexcept
        {
            return lhs.base() != rhs.base();
        }
        template<typename T, typename Y>
        inline constexpr bool operator>(const pointer_iterator<T>& lhs,
            const pointer_iterator<Y>& rhs) noexcept
        {
            return lhs.base() > rhs.base();
        }
        template<typename T, typename Y>
        inline constexpr bool operator<(const pointer_iterator<T>& lhs,
            const pointer_iterator<Y>& rhs) noexcept
        {
            return lhs.base() < rhs.base();
        }
        template<typename T, typename Y>
        inline constexpr bool operator>=(const pointer_iterator<T>& lhs,
            const pointer_iterator<Y>& rhs) noexcept
        {
            return lhs.base() >= rhs.base();
        }
        template<typename T, typename Y>
        inline constexpr bool operator<=(const pointer_iterator<T>& lhs,
            const pointer_iterator<Y>& rhs) noexcept
        {
            return lhs.base() <= rhs.base();
        }

        template<typename T>
        inline constexpr pointer_iterator<T> operator+(
            typename pointer_iterator<T>::difference_type offset,
            const pointer_iterator<T>& lhs) noexcept
        {
            return lhs + offset;
        }
        template<typename T, typename Y>
        inline constexpr typename pointer_iterator<T>::difference_type operator-(
            const pointer_iterator<T>& lhs, const pointer_iterator<Y>& rhs) noexcept
        {
            return &*lhs - &*rhs;
        }

    }

    template<typename T>
    struct iterator_traits<__detail::pointer_iterator<T>> : iterator_traits<T*> { };


    template<typename Iterator>
    class reverse_iterator
    {
        using postfix = int;

    public:
        using value_type = typename iterator_traits<Iterator>::value_type;
        using difference_type = typename iterator_traits<Iterator>::difference_type;
        using pointer = typename iterator_traits<Iterator>::pointer;
        using reference = typename iterator_traits<Iterator>::reference;
        using iterator_category = typename iterator_traits<Iterator>::iterator_category;
        using iterator_type = Iterator;

    protected:
        Iterator current{};

    public:
        constexpr reverse_iterator() { }

        constexpr explicit reverse_iterator(Iterator iterator)
            : current(iterator) { }

        template<typename U>
        constexpr reverse_iterator(const reverse_iterator<U>& other)
            : current(other) { }

        template<typename U>
        constexpr reverse_iterator& operator=(const reverse_iterator<U>& other) {
            current = other.base();
        }

        constexpr Iterator base() const {
            return current;
        }
        constexpr reference operator*() const {
            return *(current-1);
        }
        constexpr pointer operator->() const {
            return &(*(current-1));
        }
        constexpr auto operator[](difference_type offset) const {
            return current[-offset-1];
        }
        constexpr reverse_iterator& operator++() {
            current--; return *this;
        }
        constexpr reverse_iterator& operator--() {
            current++; return *this;
        }
        constexpr reverse_iterator operator++(postfix) {
            reverse_iterator copy = *this; this->operator++(); return copy;
        }
        constexpr reverse_iterator operator--(postfix) {
            reverse_iterator copy = *this; this->operator--(); return copy;
        }
        constexpr reverse_iterator operator+(difference_type offset) const {
            return reverse_iterator(current - offset);
        }
        constexpr reverse_iterator operator-(difference_type offset) const {
            return reverse_iterator(current + offset);
        }
        constexpr reverse_iterator& operator+=(difference_type offset) {
            current -= offset; return *this;
        }
        constexpr reverse_iterator& operator-=(difference_type offset) {
            current += offset; return *this;
        }
    };

    template<typename Iterator1, typename Iterator2>
    inline constexpr bool operator==(const reverse_iterator<Iterator1>& lhs,
        const reverse_iterator<Iterator2>& rhs)
    {
        return lhs.base() == rhs.base();
    }
    template<typename Iterator1, typename Iterator2>
    inline constexpr bool operator!=(const reverse_iterator<Iterator1>& lhs,
        const reverse_iterator<Iterator2>& rhs)
    {
        return lhs.base() != rhs.base();
    }
    template<typename Iterator1, typename Iterator2>
    inline constexpr bool operator>(const reverse_iterator<Iterator1>& lhs,
        const reverse_iterator<Iterator2>& rhs)
    {
        return lhs.base() < rhs.base();
    }
    template<typename Iterator1, typename Iterator2>
    inline constexpr bool operator<(const reverse_iterator<Iterator1>& lhs,
        const reverse_iterator<Iterator2>& rhs)
    {
        return lhs.base() > rhs.base();
    }
    template<typename Iterator1, typename Iterator2>
    inline constexpr bool operator>=(const reverse_iterator<Iterator1>& lhs,
        const reverse_iterator<Iterator2>& rhs)
    {
        return lhs.base() <= rhs.base();
    }
    template<typename Iterator1, typename Iterator2>
    inline constexpr bool operator<=(const reverse_iterator<Iterator1>& lhs,
        const reverse_iterator<Iterator2>& rhs)
    {
        return lhs.base() >= rhs.base();
    }

    template<typename Iterator>
    inline constexpr reverse_iterator<Iterator> operator+(
        typename reverse_iterator<Iterator>::difference_type offset,
        const reverse_iterator<Iterator>& lhs)
    {
        return lhs + offset;
    }

    template<typename Iterator1, typename Iterator2>
    inline constexpr auto operator-(const reverse_iterator<Iterator1>& lhs,
        const reverse_iterator<Iterator2>& rhs)
    {
        return rhs.base() - lhs.base();
    }

    template<typename Iterator>
    inline constexpr reverse_iterator<Iterator> make_reverse_iterator(Iterator iterator)
    {
        return reverse_iterator<Iterator>(iterator);
    }

    namespace __detail
    {
        template<typename T>
        class has_subtract
        {
            template<typename Y, class=decltype(declval<Y>() - declval<Y>())>
            static true_type test(int);
            template<typename, class>
            static false_type test(...);
        public:
            using type = decltype(test<T>(0));
            static constexpr const bool value = type::value;
        };

        template<typename Iterator>
        typename iterator_traits<Iterator>::distance distance(
            Iterator first, Iterator last, true_type _)
        {
            (void)_;
            return last - first;
        }

        template<typename Iterator>
        typename iterator_traits<Iterator>::distance distance(
            Iterator first, Iterator last, false_type _)
        {
            (void)_;
            size_t count = 0;
            Iterator iter = first;
            for (; iter != last; count++, iter++) { }
            return count;
        }
    }

    template<typename Iterator>
    typename iterator_traits<Iterator>::distance distance(
        Iterator first, Iterator last)
    {
        return __detail::distance(first, last,
            __detail::has_subtract<Iterator>::type);
    }
}
