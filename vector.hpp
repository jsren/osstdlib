/* vector - (c) 2018 James Renwick */
#pragma once

#include <__config>
#include <allocator>
#include <cstddef>
#include <memory>
#include <__iterator>
#include <initializer_list>
#include <stdexcept>
#include <__smallbuff>
#include <new>

namespace std
{
    namespace __detail
    {
        template<typename T, typename Allocator>
        struct vector_sbo_config
        {
			using value_type = T;
            using alloc_traits = allocator_traits<Allocator>;
            using allocator_type = Allocator;
            using size_type = typename alloc_traits::size_type;
            using pointer = typename alloc_traits::pointer;
            using const_pointer = typename alloc_traits::const_pointer;

            static constexpr size_t get_max_size() {
                auto v = numeric_limits<size_type>::max() - 1;
                return ((v & 0b1) == 0) ? v : v - 1;
            }

            static constexpr const auto realloc_factor = _config::vector_realloc_factor;
            static constexpr const size_t extra = _config::vector_sbo_extra;
            static constexpr const size_t max_size = get_max_size();

            static void move(pointer to, pointer from, size_type size)
            {
                for (size_type i = 0; i < size; i++) {
                    __detail::move_construct_if_nothrow<T>::invoke(to, *from);
                }
            }

            static pointer allocate(allocator_type alloc, size_type size)
            {
                return alloc_traits::allocate(alloc, size);
            }

            static void destroy(allocator_type alloc, pointer ptr)
            {
                alloc_traits::destroy(alloc, ptr);
            }

            static void deallocate(allocator_type alloc, pointer ptr, size_type size)
            {
                alloc_traits::deallocate(alloc, ptr, size);
            }
        };
    }


	template<typename T, typename Allocator = allocator<T>>
	class vector : __detail::sbo_type<__detail::vector_sbo_config<T, Allocator>>
	{
		using base = __detail::sbo_type<__detail::vector_sbo_config<T, Allocator>>;

	public:
		using value_type = T;
		using allocator_type = Allocator;
		using size_type = size_t;
		using difference_type = ptrdiff_t;
		using reference = value_type&;
		using const_reference = const value_type&;
		using pointer = typename allocator_traits<Allocator>::pointer;
		using const_pointer = typename allocator_traits<Allocator>::const_pointer;
		using iterator = __detail::pointer_iterator<T>;
		using const_iterator = __detail::pointer_iterator<const T>;
		using reverse_iterator = std::reverse_iterator<iterator>;
		using const_reverse_iterator = std::reverse_iterator<const_iterator>;

	public:
		/**
		 * Default vector constructor.
		 */
		vector() noexcept(noexcept(Allocator()))
			: vector(Allocator()) { }

		/**
		 * Constructs a new vector with the given allocator.
		 *
		 * @param alloc The allocator to use.
		 */
		explicit vector(const Allocator& alloc) noexcept
			: base({}, alloc) { }

		/**
		 * Constructs a new vector with count copies of the given value.
		 *
		 * @param count The number of copies with which to fill the vector.
		 * @param value The value with which to fill the vector.
		 * @param alloc The allocator to use.
		 */
		explicit vector(size_type count, const T& value,
			const Allocator& alloc = Allocator());

		/**
		 * Constructs a new vector with count default-constructed elements.
		 *
		 * @param count The number of initial elements.
		 * @param alloc The allocator to use.
		 */
		explicit vector(size_type count, const Allocator& alloc = Allocator());

		/**
		 * Constructs a new vector with elements copied from the given range.
		 *
		 * @tparam InputIterator The type of the iterators.
		 * @param first The start of the range of elements to copy.
		 * @param last The end of the range of elements to copy.
		 * @param alloc The allocator to use.
		 */
		template<typename InputIterator, class=enable_if_t<
			__detail::is_input_iterator<InputIterator>()>>
		vector(InputIterator first, InputIterator last,
			const Allocator& alloc = Allocator());

		/**
		 * Constructs a new vector with elements copied from the given vector.
		 *
		 * @param other The vector from which to copy elements.
		 * @param alloc The allocator.
		 */
		vector(const vector& other, const Allocator& alloc);

		/**
		 * Constructs a new vector with elements and allocator copied from the given
		 * vector.
		 *
		 * @param other The vector from which to copy elements and allocator.
		 */
		vector(const vector& other) : vector(other, allocator_traits<Allocator>::
			select_on_container_copy_construction(other.get_allocator())) { }

		/**
		 * Move-constructs a new vector from the given vector.
		 *
		 * @param other The vector from which to move-construct.
		 */
		vector(vector&& other) noexcept;

		/**
		 * Move-constructs a new vector from the given vector.
		 *
		 * @param other The vector from which to move-construct.
		 * @param alloc The allocator to use.
		 */
		vector(vector&& other, const Allocator& alloc);

		/**
		 * Constructs a new vector with the given list of elements.
		 *
		 * @param elements The initial contents of the vector.
		 * @param alloc The allocator to use.
		 */
		vector(initializer_list<T> elements, const Allocator& alloc = Allocator());

		/**
		 * Copies and replaces this vector's elements with those of the given vector.
		 *
		 * @param other The vector from which to copy.
		 * @return vector& This vector.
		 */
		vector& operator =(const vector& other);

		/**
		 * Replaces this vector's elements with those of the given vector.
		 * @param other The vector from which to move the elements.
		 * @return vector& This vector.
		 */
		vector& operator =(vector&& other) noexcept(
			allocator_traits<Allocator>::propagate_on_container_move_assignment::value ||
			allocator_traits<Allocator>::is_always_equal::value);

		/**
		 * Replaces this vector's elements with those of the given initializer list.
		 *
		 * @param items The items to replace the current contents.
		 * @return vector& This vector.
		 */
		vector& operator =(initializer_list<T> items);

		/**
		 * Replaces this vector's elements with \p count copies of the given value.
		 *
		 * @param count The number of copies of \p value with which to fill this vector.
		 * @param value The value to copy.
		 */
		void assign(size_type count, const T& value);

		/**
		 * Replaces this vector's elements with copies of those in the given range.
		 *
		 * @tparam InputIter The type of the iterators.
		 * @param first The start of the range to copy.
		 * @param last The end of the range to copy.
		 */
		template<typename InputIter, class=enable_if_t<__detail::is_input_iterator<InputIter>>>
		void assign(InputIter first, InputIter last);

		/**
		 * Replaces this vector's elements with those of the given initializer list.
		 *
		 * @param items The items to replace the current contents.
		 */
		void assign(initializer_list<T> items);

		/**
		 * Gets the allocator object used by this container.
		 *
		 * @return allocator_type The allocator used by this container.
		 */
		allocator_type get_allocator() const;

		/**
		 * Returns a reference to the element at the given index.
		 *
		 * @throws std::out_of_range Index is outside the bounds of the vector.
		 * @param index The index of the element to reference.
		 * @return reference A reference to the element at the given index.
		 */
		reference at(size_type index);
		/**
		 * Returns a reference to the element at the given index.
		 *
		 * @throws std::out_of_range Index is outside the bounds of the vector.
		 * @param index The index of the element to reference.
		 * @return const_reference A reference to the element at the given index.
		 */
		const_reference at(size_type index) const;

		/**
		 * Returns a reference to the element at the given index.
		 *
		 * @param index The index of the element to reference.
		 * @return reference A reference to the element at the given index.
		 */
		reference operator[](size_type index);
		/**
		 * Returns a reference to the element at the given index.
		 *
		 * @param index The index of the element to reference.
		 * @return const_reference A reference to the element at the given index.
		 */
		const_reference operator[](size_type index) const;

		/**
		 * @brief Returns a reference to the first element.
		 * Behaviour is undefined if the vector is empty.
		 *
		 * @return reference Reference to the first element.
		 */
		reference front();
		/**
		 * @brief Returns a reference to the first element.
		 * Behaviour is undefined if the vector is empty.
		 *
		 * @return reference Reference to the first element.
		 */
		const_reference front() const;

		/**
		 * @brief Returns a reference to the final element.
		 * Behaviour is undefined if the vector is empty.
		 *
		 * @return reference Reference to the final element.
		 */
		reference back();
		/**
		 * @brief Returns a reference to the final element.
		 * Behaviour is undefined if the vector is empty.
		 *
		 * @return reference Reference to the final element.
		 */
		const_reference back() const;

		/**
		 * @brief Gets the underlying data array.
		 *
		 * This array is suitable to use in place of a C array. However,
		 * subsequent modification (push/pop) of the vector may trigger a
		 * re-allocation, which could render the pointer returned by a previous
		 * call to data() invalid.
		 *
		 * @return T* Pointer to the underlying array.
		 */
		T* data() noexcept;
		/**
		 * @brief Gets the underlying data array.
		 *
		 * This array is suitable to use in place of a C array. However,
		 * subsequent modification (push/pop) of the vector may trigger a
		 * re-allocation, which could render the pointer returned by a previous
		 * call to data() invalid.
		 *
		 * @return T* Pointer to the underlying array.
		 */
		const T* data() const noexcept;

		/**
		 * @brief Gets if the vector does not contain any elements.
		 *
		 * @return true The vector is empty.
		 * @return false The vector contains one or more elements.
		 */
		bool empty() const noexcept;

		/**
		 * @brief Gets the number of elements this vector contains.
		 *
		 * @return size_type The number of elements in this vector.
		 */
		size_type size() const noexcept;

		/**
		 * @brief Gets the maximum capacity of any vector.
		 *
		 * Heap space availability may limit the capacity of this vector further.
		 *
		 * @return size_type The maximum capacity of any one vector.
		 */
		size_type max_size() const noexcept;

		/**
		 * @brief Pre-allocates the memory necessary to hold
		 *        the given number of elements.
		 *
		 * Where calls to this function result in re-allocation,
		 * existing references to elements, points to the underlying
		 * array or iterators may become invalid.
		 *
		 * @throws std::length_error if the requested capacity
		 *         exceeds the value returned by max_size().
		 * @param new_capacity The minimum number of elements for which
		 *                     to pre-allocate memory for their storage.
		 */
		void reserve(size_type new_capacity);

		/**
		 * @brief Gets the number of elements the vector can currently
		 *        hold without requiring a re-allocation.
		 *
		 * If this number of elements is exceeded, a re-allocation will
		 * be triggered, which can invalidate existing iterators and
		 * element references.
		 *
		 */
		size_type capacity() const noexcept;

		/**
		 * @brief Requests reduction of this vector's capacity
		 *        to match the space required.
		 *
		 * This function is typically called following copy/move-assignment
		 * or where elements have been removed from the vector to free unused
		 * space.
		 *
		 * Calls to this function may result in re-allocation, which can invalidate
		 * existing iterators and element references.
		 */
		void shrink_to_fit();

		/**
		 * @brief Removes all elements from the vector.
		 *
		 * To subsequently free unused memory, call shrink_to_fit().
		 */
		void clear() noexcept;

		/**
		 * @brief Inserts the given item at the end of the vector.
		 *
		 * Where the current capacity is exceeded, a re-allocation may
		 * be performed which can invalid existing iterators and element references
		 *
		 * @param value The item to insert.
		 */
		void push_back(const T& value);

		/**
		 * @brief Inserts the given item at the end of the vector.
		 *
		 * Where the current capacity is exceeded, a re-allocation may
		 * be performed which can invalid existing iterators and element references
		 *
		 * @param value The item to insert.
		 */
		void push_back(T&& value);

	private:
		template<typename InputIterator, class=enable_if_t<
			__detail::is_input_iterator<InputIterator>()>>
		vector(InputIterator first, InputIterator last, size_type size,
			const Allocator& alloc = Allocator());
	};


	template<typename T, typename A>
	vector<T, A>::vector(size_type count, const T& value, const A& alloc)
		: base({}, alloc)
	{
		base::_reallocate(count);
		base::_set_size(count);
		for (size_t i = 0; i < count; i++) {
			new (static_cast<void*>(base::_data() + i)) T(value);
		}
	}

	template<typename T, typename A>
	vector<T, A>::vector(size_type count, const A& alloc)
		: base({}, alloc)
	{
		base::_reallocate(count);
		base::_set_size(count);
		new (static_cast<void*>(base::_data())) T[count]();
	}

	template<typename T, typename A>
	template<typename InputIter, class>
	vector<T, A>::vector(InputIter first, InputIter last, const A& alloc)
		: vector(first, last, distance(first, last), alloc)
	{
	}

	template<typename T, typename A>
	vector<T,A>::vector(const vector<T,A>& other, const A& alloc)
		: base({}, alloc)
	{
		base::_reallocate(other.size());
		base::_set_size(other.size());

		for (size_type i = 0; i < other.size(); i++) {
			new (static_cast<void*>(base::_data() + i)) T(other.data()[i]);
		}
	}

	template<typename T, typename A>
	vector<T,A>::vector(vector<T,A>&& other, const A& alloc)
		: base({}, alloc)
	{
		base::_reallocate(other.size());
		base::_set_size(other.size());

		for (size_type i = 0; i < other.size(); i++) {
			__detail::move_construct_if_nothrow<T>(base::_data() + i, other.data()[i]);
		}
	}

	template<typename T, typename A>
	vector<T,A>::vector(vector<T,A>&& other) : vector(reinterpret_cast<T&&>(other), A())
	{
		base::_reallocate(other.size());
		base::_set_size(other.size());

		for (size_type i = 0; i < other.size(); i++) {
			__detail::move_construct_if_nothrow<T>(base::_data() + i, other.data()[i]);
		}
	}

	template<typename T, typename A>
	vector<T,A>::vector(initializer_list<T> elements, const A& alloc)
		: vector(elements.begin(), elements.end(), elements.size(), alloc)
	{
	}

	template<typename T, typename A>
	template<typename InputIterator, class>
	vector<T, A>::vector(InputIterator first, InputIterator last, size_type size,
		const A& alloc) : base({}, alloc)
	{
		// Allocate and copy vector items
		base::_reallocate(size);
		base::_set_size(size);

		// TODO: forward if no-throw
		for (size_t i = 0; i < size; i++, first++) {
			new (static_cast<void*>(base::_data() + i)) T(*first);
		}
	}

	template<typename T, typename A>
	vector<T, A>& vector<T, A>::operator =(const vector<T, A>& other)
	{
		base::_destroy();
		if (other.size() > base::_size()) {
			base::_reallocate(other.size());
		}
		for (size_t i = 0; i < other.size(); i++) {
			new (static_cast<void*>(base::_data() + i)) T(other[i]);
		}
		base::_set_size(other.size());
	}

	template<typename T, typename A>
	typename vector<T, A>::allocator_type vector<T, A>::get_allocator() const
	{
		return base::_alloc;
	}

	template<typename T, typename A>
	typename vector<T, A>::reference& vector<T, A>::at(size_type index)
	{
		if (index < 0 || index >= base::_size()) {
			__abi::__throw_exception(std::out_of_range("index"));
		}
		else return base::_data()[index];
	}

	template<typename T, typename A>
	typename vector<T, A>::reference vector<T, A>::operator[](size_type index)
	{
		return base::_data()[index];
	}

	template<typename T, typename A>
	typename vector<T, A>::const_reference vector<T, A>::operator[](size_type index) const
	{
		return base::_data()[index];
	}

	template<typename T, typename A>
	typename vector<T, A>::reference vector<T, A>::front()
	{
		return base::_data()[0];
	}

	template<typename T, typename A>
	typename vector<T, A>::const_reference vector<T, A>::front() const
	{
		return base::_data()[0];
	}

	template<typename T, typename A>
	typename vector<T, A>::reference vector<T, A>::back()
	{
		return base::_data()[base::_size() - 1];
	}

	template<typename T, typename A>
	typename vector<T, A>::const_reference vector<T, A>::back() const
	{
		return base::_data()[base::_size() - 1];
	}

	template<typename T, typename A>
	T* vector<T, A>::data() noexcept
	{
		return base::_data();
	}

	template<typename T, typename A>
	const T* vector<T, A>::data() const noexcept
	{
		return base::_data();
	}

	template<typename T, typename A>
	bool vector<T, A>::empty() const noexcept
	{
		return base::_size() == 0;
	}

	template<typename T, typename A>
	typename vector<T, A>::size_type vector<T, A>::size() const noexcept
	{
		return base::_size();
	}

	template<typename T, typename A>
	typename vector<T, A>::size_type vector<T, A>::max_size() const noexcept
	{
		return base::max_size;
	}

	template<typename T, typename A>
	void vector<T, A>::reserve(size_type new_capacity)
	{
		if (new_capacity > base::_capacity()) {
			base::_reallocate(new_capacity);
		}
	}

	template<typename T, typename A>
	typename vector<T, A>::size_type vector<T, A>::capacity() const noexcept
	{
		return base::_capacity();
	}

	template<typename T, typename A>
	void vector<T, A>::shrink_to_fit()
	{
		base::_reallocate(base::_size());
	}

	template<typename T, typename A>
	void vector<T, A>::clear() noexcept
	{
		base::_destroy();
		base::_set_size(0);
	}

	template<typename T, typename A>
	void vector<T, A>::push_back(const T& value)
	{
		reserve(size() + 1);
		new (static_cast<void*>(data())) T(value);
		base::_set_size(size() + 1);
	}

	template<typename T, typename A>
	void vector<T, A>::push_back(T&& value)
	{
		reserve(size() + 1);
		new (static_cast<void*>(data())) T(reinterpret_cast<T&&>(value));
		base::_set_size(size() + 1);
	}
}
