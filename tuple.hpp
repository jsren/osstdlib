/* tuple.hpp - (c) 2017 James S Renwick */

#pragma once
#include "_feats.hpp"
#include "meta.hpp"


namespace std
{



    namespace _internal
    {
        template<size_t Index, typename... Ts>
        struct _TypeAtIndex {
        };
        template<typename... Ts>
        struct _TypeAtIndex<0, Ts...> {
        };
        template<size_t Index, typename T, typename... Ts>
        struct _TypeAtIndex<Index, T, Ts...> {
            using type = typename _TypeAtIndex<Index - 1, Ts...>::type;
        };
        template<typename T, typename... Ts>
        struct _TypeAtIndex<0, T, Ts...> {
            using type = T;
        };


        template <typename T, size_t index>
        struct _TupleItem { T item; };

        template <typename, typename... Ts_>
        struct _TupleBase;

        template<size_t... Is, typename... Ts>
        class _TupleBase <index_sequence<Is...>, Ts...> : protected _TupleItem<Ts, Is>... { };
    }

    template<size_t Index, typename... Ts>
    using TypeAtIndex = typename _internal::_TypeAtIndex<Index, Ts...>::type;

    template<typename... Ts>
    struct Tuple : _internal::_TupleBase<make_index_sequence<sizeof...(Ts)>, Ts...>
    {
        template<size_t Index, typename... Ys>
        friend TypeAtIndex<Index, Ys...>& get(Tuple<Ys...>&);

        static constexpr const size_t itemCount = sizeof...(Ts);
    };

    template<size_t Index, typename... Ts>
    inline TypeAtIndex<Index, Ts...>& get(std::Tuple<Ts...>& tuple) {
        return static_cast<_internal::_TupleItem<TypeAtIndex<Index, Ts...>, Index>*>(&tuple)->item;
    }

    namespace _internal {
        template<template<class...> class Tuple, typename Func, typename... Ts, size_t... Is>
        inline void _for_each_helper(Tuple<Ts...>& tuple, Func&& func, index_sequence<Is...>) {
            int _[] = { 0, ((void)(func(get<Is>(tuple))), 0)... };
            (void)_;
        }
    }

    template<template<class...> class Tuple, typename Func, typename... Ts>
    inline void for_each(Tuple<Ts...>& tuple, Func&& func) {
        _internal::_for_each_helper(tuple, static_cast<Func&&>(func), make_index_sequence<sizeof...(Ts)>());
    }

    template<typename... Ts>
    struct _tuple_size { };
    template<typename... Ts>
    struct _tuple_size<Tuple<Ts...>> { constexpr static const size_t value = Tuple<Ts...>::itemCount; };

#if defined(__cpp_variable_templates)
    template<typename... Ts>
    constexpr size_t tuple_size = _tuple_size<Ts...>::value;
#else
    template<typename... Ts>
    using tuple_size = _tuple_size<Ts...>;
#endif


    namespace _internal {
        template <class F, class Tuple, size_t... I>
        constexpr __STD_DECLTYPE_AUTO _apply(F &&f, Tuple &&t, index_sequence<I...>) {
            return f(get<I>(t)...);
        }
    }

#if defined(__cpp_variable_templates)
    template <class Func, class Tuple>
    constexpr __STD_DECLTYPE_AUTO apply(Func&& func, Tuple&& tuple) {
        return _internal::_apply(func, tuple, make_index_sequence<tuple_size<remove_reference_t<Tuple>>>{});
    }
#else
    template <class Func, class Tuple>
    constexpr __STD_DECLTYPE_AUTO apply(Func&& func, Tuple&& tuple) {
        return _internal::_apply(func, tuple, make_index_sequence<tuple_size<remove_reference_t<Tuple>>::value>{});
    }
#endif

}
