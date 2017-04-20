//
//#if !defined(__STD_DECLTYPE_AUTO)
//    #if defined(__cpp_decltype_auto)
//        #define __STD_DECLTYPE_AUTO decltype(auto)
//    #else
//        #define __STD_DECLTYPE_AUTO auto
//    #endif
//#endif
//
//// Handle no constexpr support
//#if !defined(__STD_CONSTEXPR)
//    // MSVC wisely decided that "constexpr == constexpr const"
//    // Thanks, Microsoft
//    #if !defined(__cpp_constexpr) || defined(_MSC_VER)
//        #define __STD_CONSTEXPR
//        #define __STD_CONSTEXPR_CONST constexpr
//    #else
//        #define __STD_CONSTEXPR constexpr
//        #define __STD_CONSTEXPR_CONST constexpr
//    #endif
//#endif
//
//
//namespace std
//{
//	class nullptr_t
//	{
//	private:
//		unsigned : sizeof(void*);
//
//	public:
//		template<typename T>
//        inline __STD_CONSTEXPR operator T*() const { return (T*)0; }
//
//		template<typename T, typename Y>
//        inline __STD_CONSTEXPR operator T Y::*() const { return 0; }
//
//		void operator&() const = delete;
//		inline __STD_CONSTEXPR operator void*() const { return (void*)0; }
//
//		inline __STD_CONSTEXPR bool operator==(const nullptr_t&) const { return true; }
//		inline __STD_CONSTEXPR bool operator!=(const nullptr_t&) const { return false; }
//	};
//    
//
//    template <class T, T v>
//    struct integral_constant
//    {
//        using value_type = T;
//        using type = integral_constant<T, v>;
//
//        static constexpr T value = v;
//        inline constexpr operator T() const noexcept { return v; }
//        inline constexpr T operator()() const noexcept { return v; }
//    };
//    template<bool v> using bool_constant = integral_constant<bool, v>;
//
//
//    using true_type  = bool_constant<true>;
//    using false_type = bool_constant<false>;
//
//
//    template<bool, bool> struct And : false_type { };
//    template<> struct And<true, true> : true_type { };
//
//    template<bool, bool> struct Or : true_type { };
//    template<> struct Or <false, false> : false_type { };
//
//    template<bool> struct Not : true_type { };
//    template<> struct Not<true> : false_type { };
//
//
//	template<typename> struct is_void : false_type { };
//	template<> struct is_void<void> : true_type { };
//#if defined(__cpp_variable_templates)
//	template<typename T> constexpr bool is_void_v = is_void<T>::value;
//#endif
//
//	template<typename T> 
//	struct is_array : false_type { };
//	template<typename T, size_t N>
//	struct is_array<T[N]> : true_type { };
//	template<typename T> 
//	struct is_array<T[]> : true_type { };
//#if defined(__cpp_variable_templates)
//	template<typename T>
//	constexpr bool is_array_v = is_array<T>::value;
//#endif
//
//
//    template<long long int N> struct is_even : bool_constant<N % 2 == 0> { };
//#if defined(__cpp_variable_templates)
//    template<long long int N> constexpr bool is_even_v = is_even<N>::value;
//#endif
//
//	template<typename T> struct add_reference { typedef T& type; };
//	template<typename T> struct add_reference<T&> { typedef T& type; };
//	template<typename T> struct add_reference<T&&> { typedef T& type; };
//	template<typename T> using add_reference_t = typename add_reference<T>::type;
//
//    template<typename T> struct remove_reference { typedef T type; };
//    template<typename T> struct remove_reference<T&> { typedef T type; };
//    template<typename T> struct remove_reference<T&&> { typedef T type; };
//    template<typename T> using remove_reference_t = typename remove_reference<T>::type;
//
//	template<typename T> struct add_const { typedef const T type; };
//	template<typename T> struct add_const<const T> { typedef const T type; };
//	template<typename T> using add_const_t = typename add_const<T>::type;
//
//    template<typename T> struct remove_const { typedef T type; };
//    template<typename T> struct remove_const<const T> { typedef T type; };
//    template<typename T> using remove_const_t = typename remove_const<T>::type;
//
//    template<typename T> struct remove_volatile { typedef T type; };
//    template<typename T> struct remove_volatile<volatile T> { typedef T type; };
//    template<typename T> using remove_volatile_t = typename remove_volatile<T>::type;
//
//    template<typename T> struct bare_type { typedef remove_const_t<remove_volatile_t<remove_reference_t<T>>> type; };
//    template<typename T> using bare_type_t = remove_const_t<remove_volatile_t<remove_reference_t<T>>>;
//
//    template<typename T> struct is_volatile : false_type {  };
//    template<typename T> struct is_volatile<volatile T> : true_type { };
//#if defined(__cpp_variable_templates)
//    template<typename T> constexpr bool is_volatile_v = is_volatile<T>::value;
//#endif
//
//    template<typename T> struct is_const : false_type {  };
//    template<typename T> struct is_const<const T> : true_type { };
//#if defined(__cpp_variable_templates)
//    template<typename T> constexpr bool is_const_v = is_const<T>::value;
//#endif
//
//    template<bool, class T = void> struct enable_if { };
//    template<class T> struct enable_if<true, T> { typedef T type; };
//
//    template<bool B, class T = void>
//    using enable_if_t = typename enable_if<B, T>::type;
//
//    template<typename T> using is_union      = bool_constant<__is_union(T)>;
//    template<typename T> using is_empty_type = bool_constant<__is_empty(T)>;
//    template<typename T> using is_class      = bool_constant<__is_class(T)>;
//    template<typename T> using is_abstract   = bool_constant<__is_abstract(T)>;
//    template<typename T> using is_enum       = bool_constant<__is_enum(T)>;
//
//	
//#if defined(__cpp_variable_templates)
//    template<typename T> constexpr bool is_union_v      = is_union<T>::value;
//    template<typename T> constexpr bool is_empty_type_v = is_empty_type<T>::value;
//    template<typename T> constexpr bool is_class_v      = is_class<T>::value;
//    template<typename T> constexpr bool is_abstract_v   = is_abstract<T>::value;
//    template<typename T> constexpr bool is_enum_v       = is_enum<T>::value;
//#endif
//
//    template<typename T> using has_trivial_constructor = bool_constant<__has_trivial_constructor(T)>;
//    template<typename T> using has_virtual_destructor  = bool_constant<__has_virtual_destructor(T)>;
//#if defined(__cpp_variable_templates)
//    template<typename T> constexpr bool has_trivial_constructor_v = has_trivial_constructor<T>::value;
//    template<typename T> constexpr bool has_virtual_destructor_v  = has_virtual_destructor<T>::value;
//#endif
//
//    template<typename T> using is_pod = std::And<__is_pod(T),
//        has_trivial_constructor<T>::value>;
//
//    template<typename T> using has_trivial_copy = std::Or<__has_trivial_copy(T),
//        std::And<is_pod<T>::value, std::Not<is_volatile<T>::value>::value>::value>;
//
//#if defined(__cpp_variable_templates)
//    template<typename T> constexpr bool is_pod_v = is_pod<T>::value;
//    template<typename T> constexpr bool has_trivial_copy_v = has_trivial_copy<T>::value;
//#endif
//
//
//	template<typename Base, typename Derived> 
//	using is_base_of = bool_constant<__is_base_of(Base, Derived)>;
//#if defined(__cpp_variable_templates)
//    template<typename Base, typename Derived>
//    constexpr bool is_base_of_v = is_base_of<Base, Derived>::value;
//#endif
//
//    // See http://www.boost.org/doc/libs/1_53_0/boost/type_traits/intrinsics.hpp
//	// https://github.com/gcc-mirror/gcc/blob/master/libstdc%2B%2B-v3/include/std/type_traits
//    // for examples for more
//
//    template<typename T> struct is_fractional : false_type { };
//    template<> struct is_fractional<float> : true_type { };
//    template<> struct is_fractional<double> : true_type { };
//#if defined(__cpp_variable_templates)
//    template<typename T> constexpr bool is_fractional_v = is_fractional<T>::value;
//#endif
//
//    template<typename T> struct is_integer : false_type { };
//    template<> struct is_integer<signed char> : true_type { };
//    template<> struct is_integer<signed short> : true_type { };
//    template<> struct is_integer<signed int> : true_type { };
//    template<> struct is_integer<signed long int> : true_type { };
//    template<> struct is_integer<signed long long int> : true_type { };
//    template<> struct is_integer<unsigned char> : true_type { };
//    template<> struct is_integer<unsigned short> : true_type { };
//    template<> struct is_integer<unsigned int> : true_type { };
//    template<> struct is_integer<unsigned long int> : true_type { };
//    template<> struct is_integer<unsigned long long int> : true_type { };
//#if defined(__cpp_variable_templates)
//    template<typename T> constexpr bool is_integer_v = is_integer<T>::value;
//#endif
//
//    template<typename T, class=enable_if_t<is_integer<T>::value>> struct is_unsigned : false_type { };
//    template<> struct is_unsigned<unsigned char> : true_type { };
//    template<> struct is_unsigned<unsigned short> : true_type { };
//    template<> struct is_unsigned<unsigned int> : true_type { };
//    template<> struct is_unsigned<unsigned long int> : true_type { };
//    template<> struct is_unsigned<unsigned long long int> : true_type { };
//
//    template<typename T, class=enable_if_t<is_integer<T>::value>>
//    struct is_signed : Not<is_unsigned<T>::value> { };
//
//#if defined(__cpp_variable_templates)
//    template<typename T> constexpr bool is_unsigned_v = is_unsigned<T>::value;
//    template<typename T> constexpr bool is_signed_v = is_signed<T>::value;
//#endif
//
//    //template<typename T=void> struct _is_empty : true_type { };
//    //template<typename T, typename ...Ts> struct _is_empty : false_type { };
//
//
//	template<class T>
//	std::remove_reference_t<T>&& declval();
//
//
//// TODO: Support in VS
//#if !defined(_MSC_VER)
//
//	// is_function - Taken from libstdc++
//	template<typename>
//	struct is_function : public false_type { };
//	template<typename _Res, typename... _ArgTypes>
//	struct is_function<_Res(_ArgTypes...)> : public true_type { };
//	template<typename _Res, typename... _ArgTypes>
//	struct is_function<_Res(_ArgTypes...) &> : public true_type { };
//	template<typename _Res, typename... _ArgTypes>
//	struct is_function<_Res(_ArgTypes...) && > : public true_type { };
//	template<typename _Res, typename... _ArgTypes>
//	struct is_function<_Res(_ArgTypes......)> : public true_type { };
//	template<typename _Res, typename... _ArgTypes>
//	struct is_function<_Res(_ArgTypes......) &> : public true_type { };
//	template<typename _Res, typename... _ArgTypes>
//	struct is_function<_Res(_ArgTypes......) && > : public true_type { };
//	template<typename _Res, typename... _ArgTypes>
//	struct is_function<_Res(_ArgTypes...) const> : public true_type { };
//	template<typename _Res, typename... _ArgTypes>
//	struct is_function<_Res(_ArgTypes...) const &> : public true_type { };
//	template<typename _Res, typename... _ArgTypes>
//	struct is_function<_Res(_ArgTypes...) const &&> : public true_type { };
//	template<typename _Res, typename... _ArgTypes>
//	struct is_function<_Res(_ArgTypes......) const> : public true_type { };
//	template<typename _Res, typename... _ArgTypes>
//	struct is_function<_Res(_ArgTypes......) const &> : public true_type { };
//	template<typename _Res, typename... _ArgTypes>
//	struct is_function<_Res(_ArgTypes......) const &&> : public true_type { };
//	template<typename _Res, typename... _ArgTypes>
//	struct is_function<_Res(_ArgTypes...) volatile> : public true_type { };
//	template<typename _Res, typename... _ArgTypes>
//	struct is_function<_Res(_ArgTypes...) volatile &> : public true_type { };
//	template<typename _Res, typename... _ArgTypes>
//	struct is_function<_Res(_ArgTypes...) volatile &&> : public true_type { };
//	template<typename _Res, typename... _ArgTypes>
//	struct is_function<_Res(_ArgTypes......) volatile> : public true_type { };
//	template<typename _Res, typename... _ArgTypes>
//	struct is_function<_Res(_ArgTypes......) volatile &> : public true_type { };
//	template<typename _Res, typename... _ArgTypes>
//	struct is_function<_Res(_ArgTypes......) volatile &&> : public true_type { };
//	template<typename _Res, typename... _ArgTypes>
//	struct is_function<_Res(_ArgTypes...) const volatile> : public true_type { };
//	template<typename _Res, typename... _ArgTypes>
//	struct is_function<_Res(_ArgTypes...) const volatile &> : public true_type { };
//	template<typename _Res, typename... _ArgTypes>
//	struct is_function<_Res(_ArgTypes...) const volatile &&> : public true_type { };
//	template<typename _Res, typename... _ArgTypes>
//	struct is_function<_Res(_ArgTypes......) const volatile> : public true_type { };
//	template<typename _Res, typename... _ArgTypes>
//	struct is_function<_Res(_ArgTypes......) const volatile &> : public true_type { };
//	template<typename _Res, typename... _ArgTypes>
//	struct is_function<_Res(_ArgTypes......) const volatile &&> : public true_type { };
//
//#if defined(__cpp_variable_templates)
//	template<typename Return>
//	constexpr bool is_function_v = is_function<Return>::value;
//#endif
//#endif
//
//	template<typename From, typename To,
//		bool = is_void<From>::value || is_function<To>::value || is_array<To>::value>
//	struct is_convertible : is_void<To> { };
//
//	template<typename From, typename To>
//	class is_convertible<From, To, false>
//	{
//		template<typename _To>
//		static void __test_aux(_To);
//
//		template<typename _From, typename _To,
//			typename = decltype(__test_aux<_To>(std::declval<_From>()))>
//		static true_type __test(int);
//
//		template<typename, typename>
//		static false_type __test(...);
//
//	public:
//		typedef decltype(__test<From, To>(0)) type;
//	};
//
//#if defined(__cpp_variable_templates)
//	template<typename From, typename To>
//	constexpr bool is_convertible_v = is_convertible<From, To>::type::value;
//#endif
//
//	template<typename T>
//	constexpr inline typename remove_reference<T>::type&& move(T&& ref) noexcept
//	{
//		return static_cast<typename remove_reference<T>::type&&>(ref);
//	}
//
//	template<typename T>
//	constexpr inline T&& forward(typename remove_reference<T>::type&& ref) noexcept
//	{
//		return static_cast<T&&>(ref);
//	}
//
//	template<typename T1, typename T2>
//	inline void swap(T1& object1, T2& object2)
//	{
//		T1 tmp  = std::move(object1);
//		object1 = std::move(object2);
//		object2 = std::move(tmp);
//	}
//
//
//    template <class T, T... Is_>
//    struct integer_sequence { };
//
//    template <size_t... Is_>
//    using index_sequence = integer_sequence<size_t, Is_...>;
//
//    namespace _internal
//    {
//        template <class T, T Start, typename, T End>
//        struct _integer_sequence;
//
//        template <class T, T Start, T... Indexes_, T End>
//        struct _integer_sequence<T, Start, integer_sequence<T, Indexes_...>, End> {
//            typedef typename _integer_sequence<T, Start + 1, integer_sequence<T, Indexes_..., Start>, End>::type type;
//        };
//        template <class T, T End, T... Indexes_>
//        struct _integer_sequence<T, End, integer_sequence<T, Indexes_...>, End> {
//            typedef integer_sequence<T, Indexes_...> type;
//        };
//
//        template <class T, T End, T Start = 0>
//        struct _make_integer_sequence {
//            static_assert(Start <= End, "Start index cannot be greater than end index.");
//            typedef typename _integer_sequence<T, Start, integer_sequence<T>, End>::type type;
//        };
//    }
//
//    template <class T, T End, T Start = 0>
//    using make_integer_sequence = typename _internal::_make_integer_sequence<T, End, Start>::type;
//
//    template <size_t End, size_t Start = 0>
//    using make_index_sequence = typename _internal::_make_integer_sequence<size_t, End, Start>::type;
//
//
//	template<class T>
//	struct default_delete
//	{
//		constexpr default_delete() = default;
//
//		template<typename Y, class=enable_if_t<is_convertible<Y, T>::value>>
//        inline default_delete(const default_delete<Y>&) noexcept { }
//
//		inline void operator()(T* ptr) const {
//			delete ptr;
//		}
//	};
//	template<typename T>
//	struct default_delete<T[]>
//	{
//        constexpr default_delete() = default;
//
//		template<typename Y, class=enable_if<is_convertible<Y, T>::value>>
//        inline default_delete(const default_delete<Y[]>&) noexcept { };
//
//		template<typename Y, class=enable_if<is_convertible<Y, T>::value>>
//		inline void operator()(Y* ptr) const {
//			delete[] ptr;
//		}
//	};
//
//
//
//	/* A pointer type enforcing automatic object destruction when the 
//	referenced object leaves the current scope. */ 
//	template<typename T, typename Deleter = default_delete<T>>
//	struct unique_ptr
//	{
//	private:
//		T* ptr = nullptr;
//        Deleter deleter{};
//
//	public:
//		using element_type = T;
//
//		constexpr unique_ptr() noexcept = default;
//        constexpr inline unique_ptr(nullptr_t) noexcept { };
//
//		/* Creates a new unique_ptr from the given object pointer. */
//		inline explicit unique_ptr(T* obj) noexcept : ptr(obj) { }
//
//		// No copy constructor
//		unique_ptr(const unique_ptr<T>& ptr) = delete;
//		// No copy assignment
//		unique_ptr& operator =(const unique_ptr<T>& ptr) = delete;
//
//		// Move constructor
//		inline unique_ptr(unique_ptr<T>&& ptr) noexcept
//		{
//			std::swap(this->ptr, ptr.ptr);
//			std::swap(this->deleter, ptr.deleter);
//		}
//		template<typename Y, typename D1, 
//			class=std::enable_if<!is_array<Y>::value && is_convertible<D1, Deleter>::value>>
//		inline unique_ptr(unique_ptr<Y, D1>&& other)
//		{
//			std::swap(this->ptr, other.ptr);
//			std::swap(this->deleter, other.deleter);
//		}
//
//		// Move assignment
//		inline unique_ptr& operator =(unique_ptr<T>&& ptr) noexcept
//		{
//			std::swap(this->ptr, ptr.ptr);
//			std::swap(this->deleter, ptr.deleter);
//		}
//
//        inline T* get() noexcept { return ptr; }
//        inline const T* get() const noexcept { return ptr; }
//
//		/* Calling this destructor will also invoke the destructor
//		of the object being pointed to. */
//		inline ~unique_ptr() {
//			if (this->ptr != nullptr) deleter(this->ptr);
//		}
//
//	public:
//		/* Structure dereference. */
//		inline T* operator ->() const noexcept { return ptr; }
//		/* Indirection. */
//		inline T& operator *() const { return *ptr; }
//	};
//
//	/* A pointer type enforcing automatic object destruction when the
//	referenced object leaves the current scope. */
//	template<typename T, typename Deleter>
//	struct unique_ptr<T[], Deleter>
//	{
//	private:
//		T* ptr = nullptr;
//        Deleter deleter{};
//
//	public:
//		using element_type = T[];
//
//        constexpr unique_ptr() noexcept { };
//        constexpr inline unique_ptr(nullptr_t) noexcept { }
//
//		/* Creates a new unique_ptr from the given object pointer. */
//		inline explicit unique_ptr(T* obj) noexcept : ptr(obj) { }
//
//		// No copy constructor/assignment
//		unique_ptr(const unique_ptr<T>& ptr) = delete;
//		unique_ptr& operator =(const unique_ptr<T>& ptr) = delete;
//
//		// Move constructor
//		inline unique_ptr(unique_ptr<T[]>&& ptr) noexcept
//		{
//			this->ptr = ptr.ptr;
//			ptr.ptr = nullptr;
//		}
//		// Move assignment
//		inline unique_ptr& operator =(unique_ptr<T>&& ptr) noexcept
//		{
//			this->ptr = ptr.ptr;
//			ptr.ptr = nullptr;
//			return *this;
//		}
//
//        inline unique_ptr& operator =(T ptr[])
//        {
//            if (this->ptr != nullptr) deleter(this->ptr);
//            this->ptr = ptr;
//            return *this;
//        }
//
//        inline T* get() noexcept { return ptr; }
//        inline const T* get() const noexcept { return ptr; }
//
//		/* Structure dereference. */
//		inline T* operator ->() const noexcept { return ptr; }
//		/* Indirection. */
//		inline T& operator *() const { return *ptr; }
//
//		/* Calling this destructor will also invoke the destructor
//		of the object being pointed to. */
//		~unique_ptr() {
//            if (this->ptr != nullptr) deleter(this->ptr);
//        }
//	};
//}
