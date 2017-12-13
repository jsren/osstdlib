#pragma once

#include <__iterator>
#include <functional>
#include <stdexcept>
#include <type_traits>

namespace std
{
    template<typename T, size_t N>
    struct array
    {
        using value_type = T;
        using size_type = size_t;
        using difference_type = ptrdiff_t;
        using reference = value_type&;
        using const_reference = const value_type&;
        using pointer = value_type*;
        using const_pointer = const value_type*;
        using iterator = __detail::pointer_iterator<T>;
        using const_iterator = __detail::pointer_iterator<const T>;
        using reverse_iterator = std::reverse_iterator<iterator>;
        using const_reverse_iterator = std::reverse_iterator<const_iterator>;

        T _data[N];

        constexpr reference at(size_type index) {
            if (index >= N) __abi::__throw_exception(out_of_range("index"));
            else return _data[index];
        }
        constexpr const_reference at(size_type index) const {
            if (index >= N) __abi::__throw_exception(out_of_range("index"));
            else return _data[index];
        }

        constexpr reference operator[](size_type index) {
            return _data[index];
        }
        constexpr const_reference operator[](size_type index) const {
            return _data[index];
        }

        constexpr reference front() {
            return this->operator[](0);
        }
        constexpr reference front() const {
            return this->operator[](0);
        }

        constexpr reference back() {
            return this->operator[](N-1);
        }
        constexpr reference back() const {
            return this->operator[](N-1);
        }

        constexpr T* data() noexcept {
            return _data;
        }
        constexpr const T* data() const noexcept {
            return _data;
        }

        constexpr iterator begin() noexcept {
            return iterator(_data);
        }
        constexpr const_iterator begin() const noexcept {
            return const_iterator(_data);
        }
        constexpr const_iterator cbegin() const noexcept {
            return const_iterator(_data);
        }

        constexpr reverse_iterator rbegin() noexcept {
            return reverse_iterator(_data);
        }
        constexpr const_reverse_iterator rbegin() const noexcept {
            return reverse_iterator(_data);
        }
        constexpr const_reverse_iterator crbegin() const noexcept {
            return reverse_iterator(_data);
        }

        constexpr iterator end() noexcept {
            return iterator(_data + N);
        }
        constexpr const_iterator end() const noexcept {
            return const_iterator(_data + N);
        }
        constexpr const_iterator cend() const noexcept {
            return const_iterator(_data + N);
        }

        constexpr reverse_iterator rend() noexcept {
            return reverse_iterator(_data + N);
        }
        constexpr const_reverse_iterator rend() const noexcept {
            return const_reverse_iterator(_data + N);
        }
        constexpr const_reverse_iterator crend() const noexcept {
            return const_reverse_iterator(_data + N);
        }

        constexpr bool empty() const noexcept {
            return N == 0;
        }
        constexpr size_type size() const noexcept {
            return static_cast<size_type>(N);
        }
        constexpr size_type max_size() const noexcept {
            return static_cast<size_type>(N);
        }

        void fill(const T& value) {
            for (size_t i = 0; i < N; i++) _data[i] = value;
        }

        void swap(array& other) noexcept(
            conditional_t<N == 0, true_type, is_nothrow_swappable<T>>::value)
        {
            for (size_t i = 0; i < N; i++) std::swap(_data[i], other[i]);
        }

    };

#define __STD_ARRAY_OPERATOR(op) \
    template<typename T, size_t N> \
    inline bool operator op (const array<T,N>& lhs, const array<T,N>& rhs) { \
        for (size_t i = 0; i < N; i++) { \
            if (!(lhs[i] op rhs[i])) return false; \
        } \
        return true; \
    }

    __STD_ARRAY_OPERATOR(==)
    __STD_ARRAY_OPERATOR(!=)
    __STD_ARRAY_OPERATOR(<)
    __STD_ARRAY_OPERATOR(>)
    __STD_ARRAY_OPERATOR(<=)
    __STD_ARRAY_OPERATOR(>=)


    template<size_t Index, typename T, size_t N>
    inline constexpr T& get(array<T,N>& a) noexcept {
        static_assert(Index < N, ""); return a[Index];
    }
    template<size_t Index, typename T, size_t N>
    inline constexpr const T& get(const array<T,N>& a) noexcept {
        static_assert(Index < N, ""); return a[Index];
    }
    template<size_t Index, typename T, size_t N>
    inline constexpr T&& get(array<T,N>&& a) noexcept {
        static_assert(Index < N, ""); return std::move(a[Index]);
    }
    template<size_t Index, typename T, size_t N>
    inline constexpr const T&& get(const array<T,N>&& a) noexcept {
        static_assert(Index < N, ""); return std::move(a[Index]);
    }

    template<typename T, size_t N, enable_if_t<N == 0 || is_swappable<T>::value>>
    void swap(array<T,N>& lhs, array<T,N>& rhs) noexcept(noexcept(lhs.swap(rhs))) {
        lhs.swap(rhs);
    }

    //template<typename T, size_t N>
    //class tuple_size<array<T, N>> : public integral_constant<size_t, N> { };

    //template<size_t Index, typename T, size_t N>
    //struct tuple_element<T, array<T, N>> { using type = T; }


#ifdef __cpp_deduction_guides
    template<typename T, typename ...U>
    array(T, U...) -> array<T, 1 + sizeof...(U)>;
#endif

    namespace __detail
    {
        template<typename T> struct is_reference_wrapper : false_type { };
        template<typename T> struct is_reference_wrapper<reference_wrapper<T>> : true_type { };

        template<typename T, typename... Ts>
        inline constexpr bool reference_wrapper_in() noexcept {
            return is_reference_wrapper<T>::value;
        }
    }

    template<typename Y = void, typename... Ts,
        typename T = conditional_t<is_same<Y, void>::value, common_type_t<Ts...>, Y>>
    inline constexpr array<T, sizeof...(Ts)> make_array(Ts&&... elements)
    {
        using namespace __detail;
        static_assert(is_same<Y, void>::value || reference_wrapper_in<Ts...>(), "");

        return { {std::forward<Ts>(elements)...} };
    }

    namespace __detail
    {
        template<typename T, size_t N, size_t... I>
        constexpr array<remove_cv_t<T>, N> to_array(T (&a)[N], index_sequence<I...>) {
            return { {a[I]...} };
        }
    }

    template<typename T, size_t N>
    constexpr array<remove_cv_t<T>, N> to_array(T (&a)[N]) {
        return __detail::to_array(a, make_index_sequence<N>());
    }
}
