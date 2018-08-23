/* tuple.hpp - (c) 2017 James S Renwick */

#pragma once
#include <utility>
#include <type_traits>
#include <cstddef>

extern "C++" {
namespace std
{
	namespace __detail
	{
		template<size_t Index, typename... Ts>
		struct type_at { };
		template<typename T, typename... Ts>
		struct type_at<0, T, Ts...> {
			using type = T;
		};
		template<size_t Index, typename T, typename... Ts>
		struct type_at<Index, T, Ts...> {
			using type = typename type_at<Index - 1, Ts...>::type;
		};

		struct ignore
		{
			constexpr ignore() noexcept = default;
			constexpr ignore(const ignore&) noexcept = default;
			constexpr ignore(ignore&&) noexcept = default;

			template<typename T>
			constexpr ignore(T&) noexcept { }
			template<typename T>
			constexpr ignore& operator=(T&&) noexcept { return *this; }
			template<typename T>
			constexpr const ignore& operator=(T&&) const noexcept { return *this; }
		};


		template <typename T, size_t index>
		struct tuple_item
		{
			T item;
			constexpr tuple_item() = default;
			template<typename Y>
			constexpr tuple_item(Y&& value) : item(forward<Y>(value)) { }
		};

		template <size_t index>
		struct tuple_item<ignore&, index>
		{
			ignore item;
			constexpr tuple_item() = default;

			template<typename Y>
			constexpr tuple_item(Y&&) { }
		};

	}

	template<typename ...Ts>
	class tuple;

	template<size_t Index, typename T>
	struct tuple_element;

	template<size_t Index, typename ...Ts>
	struct tuple_element<Index, tuple<Ts...>>
		: __detail::type_at<Index, Ts...> { };

	template<size_t Index, typename T>
	using tuple_element_t = typename tuple_element<Index, T>::type;

	template<size_t Index, typename ...Ts>
	struct tuple_element<Index, const tuple<Ts...>> {
		using type = typename std::add_const<tuple_element_t<Index, tuple<Ts...>>>::type;
	};
	template<size_t Index, typename ...Ts>
	struct tuple_element<Index, volatile tuple<Ts...>> {
		using type = typename std::add_volatile<tuple_element_t<Index, tuple<Ts...>>>::type;
	};
	template<size_t Index, typename ...Ts>
	struct tuple_element<Index, const volatile tuple<Ts...>> {
		using type = typename std::add_cv<tuple_element_t<Index, tuple<Ts...>>>::type;
	};

	template<size_t Index, typename... Ts>
	constexpr tuple_element_t<Index, tuple<Ts...>>& get(tuple<Ts...>& t) noexcept {
		return static_cast<__detail::tuple_item<tuple_element_t<Index, tuple<Ts...>>, Index>&>(t).item;
	}
	template<size_t Index, typename... Ts>
	constexpr tuple_element_t<Index, tuple<Ts...>>&& get(tuple<Ts...>&& t) noexcept {
		return static_cast<__detail::tuple_item<tuple_element_t<Index, tuple<Ts...>>, Index>&&>(move(t)).item;
	}
	template<size_t Index, typename... Ts>
	constexpr const tuple_element_t<Index, tuple<Ts...>>& get(const tuple<Ts...>& t) noexcept {
		return static_cast<const __detail::tuple_item<tuple_element_t<Index, tuple<Ts...>>, Index>&>(t).item;
	}
	template<size_t Index, typename... Ts>
	constexpr const tuple_element_t<Index, tuple<Ts...>>&& get(const tuple<Ts...>&& t) noexcept {
		return static_cast<const __detail::tuple_item<tuple_element_t<Index, tuple<Ts...>>, Index>&&>(move(t)).item;
	}


	template<size_t Index, typename... Ys>
	constexpr const tuple_element_t<Index, tuple<Ys...>>& get(const tuple<Ys...>& t) noexcept;
	template<size_t Index, typename... Ys>
	constexpr tuple_element_t<Index, tuple<Ys...>>&& get(tuple<Ys...>&& t) noexcept;



	namespace __detail
	{
		template <typename, typename... Ts_>
		struct tuple_base;

		template<size_t... Is, typename... Ts>
		struct tuple_base <index_sequence<Is...>, Ts...> : protected tuple_item<Ts, Is>...
		{
			constexpr tuple_base() = default;

			template<typename ...Ys>
			constexpr tuple_base(const tuple<Ys...>& t) : tuple_item<Ts, Is>(get<Is>(t))... { }
			template<typename ...Ys>
			constexpr tuple_base(tuple<Ys...>&& t) : tuple_item<Ts, Is>(get<Is>(t))... { }

			template<typename ...Ys>
			constexpr tuple_base(const Ys&... values) : tuple_item<Ts, Is>(values)... { }
			template<typename ...Ys>
			tuple_base(Ys&&... values) : tuple_item<Ts, Is>(forward<Ys>(values))... { }
		};
	}


	template<typename... Ts>
	class tuple : private __detail::tuple_base<make_index_sequence<sizeof...(Ts)>, Ts...>
	{
		template<size_t Index, typename... Ys>
		friend constexpr tuple_element_t<Index, tuple<Ys...>>& get(tuple<Ys...>& t) noexcept;
		template<size_t Index, typename... Ys>
		friend constexpr tuple_element_t<Index, tuple<Ys...>>&& get(tuple<Ys...>&& t) noexcept;
		template<size_t Index, typename... Ys>
		friend constexpr const tuple_element_t<Index, tuple<Ys...>>& get(const tuple<Ys...>& t) noexcept;
		template<size_t Index, typename... Ys>
		friend constexpr const tuple_element_t<Index, tuple<Ys...>>&& get(const tuple<Ys...>&& t) noexcept;

		template<size_t I, typename... Ys>
		struct copy_from {
			constexpr void operator()(tuple<Ts...>& self, const tuple<Ys...>& other) {
				get<I-1>(self) = get<I-1>(other);
				copy_from<I - 1, Ys...>()(self, other);
			}
		};
		template<typename... Ys>
		struct copy_from<0, Ys...> {
			constexpr void operator()(tuple<Ts...>&, const tuple<Ys...>&) { }
		};
		template<typename ...Ys>
		constexpr auto copy_from_f(const tuple<Ys...>& other) {
			return copy_from<sizeof...(Ys), Ys...>()(*this, other);
		}

		template<size_t I, typename... Ys>
		struct move_from {
			constexpr void operator()(tuple<Ts...>& self, tuple<Ys...>&& other) {
				get<I-1>(self) = move(get<I-1>(other));
				move_from<I - 1, Ys...>()(self, move(other));
			}
		};
		template<typename... Ys>
		struct move_from<0, Ys...> {
			constexpr void operator()(tuple<Ts...>&, tuple<Ys...>&&) { }
		};
		template<typename ...Ys>
		constexpr auto move_from_f(tuple<Ys...>&& other) {
			return move_from<sizeof...(Ys), Ys...>()(*this, move(other));
		}


		constexpr void copy_from_f() { }

		template<typename Y, typename... Ys>
		constexpr void copy_from_f(const Y& value, const Ys&... ys) {
			get<sizeof...(Ys)>(*this) = value; copy_from_f(ys...);
		}

		constexpr void move_from_f() { }

		template<typename Y, typename... Ys>
		constexpr void move_from_f(Y&& value, Ys&&... values) {
			get<sizeof...(Ys)>(*this) = forward<Y>(value);
			move_from_f(values...);
		}

		struct _ {};
		struct default_ctor { };
		struct direct_ctor { };

		template<typename, typename = _>
		struct requirements;

		template<typename _>
		struct requirements<default_ctor, _>
		{
			static constexpr const bool values_copy_ctor = sizeof...(Ts) > 0 &&
				__detail::for_all<is_copy_constructible, Ts...>::value;
		};

	public:
		constexpr tuple() = default;

		template<class _=direct_ctor, class=enable_if_t<requirements<_>::value>>
		explicit constexpr tuple(const Ts&... values) :
			__detail::tuple_base<make_index_sequence<sizeof...(Ts)>, Ts...>(values...) { }

		template<typename... Ys, class=enable_if_t<sizeof...(Ys) != 0 && sizeof...(Ys) == sizeof...(Ts)>>
		explicit constexpr tuple(Ys&&... values)
			: __detail::tuple_base<make_index_sequence<sizeof...(Ts)>, Ts...>(forward<Ys>(values)...) { }

		template<typename... Ys>
		constexpr tuple(const tuple<Ys...>& other)
			: __detail::tuple_base<make_index_sequence<sizeof...(Ts)>, Ts...>(other) { }

		template<typename... Ys>
		constexpr tuple(tuple<Ys...>&& other)
			: __detail::tuple_base<make_index_sequence<sizeof...(Ts)>, Ts...>(move(other)) { }

		//template<typename Y, typename P>
		//constexpr tuple(const std::pair<Y, P>& pair);
		//template<typename Y, typename P>
		//constexpr tuple(std::pair<Y, P>&& pair);

		constexpr tuple(const tuple&) = default;
		constexpr tuple(tuple&&) = default;

		constexpr tuple& operator=(const tuple& other) {
			copy_from_f(other); return *this;
		}
		constexpr tuple& operator=(tuple&& other) {
			move_from_f(move(other)); return *this;
		}

		template<typename... Ys>
		constexpr tuple& operator=(const tuple& other) {
			copy_from_f(other); return *this;
		}
		template<typename... Ys>
		constexpr tuple& operator=(tuple&& other) {
			move_from_f(other); return *this;
		}
	};




	template<typename T>
	struct tuple_size;

	template<typename ...Ts>
	struct tuple_size<tuple<Ts...>>
		: integral_constant<size_t, sizeof...(Ts)> { };

#if defined(__cpp_variable_templates)
    template<typename... Ts>
    constexpr size_t tuple_size_v = tuple_size<Ts...>::value;
#endif

	template<typename T>
	class reference_wrapper;

    namespace __detail
	{
        template <typename Func, typename Tuple, size_t... I>
        constexpr decltype(auto) _apply(Func &&f, Tuple &&t, index_sequence<I...>) {
            return f(get<I>(t)...);
        }

		template<typename T>
		struct reference_unwrapper { using type = T; };
		template<typename T>
		struct reference_unwrapper<reference_wrapper<T>> { using type = T&; };
    }

    template <class Func, class Tuple>
    constexpr decltype(auto) apply(Func&& func, Tuple&& tuple)
	{
        return __detail::_apply(forward<Func>(func), forward<Tuple>(tuple), 
			make_index_sequence<tuple_size<decay_t<Tuple>>::value>());
    }

	template<typename ...Ts>
	constexpr tuple<typename __detail::reference_unwrapper<decay_t<Ts>>::type...> 
		make_tuple(Ts&&... args)
	{
		return tuple<typename __detail::reference_unwrapper<decay_t<Ts>>::type...>(
			forward<Ts>(args)...);
	}

	template<typename ...Ts>
	constexpr tuple<Ts&&...> forward_as_tuple(Ts&&... args) noexcept {
		return tuple<Ts&&...>(forward<Ts>(args)...);
	}

	extern __detail::ignore ignore;

	template<typename ...Ts>
	tuple<Ts&...> tie(Ts&... args) noexcept
	{
		return tuple<Ts&...>(args...);
	}



#ifdef __cpp_deduction_guides
	template<typename ...Ts>
	tuple(Ts...) -> tuple<Ts...>;

	//template<typename T1, typename T2>
	//tuple(pair<T1, T2>) -> tuple<T1, T2>;

	//template<typename Alloc, typename ...Ts>
	//tuple(allocator_arg_t, Alloc, Ts...) -> tuple<Ts...>;

	//template<typename Alloc, typename T1, typename T2>
	//tuple(allocator_arg_t, Alloc, pair<T1, T2>) -> tuple<T1, T2>;

	//template<typename Alloc, typename ...Ts>
	//tuple(allocator_arg_t, Alloc, tuple<Ts...>) -> tuple<Ts...>;
#endif
}
}
