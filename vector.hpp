/* vector - (c) 2018 James Renwick */
#pragma once

#include <allocator>
#include <cstddef>
#include <memory>
#include <__iterator>
#include <initializer_list>

extern "C++" {
namespace std
{
	template<typename T, typename Allocator = allocator<T>>
	class vector
	{
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
		using reverse_iterator = reverse_iterator<iterator>;
		using const_reverse_iterator = reverse_iterator<const_iterator>;

		/**
		 * Default vector constructor.
		 */
		vector() noexcept(noexcept(Allocator())
			: vector(Allocator()) { }

		/**
		 * Constructs a new vector with the given allocator.
		 *
		 * @param alloc The allocator to use.
		 */
		explicit vector(const Allocator& alloc) noexcept;

		/**
		 * Constructs a new vector with \p count copies of the given value.
		 *
		 * @param count The number of copies with which to fill the vector.
		 * @param value The value with which to fill the vector.
		 * @param alloc The allocator to use.
		 */
		explicit vector(size_type count, const T& value,
			const Allocator& alloc = Allocator());

		/**
		 * Constructs a new vector with \p count default-constructed elements.
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
		template<typename InputIter, class=enable_if_t<__detail::is_input_iterator<InputIter>>
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
	};
}
}
