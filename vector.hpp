#pragma once

#include <allocator>
#include <cstddef>
#include <memory>
#include <__iterator>

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

		vector() noexcept(noexcept(Allocator()) : vector(Allocator()) { }
		explicit vector(const Allocator& alloc) noexcept;

		explicit vector(size_type count, const T& value = T(),
			const Allocator& alloc = Allocator());

		explicit vector(size_type count, const Allocator& alloc = Allocator());

		template<typename InputIterator>
		vector(InputIterator first, InputIterator last,
			const Allocator& alloc = Allocator());

		vector(const vector& other, const Allocator& alloc);

		vector(const vector& other) : vector(other, allocator_traits<Allocator>::
			select_on_container_copy_construction(other.get_allocator())) { }

		vector(vector&& other) noexcept;

		vector(vector&& other, const Allocator& alloc);


		vector& operator =(const vector& other);
		vector& operator =(vector&& other) noexcept(
			allocator_traits<Allocator>::propagate_on_container_move_assignment::value ||
			allocator_traits<Allocator>::is_always_equal::value);
		vector& operator =(initializer_list<T> items);

		void assign(size_type count, const T& value);
		template<typename InputIter>
		void assign(InputIter first, InputIter last);
		void assign(initializer_list<T> items);
	};
}
