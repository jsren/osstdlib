/* memory.hpp - (c) 2017 James Renwick */
#pragma once
#include <type_traits>
#include <cstddef>
#include <limits>

namespace std
{
	namespace __detail
	{
		template<typename U>
		struct get_rebind_s {
			template<typename T> using type = typename T::template rebind<U>;
		};

		template<typename T>
		struct addressof_make_ref
		{
			T& v;
			inline constexpr addressof_make_ref(T& v) : v(v) {}
			inline constexpr operator T& () const { return v; }
		private:
			addressof_make_ref& operator=(const addressof_make_ref&);
		};

		template<typename T>
		struct addressof_s
		{
			static inline constexpr T* f(T* v, int) { return v; }
			static inline constexpr T* f(T& v, long) {
				return reinterpret_cast<T*>(&const_cast<char&>(
					reinterpret_cast<const volatile char &>(v)));
			}
		};

		template<typename T>
		struct has_select_on_container_copy_construction
		{
		private:
			template<typename Y, class = decltype(declval<Y>().select_on_container_copy_construction())>
			static true_type test(Y);
			static false_type test(...);
		public:
			static constexpr const bool value = decltype(test(declval<T>()))::value;
		};

		template<typename T, bool b = has_select_on_container_copy_construction<T>::value>
		struct call_select_on_container_copy_construction
		{
			auto operator()(T&& allocator) {
				return allocator.select_on_container_copy_construction();
			}
		};
		template<typename T>
		struct call_select_on_container_copy_construction<T, false>
		{
			auto operator()(T&& allocator) {
				return forward<T>(allocator);
			}
		};


		template<typename Alloc, typename T, typename ...Args>
		class has_allocator_construct
		{
			template<typename A, class = decltype(
				declval<A>().construct(declval<T*>(), declval<Args>()...))>
			static true_type test(int);
			template<class>
			static false_type test(...);
		public:
			static const constexpr bool value = decltype(test<Alloc>(0))::value;
		};

		template<typename Alloc, typename T>
		class has_allocator_destroy
		{
			template<typename A, class = decltype(
				declval<A>().destroy(declval<T*>()))>
			static true_type test(int);
			template<class>
			static false_type test(...);
		public:
			static const constexpr bool value = decltype(test<Alloc>(0))::value;
		};

		template<typename Alloc>
		class has_allocator_max_size
		{
			template<typename A, class = decltype(
				declval<A>().max_size())>
			static true_type test(int);
			template<class>
			static false_type test(...);
		public:
			static const constexpr bool value = decltype(test<const Alloc>(0))::value;
		};


		template<typename Alloc, typename T, bool Enable, typename ...Args>
		struct _call_allocator_construct {
			auto operator()(Alloc& allocator, T* object, Args&&... args) {
				return allocator.construct(object, forward<Args>(args)...);
			}
		};
		template<typename Alloc, typename T, typename ...Args>
		struct _call_allocator_construct<Alloc, T, false, Args...>
		{
			auto operator()(Alloc&, T* object, Args&&... args) {
				return ::new(static_cast<void*>(object)) T(forward<Args>(args)...);
			}
		};

		template<typename Alloc, typename T, typename ...Args>
		auto call_allocator_construct(Alloc& alloc, T* t, Args&&... args)
		{
			return _call_allocator_construct<Alloc, T,
				has_allocator_construct<Alloc, T, Args...>::value, Args...>()(
					alloc, t, forward<Args>(args)...
				);
		}

		template<typename Alloc, typename T, bool Enable =
			has_allocator_destroy<Alloc, T>::value>
		struct call_allocator_destroy
		{
			void operator()(Alloc& allocator, T* object) {
				allocator.destroy(object);
			}
		};

		template<typename Alloc, typename T>
		struct call_allocator_destroy<Alloc, T, false>
		{
			void operator()(Alloc&, T* object) {
				object->~T();
			}
		};

		template<typename Alloc, typename size_type, typename value_type,
			bool Enable = has_allocator_max_size<Alloc>::value>
		struct call_allocator_max_size
		{
			auto operator()(const Alloc& alloc) {
				return alloc.max_size();
			}
		};

		template<typename Alloc, typename size_type, typename value_type>
		struct call_allocator_max_size<Alloc, size_type, value_type, false>
		{
			auto operator()(const Alloc&)
			{
				return numeric_limits<size_type>::max() /
					sizeof(typename Alloc::value_type);
			}
		};
	}


    template<typename T>
    inline constexpr T* addressof(T& v) noexcept {
        return __detail::addressof_s<T>::f(__detail::addressof_make_ref<T>(v), 0);
    }
    template <typename T>
    const T* addressof(const T&&) = delete;

	template<typename T>
	struct default_delete
	{
		constexpr default_delete() = default;

		template<typename Y, class=enable_if_t<is_convertible<Y, T>::value>>
        inline default_delete(const default_delete<Y>&) noexcept { }

		inline void operator()(T* ptr) const {
			delete ptr;
		}
	};
	template<typename T>
	struct default_delete<T[]>
	{
        constexpr default_delete() = default;

		template<typename Y, class=enable_if<is_convertible<Y, T>::value>>
        inline default_delete(const default_delete<Y[]>&) noexcept { }

		template<typename Y, class=enable_if_t<is_convertible<Y, T>::value>>
		inline void operator()(Y* ptr) const {
			delete[] ptr;
		}
	};

    template<typename Ptr>
    struct pointer_traits
    {
    private:
        template<typename T>
        using _get_element_type = typename T::element_type;
        template<typename T>
        using _get_difference_type = typename T::difference_type;

    public:
        using pointer = Ptr;

        using element_type = __detail::scope_type_or_default_t<
            Ptr, _get_element_type, __detail::first_tparam_t<Ptr>>;

        using difference_type = __detail::scope_type_or_default_t<
            Ptr, _get_difference_type, ptrdiff_t>;

        template<typename T>
        using rebind = __detail::scope_type_or_default_t<
            Ptr, __detail::get_rebind_s<T>::template type, __detail::rebind_t<Ptr, T>>;

        inline static pointer pointer_to(element_type& reference)
            noexcept(noexcept(Ptr::pointer_to(reference)))
        {
            return Ptr::pointer_to(reference);
        }
    };
    template<typename T>
    struct pointer_traits<T*>
    {
        using pointer = T*;
        using element_type = T;
        using difference_type = ptrdiff_t;

        template<typename U>
        using rebind = U*;

        inline static pointer pointer_to(element_type& reference) noexcept {
            return std::addressof(reference);
        }
    };


    template<typename Alloc>
    struct allocator_traits
    {
    private:
        template<typename T>
        using _get_pointer = typename T::pointer;
        template<typename T>
        using _get_const_pointer = typename T::const_pointer;
        template<typename T>
        using _get_void_pointer = typename T::void_pointer;
        template<typename T>
        using _get_const_void_pointer = typename T::const_void_pointer;
        template<typename T>
        using _get_difference_type = typename T::difference_type;
        template<typename T>
        using _get_size_type = typename T::size_type;
		template<typename T>
		using _get_other = typename T::other;
        template<typename T>
        using _get_propagate_on_container_copy_assignment =
            typename T::propagate_on_container_copy_assignment;
        template<typename T>
        using _get_propagate_on_container_move_assignment =
            typename T::propagate_on_container_move_assignment;
        template<typename T>
        using _get_propagate_on_container_swap =
            typename T::propagate_on_container_swap;
        template<typename T>
        using _get_is_always_equal = typename T::is_always_equal;

		template<typename T, typename ...Args>
		struct rebind { };

		template<typename T, template<class, class...> class _Alloc, typename Y, typename ...Args>
		struct rebind<T, _Alloc<Y, Args...>> {
			using type = _Alloc<T, Args...>;
		};

    public:
        using allocator_type = Alloc;
        using value_type = typename Alloc::value_type;

        using pointer = __detail::scope_type_or_default_t<
            Alloc, _get_pointer, value_type*>;
        using const_pointer = __detail::scope_type_or_default_t<
            Alloc, _get_const_pointer, typename pointer_traits<pointer>::template rebind<const value_type>>;
        using void_pointer = __detail::scope_type_or_default_t<
            Alloc, _get_void_pointer, typename pointer_traits<pointer>::template rebind<void>>;
        using const_void_pointer = __detail::scope_type_or_default_t<
            Alloc, _get_const_void_pointer, typename pointer_traits<pointer>::template rebind<const void>>;

        using difference_type = __detail::scope_type_or_default_t<
            Alloc, _get_difference_type, typename pointer_traits<pointer>::difference_type>;
        using size_type = __detail::scope_type_or_default_t<
            Alloc, _get_size_type, typename make_unsigned<difference_type>::type>;

        using propagate_on_container_copy_assignment = __detail::scope_type_or_default_t<
            Alloc, _get_propagate_on_container_copy_assignment, false_type>;
        using propagate_on_container_move_assignment = __detail::scope_type_or_default_t<
            Alloc, _get_propagate_on_container_move_assignment, false_type>;
        using propagate_on_container_swap = __detail::scope_type_or_default_t<
            Alloc, _get_propagate_on_container_swap, false_type>;

        using is_always_equal = __detail::scope_type_or_default_t<
            Alloc, _get_is_always_equal, is_empty<Alloc>>;

		template<typename T>
		using rebind_alloc = __detail::scope_type_or_default_t<
			__detail::scope_type_or_default_t<Alloc,
				__detail::get_rebind_s<T>::template type, void_t<>>,
			_get_other, typename rebind<T, Alloc>::type>;

		template<typename T>
		using rebind_traits = allocator_traits<rebind_alloc<T>>;

        inline static pointer allocate(Alloc& alloc, size_type count) {
            return alloc.allocate(count);
        }
        static void deallocate(Alloc& alloc, pointer p, size_type count) {
            alloc.deallocate(p, count);
        }

		template<typename T, typename ...Args>
		static void construct(Alloc& alloc, T* object, Args&& ...args)
		{
			__detail::call_allocator_construct(alloc, object, forward<Args>(args)...);
		}

		template<typename T>
		static void destroy(Alloc& alloc, T* object)
		{
			__detail::call_allocator_destroy<Alloc, T>()(alloc, object);
		}

		static size_type max_size(const Alloc& alloc) noexcept
		{
			return __detail::call_allocator_max_size<Alloc, size_type, value_type>()(alloc);
		}

		static Alloc select_on_container_copy_construction(const Alloc& allocator)
		{
			return __detail::call_select_on_container_copy_construction<Alloc>()(allocator);
		}


    };


	namespace __detail
	{
		template<typename T, typename Deleter>
		struct pointer_type
		{
			
		};
	}

	/* A pointer type enforcing automatic object destruction when the
	referenced object leaves the current scope. */
	template<typename T, typename Deleter = default_delete<T>>
	struct unique_ptr
	{
	private:
		T* ptr = nullptr;
        Deleter deleter{};

		template<typename Y>
		using get_deleter_pointer = typename remove_reference_t<Y>::pointer;

	public:
		using element_type = T;
		using pointer = __detail::scope_type_or_default_t<
			Deleter, get_deleter_pointer, T*>;
		using deleter_type = Deleter;

		constexpr unique_ptr() noexcept = default;
        constexpr unique_ptr(nullptr_t) noexcept { };

		/* Creates a new unique_ptr from the given object pointer. */
		explicit unique_ptr(T* obj) noexcept : ptr(obj) { }

		// No copy constructor
		unique_ptr(const unique_ptr<T>& ptr) = delete;
		// No copy assignment
		unique_ptr& operator =(const unique_ptr<T>& ptr) = delete;

		// Move constructor
		unique_ptr(unique_ptr<T>&& ptr) noexcept
		{
			std::swap(this->ptr, ptr.ptr);
			std::swap(this->deleter, ptr.deleter);
		}
		template<typename Y, typename D1,
			class=std::enable_if_t<!is_array<Y>::value && is_convertible<D1, Deleter>::value>>
		unique_ptr(unique_ptr<Y, D1>&& other)
		{
			std::swap(this->ptr, other.ptr);
			std::swap(this->deleter, other.deleter);
		}

		// Move assignment
		unique_ptr& operator =(unique_ptr<T>&& ptr) noexcept
		{
			std::swap(this->ptr, ptr.ptr);
			std::swap(this->deleter, ptr.deleter);
		}

        T* get() noexcept { return ptr; }
        const T* get() const noexcept { return ptr; }

		Deleter& get_deleter() noexcept { return deleter; }
		const Deleter& get_deleter() const noexcept { return deleter; }

		operator bool() const noexcept { return ptr != nullptr; }

		/* Calling this destructor will also invoke the destructor
		of the object being pointed to. */
		~unique_ptr() {
			if (this->ptr != nullptr) deleter(this->ptr);
		}

	public:
		/* Structure dereference. */
		T* operator ->() const noexcept { return ptr; }
		/* Indirection. */
		T& operator *() const { return *ptr; }

		T* release() noexcept {
			auto old = ptr; ptr = nullptr; return ptr;
		}

		void reset(pointer ptr = pointer()) noexcept
		{
			if (this->ptr != nullptr) deleter(this->ptr);
			this->ptr = ptr;
		}

		void swap(unique_ptr& other) noexcept
		{
			std::swap(this->ptr, other.ptr);
			std::swap(this->deleter, other.deleter);
		}
	};

	/* A pointer type enforcing automatic object destruction when the
	referenced object leaves the current scope. */
	template<typename T, typename Deleter>
	struct unique_ptr<T[], Deleter>
	{
	private:
		T* ptr = nullptr;
        Deleter deleter{};

		template<typename Y>
		using get_deleter_pointer = typename remove_reference_t<Y>::pointer;

	public:
		using element_type = T[];
		using pointer = __detail::scope_type_or_default_t<
			Deleter, get_deleter_pointer, T*>;
		using deleter_type = Deleter;

        constexpr unique_ptr() noexcept { };
        constexpr unique_ptr(nullptr_t) noexcept { }

		/* Creates a new unique_ptr from the given object pointer. */
		explicit unique_ptr(T* obj) noexcept : ptr(obj) { }

		// No copy constructor/assignment
		unique_ptr(const unique_ptr<T>& ptr) = delete;
		unique_ptr& operator =(const unique_ptr<T>& ptr) = delete;

		// Move constructor
		unique_ptr(unique_ptr<T[]>&& ptr) noexcept
		{
			this->ptr = ptr.ptr;
			ptr.ptr = nullptr;
		}
		// Move assignment
		unique_ptr& operator =(unique_ptr<T>&& ptr) noexcept
		{
			this->ptr = ptr.ptr;
			ptr.ptr = nullptr;
			return *this;
		}

        unique_ptr& operator =(T ptr[])
        {
            if (this->ptr != nullptr) deleter(this->ptr);
            this->ptr = ptr;
            return *this;
        }

        T* get() noexcept { return ptr; }
        const T* get() const noexcept { return ptr; }

		Deleter& get_deleter() noexcept { return deleter; }
		const Deleter& get_deleter() const noexcept { return deleter; }

		operator bool() const noexcept { return ptr != nullptr; }

		/* Structure dereference. */
		T* operator ->() const noexcept { return ptr; }
		/* Indirection. */
		T& operator *() const { return *ptr; }

		T* release() noexcept {
			auto old = ptr; ptr = nullptr; return ptr;
		}

		template<typename Y, class=enable_if_t<is_same<Y, pointer>::value ||
			is_same<pointer, element_type*>::value>>
		void reset(Y ptr) noexcept
		{
			if (this->ptr != nullptr) deleter(this->ptr);
			this->ptr = ptr;
		}
		void reset(nullptr_t ptr = nullptr) noexcept
		{
			if (this->ptr != nullptr) deleter(this->ptr);
			this->ptr = ptr;
		}

		void swap(unique_ptr& other) noexcept
		{
			std::swap(this->ptr, other.ptr);
			std::swap(this->deleter, other.deleter);
		}

		/* Calling this destructor will also invoke the destructor
		of the object being pointed to. */
		~unique_ptr() {
            if (this->ptr != nullptr) deleter(this->ptr);
        }
	};

	template<typename T, typename Deleter>
    void swap(unique_ptr<T, Deleter>& lhs, unique_ptr<T, Deleter>& rhs) noexcept
	{
		lhs.swap(rhs);
	}


	/*
	A pointer type allowing for the creation of other pointers,
	each pointing to the same object. This creation happens via the copy constructor.
	Atomatic object destruction occurs when the final referenced to the object is destroyed.
	*/
	template<typename T>
	struct shared_ptr
	{
	private:
		T* ptr;
		size_t* refCount;

	public:
		/* Creates the first shared_ptr for the given object. */
		inline explicit shared_ptr(T* ptr) : ptr(ptr), refCount(new size_t(1)) { }

		/* Creates the first shared_ptr for the given object. */
		inline explicit shared_ptr(const T& obj) : ptr(&obj), refCount(new size_t(1)) { }

		/* Creates another shared_ptr pointing to the same object. */
		inline shared_ptr(const shared_ptr& copy) noexcept : ptr(copy.ptr), refCount(copy.refCount)
		{
			*(this->refCount)++;
		}

		/* Destructor. When called, will decrement the object's reference count
		and destroy the object when the count reaches zero. */
		inline ~shared_ptr()
		{
			if (--(*this->refCount) == 0)
			{
				if (this->ptr != nullptr) delete this->ptr;
				if (this->refCount != nullptr) delete this->refCount;
			}
		}

		/*  */
		shared_ptr& operator =(const shared_ptr& value)
		{
			if (this != &value)
			{
				if (--(*this->refCount) == 0)
				{
					delete this->ptr;
					delete this->refCount;
				}
				this->ptr      = value.ptr;
				this->refCount = ++(*value.refCount);
			}
			return *this;
		}

        inline T* get() noexcept { return ptr; }
        inline const T* get() const noexcept { return ptr; }

		/* Structure dereference. */
		inline T* operator ->() const noexcept { return ptr; }
		/* Indirection. */
		inline T& operator  *() const { return *ptr; }

		/* Prevent move operations. */
		shared_ptr(shared_ptr&& value)             = delete;
		shared_ptr& operator =(shared_ptr&& value) = delete;
	};
}
