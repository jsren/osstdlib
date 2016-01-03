#pragma once

namespace std
{
    template <class T, T v>
    struct integral_constant
    {
        using value_type = T;
        using type = integral_constant<T, v>;

        static constexpr T value = v;
        inline constexpr operator T() const noexcept { return v; }
        inline constexpr T operator()() const noexcept { return v; }
    };
    template<bool v> using bool_constant = integral_constant<bool, v>;

    using true_type  = bool_constant<true>;
    using false_type = bool_constant<false>;


    template<bool, bool> struct and : false_type { };
    template<> struct and<true, true> : true_type { };

    template<bool, bool> struct or : true_type { };
    template<> struct or <false, false> : false_type { };

    template<bool> struct not : true_type { };
    template<> struct not<true> : false_type { };

    template<typename T> struct _remove_reference { typedef T type; };
    template<typename T> struct _remove_reference<T&> { typedef T type; };
    template<typename T> struct _remove_reference<T&&> { typedef T type; };
    template<typename T> using remove_reference = typename _remove_reference<T>::type;

    template<typename T> struct _is_volatile : false_type {  };
    template<typename T> struct _is_volatile<volatile T> : true_type { };
    template<typename T> using is_volatile = _is_volatile<T>;

    template<typename T> struct _is_const : false_type {  };
    template<typename T> struct _is_const<const T> : true_type { };
    template<typename T> using is_const = _is_const<T>;

    template<bool, class T = void> struct enable_if { };
    template<class T> struct enable_if<true, T> { typedef T type; };

    template<typename T> using is_union    = bool_constant<__is_union(T)>;
    template<typename T> using is_empty    = bool_constant<__is_empty(T)>;
    template<typename T> using is_class    = bool_constant<__is_class(T)>;
    template<typename T> using is_abstract = bool_constant<__is_abstract(T)>;

    template<typename T> using has_trivial_constructor = bool_constant<__has_trivial_constructor(T)>;
    template<typename T> using has_virtual_destructor  = bool_constant<__has_virtual_destructor(T)>;

    template<typename T> using is_pod = std::and<
        bool_constant<__is_pod(T)>::value,
        has_trivial_constructor<T>::value>;

    template<typename T> using has_trivial_copy = std::or<
        bool_constant<__has_trivial_copy(T)>::value,
        std::and<is_pod<T>::value, std::not<is_volatile<T>::value>::value>::value>;

    // See http://www.boost.org/doc/libs/1_53_0/boost/type_traits/intrinsics.hpp
    // for examples for more

}