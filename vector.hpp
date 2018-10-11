/* vector - (c) 2018 James Renwick */
#pragma once

#include <allocator>
#include <cstddef>
#include <memory>
#include <__iterator>
#include <initializer_list>

namespace std
{
	template<typename T, typename Allocator = allocator<T>>
	class alignas(alignof(T) > alignof(size_t) ? alignof(T) : alignof(size_t))
		vector
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

	private:
		T* pointer{};
		size_t length{};

		struct long_vector_data
		{
			size_type _capacity;
			size_type _length;
			pointer _data;
		};

		static constexpr const size_type sbo_capacity =
			(sizeof(long_vector_data) - sizeof(uint8_t)) / sizeof(T);

		struct short_vector_data
		{
			uint8_t _length;
			aligned_storage_t<sbo_capacity, alignof(T)> _data;
		};

		static_assert(sizeof(short_vector_data) <= sizeof(long_vector_data),
			"Invalid sizing for Small-Buffer Optimisation");

	private:
		allocator_type _alloc{};
		union {
			long_vector_data _long;
			short_vector_data _short;
		} _repr{};

		constexpr bool is_sbo() const noexcept {
			return (_repr._long._capacity & 0b1) == 0b1;
		}

		constexpr size_type eval_capacity(size_type size) noexcept
		{
            size = ((size & 0b1) == 0) ? size : size + 1;
            if (size > max_size() || size == 0) {
                __abi::__throw_exception(length_error("size"));
            }
            return size;
		}

		void _set_size(size_type size) noexcept
		{
			if (is_sbo()) _repr._short._length = 0b1 | (size << 1);
			else _repr._long._length = size;
		}

	public:
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

	private:
		void _destroy(pointer data, size_type size, size_type capacity)
		{
			for (size_type i = 0; i < size; i++) {
				alloc_traits::destroy(_alloc, &data[i]);
			}
			alloc_traits::deallocate(_alloc, data, capacity);
		}

		void _destroy()
		{
			if (is_sbo()) {
				for (size_type i = 0; i < size() + 1; i++) {
					alloc_traits::destroy(_alloc, &data()[i]);
				}
			}
			else _destroy(data(), size() + 1, _capacity());
		}

        pointer _reallocate(size_type newCapacity)
        {
            pointer data;
            pointer prevData = this->data();
            size_type prevSize = size();
            size_type prevCapacity = _capacity();
            bool prevHeap = !is_sbo() && prevData != nullptr;
            size_type newSize = (prevSize + 1 > newCapacity) ?
                newCapacity - 1 : prevSize;

            // Short string
            if (newCapacity <= sbo_capacity)
            {
                _repr._short._length = 0b1 | (newSize << 1); // Set SbO flag
                data = reinterpret_cast<pointer>(&_repr._short._data);

                if (prevCapacity != 0)
                {
                    if (!prevHeap) // Destroy extra items if shrunk
                    {
                        for (size_type i = newSize; i < prevSize + 1; i++) {
                            prevData[i].~Char();
                        }
                    }
                    else // Copy items from heap
                    {
                        for (size_type i = 0; i < prevSize + 1; i++) {
                            prevData[i].~Char();
                        }
                    }
                }
            }
            // Long string
            else
            {
                newCapacity = eval_capacity(newCapacity);
                data = alloc_traits::allocate(_alloc, newCapacity);

                // Copy and destroy previous data
                traits_type::copy(data, prevData, prevSize < newSize ? prevSize : newSize);
                if (!prevHeap && prevCapacity != 0)
                {
                    for (size_type i = 0; i < prevSize + 1; i++) {
                        prevData[i].~Char();
                    }
                }
                this->_repr._long._length = newSize;
                this->_repr._long._capacity = newCapacity;
                this->_repr._long._data = data;
            }

            data[newSize] = '\0';

            if (prevHeap) {
                _destroy(prevData, prevSize, prevCapacity);
                alloc_traits::deallocate(_alloc, prevData, prevCapacity);
            }
            return data;
        }

	};
}
