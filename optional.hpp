/* optional.hpp - (c) 2018 James Renwick */
#pragma once
#include <initializer_list>
#include <type_traits>
#include <utility>
#include <new>

namespace std
{
    struct nullopt_t
    {
        explicit constexpr nullopt_t(int) noexcept { }
    };

#if __cpp_inline_variables
    inline constexpr nullopt_t nullopt{0};
#endif

    namespace __detail
    {
        struct empty {};
    
        template<typename T>
        struct destroy
        {
            ~destroy()
            {
                auto& optional = *static_cast<T*>(this);
                if (optional.has_value())
                {
                    using Y = typename T::value_type;
                    optional.value().~Y();
                }
            }
        };
    }


    template<typename T>
    class optional :
        // Only provide destructor if non-trivial
        conditional_t<is_default_constructible<T>::value,
                      __detail::empty,
                      __detail::destroy<optional<T>>>
    {
    public:
        using value_type = T;

    private:
        union {
            __detail::empty _;
            T _value;
        } _data{};

        bool _has_value{};

        template<typename Y>
        constexpr void assign_value(Y&& y)
        {
            if (_has_value) {
                _data._value = forward<Y>(y);
            }
            else {
                using E = __detail::empty;
                _data._.~E();
                new (&_data._value) T(forward<Y>(y));
            }
            _has_value = true;
        }

        template<typename ...Args>
        constexpr void emplace_value(Args&&... args)
        {
            if (_has_value) {
                _data._value.~T();
            }
            else {
                using E = __detail::empty;
                _data._.~E();
            }
            new ((void*)&_data._value) T(forward<Args>(args)...);
            _has_value = true;
        }

        constexpr void clear_value()
        {
            if (_has_value) {
                // try {
                _data._value.~T();
                // } catch (...) { }
                new ((void*)&_data._value) __detail::empty{};
            }
        }

    public:
        /**
         * Constructs an optional without a value.
         */
        constexpr optional() noexcept = default;
        /**
         * Constructs an optional without a value.
         */
        constexpr optional(nullopt_t) noexcept {};
        /**
         * Copy-constructs an optional from the given value.
         * @param other The value to copy.
         */
        //template<class Y=T, class=enable_if_t<is_copy_constructible<Y>::value>>
        constexpr optional(const optional& other) = default;
        /**
         * Move-constructs an optional from the given optional.
         * @param other The optional to move.
         */
        //template<class Y=T, class=enable_if_t<is_move_constructible<Y>::value>>
        constexpr optional(optional&& other)
            noexcept(noexcept(is_nothrow_move_constructible<T>::value)) = default;
        /**
         * Copy-constructs an optional from the given value.
         * @param other The value to copy.
         */
        template<typename Y>
        constexpr optional(const optional<Y>& other);
        /**
         * Move-constructs an optional from the given optional.
         * @param other The optional to move.
         */
        template<typename Y>
        constexpr optional(optional<Y>&&);
        /**
         * Constructs a value in-place within an optional using the given
         * constructor arguments.
         *
         * @param args The arguments with which to construct the value of this
         *             optional.
         */
        template<typename ...Args>
        constexpr explicit optional(in_place_t, Args&&... args);
        /**
         * Constructs a value in-place within an optional using the given
         * constructor arguments.
         *
         * @param arg The first argument, where that argument is an
                      initializer_list.
         * @param args The arguments with which to construct the value of this
         *             optional.
         */
        template<typename Arg, typename ...Args>
        constexpr explicit optional(in_place_t, initializer_list<Arg> arg, Args&&... args);
        /**
         * Move-constructs an optional with the given value.
         * @param other The value to move.
         */
        template<typename Y>
        constexpr optional(Y&& value);

        /**
         * Assigns the optional to be empty.
         * Destroys any existing value.
         */
        optional& operator =(nullopt_t) noexcept;
        /**
         * Copy-assigns this optional with the given optional.
         * Destroys any existing value.
         *
         * @param other The optional to assign to this optiona
         */
        optional& operator =(const optional& other) = default;
        /**
         * Move-assigns this optional with the given optional.
         * Destroys any existing value.
         *
         * @param other The optional to assign to this optional.
         */
        //template<class Y=T, class=enable_if_t<is_move_constructible<Y>::value && is_move_assignable<Y, Y>::value>>
        optional& operator =(optional&& other)
            noexcept(noexcept(is_nothrow_move_constructible<T>::value &&
                     is_nothrow_move_assignable<T, T>::value)) = default;
        /**
         * Move-assigns this optional with the given value.
         * Destroys any existing value.
         *
         * @param value The value to assign to this optional.
         */
        template<typename Y>
        optional& operator =(Y&& value);
        /**
         * Copy-assigns this optional with the given optional.
         * Destroys any existing value.
         *
         * @param other The optional to assign to this optional.
         */
        template<typename Y>
        optional& operator =(const optional<Y>& other);
        /**
         * Move-assigns this optional with the given optional.
         * Destroys any existing value.
         *
         * @param other The optional to assign to this optional.
         */
        template<typename Y>
        optional& operator =(optional<Y>&& other);

        constexpr T* operator ->();
        constexpr const T* operator ->() const;
        constexpr T& operator *() &;
        constexpr const T& operator *() const &;
        constexpr T&& operator *() &&;
        constexpr const T&& operator *() const &&;

        /**
         * Gets whether the optional contains a value.
         * @returns true if the optional contains a value,
         *          false otherwise.
         */
        constexpr explicit operator bool() const noexcept;
        /**
         * Gets whether the optional contains a value.
         * @returns true if the optional contains a value,
         *          false otherwise.
         */
        constexpr bool has_value() const noexcept;
        /**
         * Gets the value in the optional.
         * If the optional doesn't have a value, behaviour
         * is undefined.
         * 
         * @returns The value in the optional.
         */
        constexpr T& value() &;
        /**
         * Gets the value in the optional.
         * If the optional doesn't have a value, behaviour
         * is undefined.
         * 
         * @returns The value in the optional.
         */
        constexpr const T& value() const &;
        /**
         * Gets the value in the optional.
         * If the optional doesn't have a value, behaviour
         * is undefined.
         * 
         * @returns The value in the optional.
         */
        constexpr T&& value() &&;
        /**
         * Gets the value in the optional.
         * If the optional doesn't have a value, behaviour
         * is undefined.
         * 
         * @returns The value in the optional.
         */
        constexpr const T&& value() const &&;
        /**
         * Gets the value in the optional or the given
         * default if no value is present.
         *
         * @param default_value The value to return if no value is present.
         * @returns The value in the optional or the given default.
         */
        template<typename Y>
        constexpr T value_or(Y&& default_value) const &;
        /**
         * Gets the value in the optional or the given
         * default if no value is present.
         *
         * @param default_value The value to return if no value is present.
         * @returns The value in the optional or the given default.
         */
        template<typename Y>
        constexpr T value_or(Y&& default_value) &&;
        /**
         * Swaps the contents of this optional with the given optional.
         *
         * @param other The other with which to swap contents.
         */
        void swap(optional& other) noexcept(noexcept(
            is_nothrow_move_constructible<T>::value && is_nothrow_swappable<T>::value));
        /**
         * Clears the optional if it contains a vlaue.
         */
        void reset() noexcept;
        /**
         * Constructs a value in-place within the optional with the given
         * arguments.
         *
         * @param args The arguments with which to construct the value of this
         *             optional.
         */
        template<typename ...Args>
        T& emplace(Args&... args);
        /**
         * Constructs a value in-place within the optional with the given
         * arguments.
         *
         * @param arg The first argument, where that argument is an
                      initializer_list.
         * @param args The arguments with which to construct the value of this
         *             optional.
         */
        template<typename Arg, typename ...Args,
                 class=enable_if_t<is_constructible<T, initializer_list<Arg>, Args...>::value>>
        T& emplace(initializer_list<Arg> arg, Args&... args);
    };

    template<typename T>
    constexpr optional<decay_t<T>> make_optional(T&& value)
    {
        return optional<decay_t<T>>{forward<T>(value)};
    }

    template<typename T, typename ...Args>
    constexpr optional<decay_t<T>> make_optional(Args&&... args)
    {
        return optional<decay_t<T>>{in_place, forward<Args>(args)...};
    }

    template<typename T, typename Arg, typename ...Args>
    constexpr optional<decay_t<T>> make_optional(initializer_list<Arg> arg, Args&&... args)
    {
        return optional<decay_t<T>>{in_place, move(arg), forward<Args>(args)...};
    }

    template<typename T>
    template<typename Y>
    constexpr optional<T>::optional(const optional<Y>& other)
    {
        if (other._has_value) {
            emplace_value(other._data._value);
        }
    }

    template<typename T>
    template<typename Y>
    constexpr optional<T>::optional(optional<Y>&& other)
    {
        if (other._has_value) {
            emplace_value(move(other._data._value));
        }
    }

    template<typename T>
    template<typename ...Args>
    constexpr optional<T>::optional(in_place_t, Args&&... args)
    {
        emplace_value(forward<Args>(args)...);
    }

    template<typename T>
    template<typename Arg, typename ...Args>
    constexpr optional<T>::optional(in_place_t, initializer_list<Arg> arg, Args&&... args)
    {
        emplace_value(move(arg), forward<Args>(args)...);
    }

    template<typename T>
    template<typename Y>
    constexpr optional<T>::optional(Y&& value)
    {
        emplace_value(forward<Y>(value));
    }

    template<typename T>
    optional<T>& optional<T>::operator =(nullopt_t) noexcept
    {
        clear_value();
        return *this;
    }

    template<typename T>
    template<typename Y>
    optional<T>& optional<T>::operator =(Y&& value) noexcept
    {
        assign_value(forward<Y>(value));
        return *this;
    }

    template<typename T>
    template<typename Y>
    optional<T>& optional<T>::operator =(const optional<Y>& other)
    {
        if (&other == this) return *this;
        if (other._has_value) {
            assign_value(other._data._value);
        }
        return *this;
    }

    template<typename T>
    template<typename Y>
    optional<T>& optional<T>::operator =(optional<Y>&& other)
    {
        if (&other == this) return *this;
        if (other._has_value) {
            assign_value(move(other._data._value));
        }
        return *this;
    }

    template<typename T>
    constexpr T* optional<T>::operator ->()
    {
        return &_data._value;
    }

    template<typename T>
    constexpr const T* optional<T>::operator ->() const
    {
        return &_data._value;
    }

    template<typename T>
    constexpr T& optional<T>::operator *() &
    {
        return _data._value;
    }

    template<typename T>
    constexpr const T& optional<T>::operator *() const &
    {
        return _data._value;
    }

    template<typename T>
    constexpr T&& optional<T>::operator *() &&
    {
        return _data._value;
    }

    template<typename T>
    constexpr const T&& optional<T>::operator *() const &&
    {
        return _data._value;
    }

    template<typename T>
    constexpr optional<T>::operator bool() const noexcept
    {
        return _has_value;
    }

    template<typename T>
    constexpr bool optional<T>::has_value() const noexcept
    {
        return _has_value;
    }

    template<typename T>
    constexpr T& optional<T>::value() &
    {
        return _data._value;
    }

    template<typename T>
    constexpr const T& optional<T>::value() const &
    {
        return _data._value;
    }

    template<typename T>
    constexpr T&& optional<T>::value() &&
    {
        return move(_data._value);
    }

    template<typename T>
    constexpr const T&& optional<T>::value() const &&
    {
        return _data._value;
    }

    template<typename T>
    template<typename Y>
    constexpr T optional<T>::value_or(Y&& default_value) const &
    {
        return _has_value ? _data._value : static_cast<T>(move(default_value));
    }

    template<typename T>
    template<typename Y>
    constexpr T optional<T>::value_or(Y&& default_value) &&
    {
        return _has_value ? move(_data._value) : static_cast<T>(move(default_value));
    }

    template<typename T>
    void optional<T>::swap(optional<T>& other) noexcept(noexcept(
            is_nothrow_move_constructible<T>::value && is_nothrow_swappable<T>::value))
    {
        if (_has_value) {
            swap(_data._value, other._data._value);
        }
    }

    template<typename T>
    void optional<T>::reset() noexcept
    {
        clear_value();
    }

    template<typename T>
    template<typename ...Args>
    T& optional<T>::emplace(Args&... args)
    {
        emplace_value(forward<Args>(args)...);
        return *this;
    }

    template<typename T>
    template<typename Arg, typename ...Args, class>
    T& optional<T>::emplace(initializer_list<Arg> arg, Args&... args)
    {
        emplace_value(move(arg), forward<Args>(args)...);
        return *this;
    }
}
