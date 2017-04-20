/* meta.h - (c) James S Renwick 2015-17
   except where otherwise stated.
*/
#pragma once
#include "_std.hpp"

namespace std
{
    template<class...>
    using void_t = void;


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


    using true_type = bool_constant<true>;
    using false_type = bool_constant<false>;


    template<bool, bool> struct And : false_type { };
    template<> struct And<true, true> : true_type { };

    template<bool, bool> struct Or : true_type { };
    template<> struct Or <false, false> : false_type { };

    template<bool> struct Not : true_type { };
    template<> struct Not<true> : false_type { };


    template<bool, typename TrueType, typename FalseType>
    struct conditional { using type = TrueType; };

    template<typename TrueType, typename FalseType>
    struct conditional<false, TrueType, FalseType> { using type = FalseType; };

    template<bool Condition, typename TrueType, typename FalseType>
    using conditional_t = typename conditional<Condition, TrueType, FalseType>::type;


    template<typename T, typename U>
    struct is_same : false_type {};
    template<typename T>
    struct is_same<T, T> : true_type {};
#if defined(__cpp_variable_templates)
    template<typename T, typename U> constexpr bool is_same_v = is_same<T, U>::value;
#endif

    template<typename> struct is_void : false_type { };
    template<> struct is_void<void> : true_type { };
#if defined(__cpp_variable_templates)
    template<typename T> constexpr bool is_void_v = is_void<T>::value;
#endif

    template<typename T>
    struct is_array : false_type { };
    template<typename T, size_t N>
    struct is_array<T[N]> : true_type { };
    template<typename T>
    struct is_array<T[]> : true_type { };
#if defined(__cpp_variable_templates)
    template<typename T>
    constexpr bool is_array_v = is_array<T>::value;
#endif


    template<long long int N> struct is_even : bool_constant<N % 2 == 0> { };
#if defined(__cpp_variable_templates)
    template<long long int N> constexpr bool is_even_v = is_even<N>::value;
#endif

    template<class T>
    struct underlying_type {
        using type = __underlying_type(T);
    };
    template<class T>
    using underlying_type_t = typename underlying_type<T>::type;

    template<typename T> struct add_reference { typedef T& type; };
    template<typename T> struct add_reference<T&> { typedef T& type; };
    template<typename T> struct add_reference<T&&> { typedef T& type; };
    template<typename T> using add_reference_t = typename add_reference<T>::type;

    template<typename T> struct remove_reference { typedef T type; };
    template<typename T> struct remove_reference<T&> { typedef T type; };
    template<typename T> struct remove_reference<T&&> { typedef T type; };
    template<typename T> using remove_reference_t = typename remove_reference<T>::type;

    template<typename T> struct add_const { typedef const T type; };
    template<typename T> struct add_const<const T> { typedef const T type; };
    template<typename T> using add_const_t = typename add_const<T>::type;

    template<typename T> struct remove_const { typedef T type; };
    template<typename T> struct remove_const<const T> { typedef T type; };
    template<typename T> using remove_const_t = typename remove_const<T>::type;

    template<typename T> struct remove_volatile { typedef T type; };
    template<typename T> struct remove_volatile<volatile T> { typedef T type; };
    template<typename T> using remove_volatile_t = typename remove_volatile<T>::type;

    template<typename T> struct bare_type { typedef remove_const_t<remove_volatile_t<remove_reference_t<T>>> type; };
    template<typename T> using bare_type_t = remove_const_t<remove_volatile_t<remove_reference_t<T>>>;

    template<typename T> struct is_volatile : false_type {  };
    template<typename T> struct is_volatile<volatile T> : true_type { };
#if defined(__cpp_variable_templates)
    template<typename T> constexpr bool is_volatile_v = is_volatile<T>::value;
#endif

    template<typename T> struct is_const : false_type {  };
    template<typename T> struct is_const<const T> : true_type { };
#if defined(__cpp_variable_templates)
    template<typename T> constexpr bool is_const_v = is_const<T>::value;
#endif

    template<bool, class T = void> struct enable_if { };
    template<class T> struct enable_if<true, T> { typedef T type; };

    template<bool B, class T = void>
    using enable_if_t = typename enable_if<B, T>::type;

    template<typename T> using is_union = bool_constant<__is_union(T)>;
    template<typename T> using is_empty = bool_constant<__is_empty(T)>;
    template<typename T> using is_class = bool_constant<__is_class(T)>;
    template<typename T> using is_abstract = bool_constant<__is_abstract(T)>;
    template<typename T> using is_enum = bool_constant<__is_enum(T)>;


#if defined(__cpp_variable_templates)
    template<typename T> constexpr bool is_union_v = is_union<T>::value;
    template<typename T> constexpr bool is_empty_v = is_empty_type<T>::value;
    template<typename T> constexpr bool is_class_v = is_class<T>::value;
    template<typename T> constexpr bool is_abstract_v = is_abstract<T>::value;
    template<typename T> constexpr bool is_enum_v = is_enum<T>::value;
#endif

    template<typename T> using has_trivial_constructor = bool_constant<__has_trivial_constructor(T)>;
    template<typename T> using has_virtual_destructor = bool_constant<__has_virtual_destructor(T)>;
#if defined(__cpp_variable_templates)
    template<typename T> constexpr bool has_trivial_constructor_v = has_trivial_constructor<T>::value;
    template<typename T> constexpr bool has_virtual_destructor_v = has_virtual_destructor<T>::value;
#endif

    template<typename T> using is_pod = std::And<__is_pod(T),
        has_trivial_constructor<T>::value>;

    template<typename T> using has_trivial_copy = std::Or<__has_trivial_copy(T),
        std::And<is_pod<T>::value, std::Not<is_volatile<T>::value>::value>::value>;

#if defined(__cpp_variable_templates)
    template<typename T> constexpr bool is_pod_v = is_pod<T>::value;
    template<typename T> constexpr bool has_trivial_copy_v = has_trivial_copy<T>::value;
#endif


    template<typename Base, typename Derived>
    using is_base_of = bool_constant<__is_base_of(Base, Derived)>;
#if defined(__cpp_variable_templates)
    template<typename Base, typename Derived>
    constexpr bool is_base_of_v = is_base_of<Base, Derived>::value;
#endif

    // See http://www.boost.org/doc/libs/1_53_0/boost/type_traits/intrinsics.hpp
    // https://github.com/gcc-mirror/gcc/blob/master/libstdc%2B%2B-v3/include/std/type_traits
    // for examples for more

    template<typename T> struct is_fractional : false_type { };
    template<> struct is_fractional<float> : true_type { };
    template<> struct is_fractional<double> : true_type { };
#if defined(__cpp_variable_templates)
    template<typename T> constexpr bool is_fractional_v = is_fractional<T>::value;
#endif

    template<typename T> struct is_integer : false_type { };
    template<> struct is_integer<signed char> : true_type { };
    template<> struct is_integer<signed short> : true_type { };
    template<> struct is_integer<signed int> : true_type { };
    template<> struct is_integer<signed long int> : true_type { };
    template<> struct is_integer<signed long long int> : true_type { };
    template<> struct is_integer<unsigned char> : true_type { };
    template<> struct is_integer<unsigned short> : true_type { };
    template<> struct is_integer<unsigned int> : true_type { };
    template<> struct is_integer<unsigned long int> : true_type { };
    template<> struct is_integer<unsigned long long int> : true_type { };
#if defined(__cpp_variable_templates)
    template<typename T> constexpr bool is_integer_v = is_integer<T>::value;
#endif

    template<typename T, class = enable_if_t<is_integer<T>::value>> struct is_unsigned : false_type { };
    template<> struct is_unsigned<unsigned char> : true_type { };
    template<> struct is_unsigned<unsigned short> : true_type { };
    template<> struct is_unsigned<unsigned int> : true_type { };
    template<> struct is_unsigned<unsigned long int> : true_type { };
    template<> struct is_unsigned<unsigned long long int> : true_type { };

    template<typename T, class = enable_if_t<is_integer<T>::value>>
    struct is_signed : Not<is_unsigned<T>::value> { };

#if defined(__cpp_variable_templates)
    template<typename T> constexpr bool is_unsigned_v = is_unsigned<T>::value;
    template<typename T> constexpr bool is_signed_v = is_signed<T>::value;
#endif

    template<class T>
    std::remove_reference_t<T>&& declval();


    // TODO: Support in VS
#if !defined(_MSC_VER)

    // is_function - Taken from libstdc++
    template<typename>
    struct is_function : public false_type { };
    template<typename _Res, typename... _ArgTypes>
    struct is_function<_Res(_ArgTypes...)> : public true_type { };
    template<typename _Res, typename... _ArgTypes>
    struct is_function<_Res(_ArgTypes...) &> : public true_type { };
    template<typename _Res, typename... _ArgTypes>
    struct is_function<_Res(_ArgTypes...) && > : public true_type { };
    template<typename _Res, typename... _ArgTypes>
    struct is_function<_Res(_ArgTypes......)> : public true_type { };
    template<typename _Res, typename... _ArgTypes>
    struct is_function<_Res(_ArgTypes......) &> : public true_type { };
    template<typename _Res, typename... _ArgTypes>
    struct is_function<_Res(_ArgTypes......) && > : public true_type { };
    template<typename _Res, typename... _ArgTypes>
    struct is_function<_Res(_ArgTypes...) const> : public true_type { };
    template<typename _Res, typename... _ArgTypes>
    struct is_function<_Res(_ArgTypes...) const &> : public true_type { };
    template<typename _Res, typename... _ArgTypes>
    struct is_function<_Res(_ArgTypes...) const &&> : public true_type { };
    template<typename _Res, typename... _ArgTypes>
    struct is_function<_Res(_ArgTypes......) const> : public true_type { };
    template<typename _Res, typename... _ArgTypes>
    struct is_function<_Res(_ArgTypes......) const &> : public true_type { };
    template<typename _Res, typename... _ArgTypes>
    struct is_function<_Res(_ArgTypes......) const &&> : public true_type { };
    template<typename _Res, typename... _ArgTypes>
    struct is_function<_Res(_ArgTypes...) volatile> : public true_type { };
    template<typename _Res, typename... _ArgTypes>
    struct is_function<_Res(_ArgTypes...) volatile &> : public true_type { };
    template<typename _Res, typename... _ArgTypes>
    struct is_function<_Res(_ArgTypes...) volatile &&> : public true_type { };
    template<typename _Res, typename... _ArgTypes>
    struct is_function<_Res(_ArgTypes......) volatile> : public true_type { };
    template<typename _Res, typename... _ArgTypes>
    struct is_function<_Res(_ArgTypes......) volatile &> : public true_type { };
    template<typename _Res, typename... _ArgTypes>
    struct is_function<_Res(_ArgTypes......) volatile &&> : public true_type { };
    template<typename _Res, typename... _ArgTypes>
    struct is_function<_Res(_ArgTypes...) const volatile> : public true_type { };
    template<typename _Res, typename... _ArgTypes>
    struct is_function<_Res(_ArgTypes...) const volatile &> : public true_type { };
    template<typename _Res, typename... _ArgTypes>
    struct is_function<_Res(_ArgTypes...) const volatile &&> : public true_type { };
    template<typename _Res, typename... _ArgTypes>
    struct is_function<_Res(_ArgTypes......) const volatile> : public true_type { };
    template<typename _Res, typename... _ArgTypes>
    struct is_function<_Res(_ArgTypes......) const volatile &> : public true_type { };
    template<typename _Res, typename... _ArgTypes>
    struct is_function<_Res(_ArgTypes......) const volatile &&> : public true_type { };

#if defined(__cpp_variable_templates)
    template<typename Return>
    constexpr bool is_function_v = is_function<Return>::value;
#endif
#endif

    template<typename From, typename To,
        bool = is_void<From>::value || is_function<To>::value || is_array<To>::value>
        struct is_convertible : is_void<To> { };

    template<typename From, typename To>
    class is_convertible<From, To, false>
    {
        template<typename _To>
        static void __test_aux(_To);

        template<typename _From, typename _To,
            typename = decltype(__test_aux<_To>(std::declval<_From>()))>
            static true_type __test(int);

        template<typename, typename>
        static false_type __test(...);

    public:
        typedef decltype(__test<From, To>(0)) type;
    };

#if defined(__cpp_variable_templates)
    template<typename From, typename To>
    constexpr bool is_convertible_v = is_convertible<From, To>::type::value;
#endif

    template<typename T>
    constexpr inline typename remove_reference<T>::type&& move(T&& ref) noexcept
    {
        return static_cast<typename remove_reference<T>::type&&>(ref);
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


    template <class T, T... Is_>
    struct integer_sequence { };

    template <size_t... Is_>
    using index_sequence = integer_sequence<size_t, Is_...>;

    namespace _internal
    {
        template <class T, T Start, typename, T End>
        struct _integer_sequence;

        template <class T, T Start, T... Indexes_, T End>
        struct _integer_sequence<T, Start, integer_sequence<T, Indexes_...>, End> {
            typedef typename _integer_sequence<T, Start + 1, integer_sequence<T, Indexes_..., Start>, End>::type type;
        };
        template <class T, T End, T... Indexes_>
        struct _integer_sequence<T, End, integer_sequence<T, Indexes_...>, End> {
            typedef integer_sequence<T, Indexes_...> type;
        };

        template <class T, T End, T Start = 0>
        struct _make_integer_sequence {
            static_assert(Start <= End, "Start index cannot be greater than end index.");
            typedef typename _integer_sequence<T, Start, integer_sequence<T>, End>::type type;
        };
    }

    template <class T, T End, T Start = 0>
    using make_integer_sequence = typename _internal::_make_integer_sequence<T, End, Start>::type;

    template <size_t End, size_t Start = 0>
    using make_index_sequence = typename _internal::_make_integer_sequence<size_t, End, Start>::type;
}

namespace __std
{
    template<typename Src, typename T>
    struct mirror_const_volatile {
    private:
        using const_type = std::conditional_t<std::is_const<Src>::value, const T, T>;
    public:
        using type = std::conditional_t<std::is_volatile<Src>::value, volatile const_type, const_type>;
    };

    template<typename T>
    struct make_unsigned_raw { };
    template<> struct make_unsigned_raw<char> { using type = unsigned char; };
    template<> struct make_unsigned_raw<signed char> { using type = unsigned char; };
    template<> struct make_unsigned_raw<unsigned char> { using type = unsigned char; };
    template<> struct make_unsigned_raw<short> { using type = unsigned short; };
    template<> struct make_unsigned_raw<unsigned short> { using type = unsigned short; };
    template<> struct make_unsigned_raw<int> { using type = unsigned int; };
    template<> struct make_unsigned_raw<unsigned int> { using type = unsigned int; };
    template<> struct make_unsigned_raw<long> { using type = unsigned long; };
    template<> struct make_unsigned_raw<unsigned long> { using type = unsigned long; };
    template<> struct make_unsigned_raw<long long> { using type = unsigned long long; };
    template<> struct make_unsigned_raw<unsigned long long> { using type = unsigned long long; };

    template<typename T>
    struct make_signed_raw { };
    template<> struct make_signed_raw<char> { using type = signed char; };
    template<> struct make_signed_raw<signed char> { using type = signed char; };
    template<> struct make_signed_raw<unsigned char> { using type = signed char; };
    template<> struct make_signed_raw<short> { using type = signed short; };
    template<> struct make_signed_raw<unsigned short> { using type = signed short; };
    template<> struct make_signed_raw<int> { using type = signed int; };
    template<> struct make_signed_raw<unsigned int> { using type = signed int; };
    template<> struct make_signed_raw<long> { using type = signed long; };
    template<> struct make_signed_raw<unsigned long> { using type = signed long; };
    template<> struct make_signed_raw<long long> { using type = signed long long; };
    template<> struct make_signed_raw<unsigned long long> { using type = signed long long; };
}

namespace std
{
    template<typename T>
    struct make_unsigned {
        using type = typename __std::mirror_const_volatile<T, typename __std::make_unsigned_raw<T>::type>::type;
    };
    template<typename T>
    using make_unsigned_t = typename make_unsigned<T>::type;

    template<typename T>
    struct make_signed {
        using type = typename __std::mirror_const_volatile<T, typename __std::make_signed_raw<T>::type>::type;
    };
    template<typename T>
    using make_signed_t = typename make_signed<T>::type;
}


namespace __std
{
    template<typename T>
    struct first_tparam {
        using type = void;
    };
    template<template<typename, typename...> class Template, typename T, typename... _>
    struct first_tparam<Template<T, _...>> {
        using type = T;
    };
    template<typename T>
    using first_tparam_t = typename first_tparam<T>::type;


    template<typename T, template<class> class Accessor, typename Default, class = void>
    struct scope_type_or_default {
        using type = Default;
    };
    template<typename T, template<class> class Accessor, typename _>
    struct scope_type_or_default<T, Accessor, _, std::void_t<Accessor<T>>> {
        using type = Accessor<T>;
    };
    template<typename T, template<class> class Accessor, typename Default>
    using scope_type_or_default_t = typename scope_type_or_default<T, Accessor, Default>::type;


    template<typename T, typename Y>
    struct rebind {
        using type = void;
    };
    template<template<class, class...> class Template, typename T, typename _, typename... Args>
    struct rebind<Template<_, Args...>, T> {
        using type = Template<T, Args...>;
    };
    template<typename Template, typename T>
    using rebind_t = typename rebind<Template, T>::type;
}
