/* type_traits.hpp - (c) James S Renwick 2015-17
   except where otherwise stated.
*/
#pragma once
#include <cstddef>

extern "C++" {
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
    template<typename T, typename U>
	constexpr const bool is_same_v = is_same<T, U>::value;
#endif

    template<typename> struct is_void : false_type { };
    template<> struct is_void<void> : true_type { };
#if defined(__cpp_variable_templates)
    template<typename T> constexpr const bool is_void_v = is_void<T>::value;
#endif

    template<typename T>
    struct is_array : false_type { };
    template<typename T, size_t N>
    struct is_array<T[N]> : true_type { };
    template<typename T>
    struct is_array<T[]> : true_type { };
#if defined(__cpp_variable_templates)
    template<typename T>
    constexpr const bool is_array_v = is_array<T>::value;
#endif

    template<typename T>
    struct is_reference : false_type { };
    template<typename T>
    struct is_reference<T&> : true_type { };
    template<typename T>
    struct is_reference<T&&> : true_type { };
#if defined(__cpp_variable_templates)
    template<typename T>
    constexpr const bool is_reference_v = is_reference<T>::value;
#endif


    template<long long int N> struct is_even : bool_constant<N % 2 == 0> { };
#if defined(__cpp_variable_templates)
    template<long long int N> constexpr const bool is_even_v = is_even<N>::value;
#endif

    template<class T>
    struct underlying_type {
        using type = __underlying_type(T);
    };
    template<class T>
    using underlying_type_t = typename underlying_type<T>::type;

    template<typename T> struct add_lvalue_reference { typedef T& type; };
    template<typename T> struct add_lvalue_reference<T&> { typedef T& type; };
    template<typename T> struct add_lvalue_reference<T&&> { typedef T& type; };
    template<typename T> using add_lvalue_reference_t = typename add_lvalue_reference<T>::type;


    template<typename T> struct add_rvalue_reference { typedef T&& type; };
    template<typename T> struct add_rvalue_reference<T&> { typedef T& type; };
    template<typename T> struct add_rvalue_reference<T&&> { typedef T&& type; };
    template<typename T> using add_rvalue_reference_t = typename add_rvalue_reference<T>::type;

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


	template<typename T> struct add_volatile { typedef volatile T type; };
	template<typename T> struct add_volatile<volatile T> { typedef volatile T type; };
	template<typename T> using add_volatile_t = typename add_volatile<T>::type;

    template<typename T> struct remove_volatile { typedef T type; };
    template<typename T> struct remove_volatile<volatile T> { typedef T type; };
    template<typename T> using remove_volatile_t = typename remove_volatile<T>::type;


	template<typename T> struct add_cv { typedef const volatile T type; };
	template<typename T> struct add_cv<const volatile T> { typedef const volatile T type; };
	template<typename T> using add_cv_t = typename add_cv<T>::type;

    template<typename T> using remove_cv = remove_volatile<remove_const_t<T>>;
    template<typename T> using remove_cv_t = typename remove_cv<T>::type;

    template<typename T> struct bare_type { typedef remove_const_t<remove_volatile_t<remove_reference_t<T>>> type; };
    template<typename T> using bare_type_t = remove_const_t<remove_volatile_t<remove_reference_t<T>>>;


	template<typename T>
	struct remove_all_extents { using type = T; };

	template<typename T>
	using remove_all_extents_t = typename remove_all_extents<T>::type;

	template<typename T>
	struct remove_all_extents<T[]> { using type = remove_all_extents_t<T>; };

	template<typename T, size_t N>
	struct remove_all_extents<T[N]> { using type = remove_all_extents_t<T>; };

    template<typename...Ts>
    struct common_type;

    template<>
    struct common_type<> { };





    template<class T>
    std::add_rvalue_reference_t<T> declval() noexcept;


    template<typename T> struct is_volatile : false_type {  };
    template<typename T> struct is_volatile<volatile T> : true_type { };
#if defined(__cpp_variable_templates)
    template<typename T> constexpr const bool is_volatile_v = is_volatile<T>::value;
#endif

    template<typename T> struct is_const : false_type {  };
    template<typename T> struct is_const<const T> : true_type { };
#if defined(__cpp_variable_templates)
    template<typename T> constexpr const bool is_const_v = is_const<T>::value;
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
    template<typename T> using is_standard_layout = bool_constant<__is_standard_layout(T)>;


#if defined(__cpp_variable_templates)
    template<typename T> constexpr const bool is_union_v = is_union<T>::value;
    //template<typename T> constexpr const bool is_empty_v = is_empty_type<T>::value;
    template<typename T> constexpr const bool is_class_v = is_class<T>::value;
    template<typename T> constexpr const bool is_abstract_v = is_abstract<T>::value;
    template<typename T> constexpr const bool is_enum_v = is_enum<T>::value;
    template<typename T> constexpr const bool is_standard_layout_v = is_standard_layout<T>::value;
#endif

    template<typename T> using has_trivial_constructor = bool_constant<__has_trivial_constructor(T)>;
    template<typename T> using has_virtual_destructor = bool_constant<__has_virtual_destructor(T)>;

	// GCC doesn't support the __is_constructible intrinsic until 8+
#if !defined(__GNUC__) || __GNUC__ > 7
	template<typename T, typename ...Args>
	using is_constructible = bool_constant<__is_constructible(T, Args...)>;
#else
	namespace __detail
	{
		// N.B - THIS IS NOT EXHAUSTIVE.
		template<typename T, typename ...Args>
		struct is_constructable
		{
			template<typename Y, class=decltype(::new T{declval<Args>()...})>
			static true_type test(int);
			template<typename>
			static false_type test(...);
		public:
			static constexpr const bool value = decltype(test(0))::value;
		};
	}

#pragma message "is_constructable is not exhaustive"
	template<typename T, typename ...Args>
	struct is_constructible : bool_constant<__detail::is_constructable<T, Args...>::value> { };
#endif

#if defined(__cpp_variable_templates)
	template<typename T, typename ...Args> constexpr const bool is_constructible_v = is_constructible<T, Args...>::value;
    template<typename T> constexpr const bool has_trivial_constructor_v = has_trivial_constructor<T>::value;
    template<typename T> constexpr const bool has_virtual_destructor_v = has_virtual_destructor<T>::value;
#endif

	template<typename T>
	using is_pod = bool_constant<__is_pod(T) && has_trivial_constructor<T>::value>;

	template<typename T>
	using has_trivial_copy = bool_constant<__has_trivial_copy(T) ||
        (is_pod<T>::value && !is_volatile<T>::value)>;

#if defined(__cpp_variable_templates)
    template<typename T> constexpr const bool is_pod_v = is_pod<T>::value;
    template<typename T> constexpr const bool has_trivial_copy_v = has_trivial_copy<T>::value;
#endif

	namespace __detail
	{
		template<typename T>
		struct is_sized_array : false_type { };

		template<typename T, size_t N>
		struct is_sized_array<T[N]> : true_type { };

		template<typename T, bool = is_array<T>::value>
		struct is_nothrow_default_constructible
		{
			static constexpr const bool value = is_sized_array<T>::value &&
				is_nothrow_default_constructible<remove_all_extents_t<T>>::value;
		};

		template<typename T>
		struct is_nothrow_default_constructible<T, false> :
			bool_constant<noexcept(T{})> { };

		template<typename T, typename ...Args>
		struct is_nothrow_constructible {
			static constexpr const bool value = noexcept(T(declval<Args>()...));
		};
		template<typename T, typename Arg>
		struct is_nothrow_constructible<T, Arg> {
			static constexpr const bool value = noexcept(static_cast<T>(declval<Arg>()));
		};
		template<typename T>
		struct is_nothrow_constructible<T> : is_nothrow_default_constructible<T> { };

		template<typename T, bool = is_array<T>::value>
		struct is_default_constructible
		{
		private:
			template<typename Y, class=decltype(Y())>
			static true_type test(Y);
			template<typename>
			static false_type test(...);
		public:
			static constexpr const bool value = !is_void<T>::value && decltype(test(declval<T>()))::value;
		};

		template<typename T>
		struct is_default_constructible<T, true>
		{
			static constexpr const bool value = is_sized_array<T>::value &&
				is_default_constructible<remove_all_extents_t<T>>::value;
		};
	}

	template<typename T, typename ...Args>
	struct is_nothrow_constructible :
		bool_constant<is_constructible<T, Args...>::value &&
			__detail::is_nothrow_constructible<T, Args...>::value> { };

	template<typename T>
	struct is_default_constructible :
		bool_constant<__detail::is_default_constructible<T>::value> { };

	template<typename T>
	struct is_nothrow_default_constructible :
		bool_constant<is_default_constructible<T>::value &&
			__detail::is_nothrow_default_constructible<T>::value> { };

	template<typename T, typename ...Args>
	struct is_trivially_constructible :
		bool_constant<__is_trivially_constructible(T, Args...)> { };

	template<typename T, typename ...Args>
	struct is_trivially_default_constructible : is_trivially_constructible<T, Args...> { };

    template<typename T> struct is_member_pointer : false_type { };
    template<typename T, typename U> struct is_member_pointer<T U::*> : true_type { };

    template<typename T> struct is_pointer : false_type { };
    template<typename T> struct is_pointer<T*> : true_type { };


    template<typename T> struct is_null_pointer : is_same<nullptr_t, remove_cv_t<T>> {};

#if defined(__cpp_variable_templates)
    template<typename T> constexpr const bool is_pointer_v = is_pointer<T>::value;
    template<typename T> constexpr const bool is_member_pointer_v = is_member_pointer<T>::value;
    template<typename T> constexpr const bool is_null_pointer_v = is_null_pointer<T>::value;
#endif


#pragma message "TODO: result_of/invoke_result are hacks"

    template<typename>
    struct result_of;

    template<typename F, typename ...Args>
    struct result_of<F(Args...)> {
        using type = decltype(std::declval<F>()(std::declval<Args>()...));
    };

    template<typename ...Args, typename R, typename T>
    struct result_of<R(T::*)(Args...)> { using type = R; };

    template<typename ...Args, typename R, typename T, typename Y>
    struct result_of<R (T::*(Y*, Args...))(Args...)> { using type = R; };


    template<typename T> using result_of_t = typename result_of<T>::type;

    template<typename T, typename ...Args>
    using invoke_result = result_of<T(Args...)>;

    template<typename T, typename ...Args>
    using invoke_result_t = typename invoke_result<T, Args...>::type;



    template<typename Base, typename Derived>
    using is_base_of = bool_constant<__is_base_of(Base, Derived)>;
#if defined(__cpp_variable_templates)
    template<typename Base, typename Derived>
    constexpr const bool is_base_of_v = is_base_of<Base, Derived>::value;
#endif


    namespace __detail
    {
        //using std::swap;

        struct is_swappable_impl
        {
            template<typename T, typename =
                decltype(swap(declval<T&>(), declval<T&>()))>
            static const constexpr true_type v(int);

            template<typename>
            static const constexpr false_type v(...);
        };

        struct is_nothrow_swappable_impl
        {
            template<typename T, typename V =
                bool_constant<noexcept(swap(declval<T&>(), declval<T&>()))>>
            static const constexpr V v(int);

            template<typename>
            static const constexpr false_type v(...);
        };
    }

    template<typename T> struct is_swappable :
        decltype(__detail::is_swappable_impl::v<T>(0)) { };

    template<typename T> struct is_nothrow_swappable :
        decltype(__detail::is_nothrow_swappable_impl::v<T>(0)) { };

#if defined(__cpp_variable_templates)
    template<typename T> constexpr const bool is_swappable_v = is_swappable<T>::value;
    template<typename T> constexpr const bool is_nothrow_swappable_v = is_nothrow_swappable<T>::value;
#endif

    // See http://www.boost.org/doc/libs/1_53_0/boost/type_traits/intrinsics.hpp
    // https://github.com/gcc-mirror/gcc/blob/master/libstdc%2B%2B-v3/include/std/type_traits
    // for examples for more

    namespace __detail
    {
        template<typename T> struct is_integral : false_type { };
        template<> struct is_integral<signed char> : true_type { };
        template<> struct is_integral<signed short> : true_type { };
        template<> struct is_integral<signed int> : true_type { };
        template<> struct is_integral<signed long int> : true_type { };
        template<> struct is_integral<signed long long int> : true_type { };
        template<> struct is_integral<unsigned char> : true_type { };
        template<> struct is_integral<unsigned short> : true_type { };
        template<> struct is_integral<unsigned int> : true_type { };
        template<> struct is_integral<unsigned long int> : true_type { };
        template<> struct is_integral<unsigned long long int> : true_type { };

        template<typename T> struct is_floating_point : false_type { };
        template<> struct is_floating_point<float> : true_type { };
        template<> struct is_floating_point<double> : true_type { };
        template<> struct is_floating_point<long double> : true_type { };
    }

    template<typename T>
    struct is_floating_point : __detail::is_floating_point<std::remove_cv_t<T>> { };

#if defined(__cpp_variable_templates)
    template<typename T> constexpr const bool is_floating_point_v = is_floating_point<T>::value;
#endif

    template<typename T>
    struct is_integral : __detail::is_integral<std::remove_cv_t<T>> { };

#if defined(__cpp_variable_templates)
    template<typename T> constexpr const bool is_integral_v = is_integral<T>::value;
#endif

    template<typename T, class = enable_if_t<is_integral<T>::value>> struct is_unsigned : false_type { };
    template<> struct is_unsigned<unsigned char> : true_type { };
    template<> struct is_unsigned<unsigned short> : true_type { };
    template<> struct is_unsigned<unsigned int> : true_type { };
    template<> struct is_unsigned<unsigned long int> : true_type { };
    template<> struct is_unsigned<unsigned long long int> : true_type { };

    template<typename T, class = enable_if_t<is_integral<T>::value>>
    struct is_signed : bool_constant<!is_unsigned<T>::value> { };

#if defined(__cpp_variable_templates)
    template<typename T> constexpr const bool is_unsigned_v = is_unsigned<T>::value;
    template<typename T> constexpr const bool is_signed_v = is_signed<T>::value;
#endif


    template<typename T>
    struct is_arithmetic :
        bool_constant<is_integral<T>::value || is_floating_point<T>::value> { };

    template<typename T>
    struct is_scalar : bool_constant<
        is_arithmetic<T>::value ||
        is_enum<T>::value ||
        is_pointer<T>::value ||
        is_member_pointer<T>::value ||
        is_null_pointer<T>::value> { };


#if defined(__cpp_variable_templates)
    template<typename T> constexpr const bool is_arithmetic_v = is_arithmetic<T>::value;
    template<typename T> constexpr const bool is_scalar_v = is_scalar<T>::value;
#endif


    // TODO: Support in VS
#if false

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
#else
	template<typename>
	struct is_function : public false_type { };
#endif

#if defined(__cpp_variable_templates)
    template<typename Return>
    constexpr const bool is_function_v = is_function<Return>::value;
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
        static constexpr const bool value = decltype(__test<From, To>(0))::value;
    };

#if defined(__cpp_variable_templates)
    template<typename From, typename To>
    constexpr const bool is_convertible_v = is_convertible<From, To>::type::value;
#endif

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


namespace std
{
	namespace __detail
	{
		template<typename Src, typename T>
		struct mirror_const_volatile {
		private:
			using const_type = std::conditional_t<std::is_const<Src>::value, const T, T>;
		public:
			using type = std::conditional_t<std::is_volatile<Src>::value, volatile const_type, const_type>;
		};

		template<template<typename> class Predicate, typename ...Ts>
		struct for_all {
			static constexpr const bool value = true;
		};
		template<template<typename> class Predicate, typename T, typename ...Ts>
		struct for_all<Predicate, T, Ts...> {
			static constexpr const bool value = Predicate<T>::value && for_all<Predicate, Ts...>::value;
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
}

namespace std
{
    template<typename T>
    struct make_unsigned {
        using type = typename __detail::mirror_const_volatile<T, typename __detail::make_unsigned_raw<T>::type>::type;
    };
    template<typename T>
    using make_unsigned_t = typename make_unsigned<T>::type;

    template<typename T>
    struct make_signed {
        using type = typename __detail::mirror_const_volatile<T, typename __detail::make_signed_raw<T>::type>::type;
    };
    template<typename T>
    using make_signed_t = typename make_signed<T>::type;
}

namespace std
{
	namespace __detail
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

        template<typename T>
        struct is_ref_qualified : false_type { };

        template<typename T, typename R, typename ...Args>
        struct is_ref_qualified<R(T::*)(Args...)&> : true_type { };
        template<typename T, typename R, typename ...Args>
        struct is_ref_qualified<R(T::*)(Args...)&&> : true_type { };

        template<typename R, typename T, typename Y, typename ...Args>
        struct is_ref_qualified<R (T::*(Y*, Args...))(Args...)&> : true_type { };
        template<typename R, typename T, typename Y, typename ...Args>
        struct is_ref_qualified<R (T::*(Y*, Args...))(Args...)&&> : true_type { };
    }

    template<typename T>
    struct add_pointer {
        using type = conditional_t<!is_function<T>::value || !__detail::is_ref_qualified<T>::value, T*, T>;
    };
    template<typename T> struct add_pointer<T&> { using type = T*; };
    template<typename T> struct add_pointer<T&&> { using type = T*; };

    template<typename T>
    using add_pointer_t = typename add_pointer<T>::type;


    template<typename T>
    struct remove_pointer { using type = T; };
    template<typename T>
    struct remove_pointer<T*> { using type = T; };
    template<typename T>
    struct remove_pointer<const T*> { using type = T; };
    template<typename T>
    struct remove_pointer<volatile T*> { using type = T; };
    template<typename T>
    struct remove_pointer<const volatile T*> { using type = T; };

    template<typename T>
    using remove_pointer_t = typename remove_pointer<T>::type;


    template<typename T> struct remove_extent { typedef T type; };
    template<typename T> struct remove_extent<T[]> { typedef T type; };

    template<typename T, size_t N>
    struct remove_extent<T[N]> { typedef T type; };

    template<typename T>
    using remove_extent_t = typename remove_extent<T>::type;



    template<typename T>
    struct decay
    {
    private:
        using Y = remove_reference_t<T>;
    public:
        using type = conditional_t<is_array<Y>::value, remove_extent_t<Y>*,
                        conditional_t<is_function<Y>::value, add_pointer_t<Y>,
                            remove_cv_t<Y>>>;
    };

    template<typename T>
    using decay_t = typename decay<T>::type;




    // primary template (used for zero types)
    template <class ...T> struct common_type { };

    template<class ...Ts>
    using common_type_t = typename common_type<Ts...>::type;


    //////// one type
    template <class T>
    struct common_type<T> {
        using type = std::decay_t<T>;
    };

    //////// two types

    // default implementation for two types
    template<class T1, class T2>
    using cond_t = decltype(false ? std::declval<T1>() : std::declval<T2>());

    template<class T1, class T2, class=void>
    struct common_type_2_default { };

    template<class T1, class T2>
    struct common_type_2_default<T1, T2, std::void_t<cond_t<T1, T2>>> {
        using type = std::decay_t<cond_t<T1, T2>>;
    };

    // dispatcher to decay the type before applying specializations
    template<class T1, class T2, class D1 = std::decay_t<T1>, class D2=std::decay_t<T2>>
    struct common_type_2_impl : common_type<D1, D2> {};

    template<class D1, class D2>
    struct common_type_2_impl<D1, D2, D1, D2> : common_type_2_default<D1, D2> {};

    template <class T1, class T2>
    struct common_type<T1, T2> : common_type_2_impl<T1, T2> { };

    //////// 3+ types

    template<class AlwaysVoid, class T1, class T2, class...R>
    struct common_type_multi_impl { };

    template< class T1, class T2, class...R>
    struct common_type_multi_impl<std::void_t<common_type_t<T1, T2>>, T1, T2, R...>
        : common_type<common_type_t<T1, T2>, R...>  { };

    template <class T1, class T2, class... R>
    struct common_type<T1, T2, R...>
        : common_type_multi_impl<void, T1, T2, R...> { };


	template<typename T>
	struct is_copy_constructible :
		is_constructible<T, add_lvalue_reference_t<add_const_t<T>>> { };

	template<typename T>
	struct is_trivially_copy_constructible :
		is_trivially_constructible<T, add_lvalue_reference_t<add_const_t<T>>> { };

	template<typename T>
	struct is_nothrow_copy_constructible :
		is_nothrow_constructible<T, add_lvalue_reference_t<add_const_t<T>>> { };

    template<typename T>
    struct alignment_of : integral_constant<size_t, alignof(T)> { };


    template<size_t Size, size_t Align = sizeof(max_align_t)>
    struct aligned_storage
    {
        struct type {
            alignas(Align) unsigned char data[Size];
        };
    };

    template<size_t Size, size_t Align = sizeof(max_align_t)>
    using aligned_storage_t = typename aligned_storage<Size, Align>::type;
}
}
