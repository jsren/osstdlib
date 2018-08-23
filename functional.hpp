#pragma once

#include <type_traits>
#include <cstdint>

extern "C++" {
namespace std
{
    template<typename T>
    class reference_wrapper
    {
    private:
        T* pointer;

    public:
        using type = T;

        reference_wrapper(T& reference) noexcept
            : pointer(&reference) { }

        reference_wrapper(const reference_wrapper<T>& other) noexcept
            : pointer(other.get()) { }

        reference_wrapper(T&& reference) = delete;

        reference_wrapper& operator=(const reference_wrapper<T>& other) noexcept {
            pointer = &other.get();
        }

        operator T&() const noexcept {
            return *pointer;
        }
        T& get() const noexcept {
            return *pointer;
        }

        template<typename... Args>
        invoke_result_t<T&, Args...> operator()(Args&&... args) const
        {
#pragma message "TODO: Enable if callable"
            return (*pointer)(std::forward<Args>(args)...);
        }
    };

#ifdef __cpp_deduction_guides
    template<typename T>
    reference_wrapper(reference_wrapper<T>) -> reference_wrapper<T>;
#endif

    template<typename T>
    inline reference_wrapper<T> ref(T& reference) noexcept {
        return reference_wrapper<T>(reference);
    }
    template<typename T>
    inline reference_wrapper<T> ref(reference_wrapper<T> reference) noexcept {
        return reference_wrapper<T>(reference.get());
    }
    template<typename T>
    inline reference_wrapper<const T> cref(const T& reference) noexcept {
        return reference_wrapper<const T>(reference);
    }
    template<typename T>
    inline reference_wrapper<const T> cref(reference_wrapper<T> reference) noexcept {
        return reference_wrapper<T>(reference.get());
    }

    template<typename T>
    void ref(const T&&) = delete;

    template<typename T>
    void cref(const T&&) = delete;


    namespace __detail
    {
        template<typename T>
        T hash_buffer_fnv(const uint8_t*, size_t) noexcept
        {
            static_assert(!std::is_same<T, uint8_t>::value,
                "8-bit hash not supported.");
            static_assert(!std::is_same<T, uint16_t>::value,
                "16-bit hash not supported.");
        }
        template<typename T, typename Char>
        T hash_string_fnv(const Char*, size_t) noexcept
        {
            static_assert(!std::is_same<T, uint8_t>::value,
                "8-bit hash not supported.");
            static_assert(!std::is_same<T, uint16_t>::value,
                "16-bit hash not supported.");
        }

        template<> uint32_t hash_buffer_fnv<uint32_t>(const uint8_t*, size_t) noexcept;
        template<> uint64_t hash_buffer_fnv<uint64_t>(const uint8_t*, size_t) noexcept;
        template<> uint32_t hash_string_fnv<uint32_t>(const char*, size_t) noexcept;
        template<> uint32_t hash_string_fnv<uint32_t>(const wchar_t*, size_t) noexcept;
        template<> uint32_t hash_string_fnv<uint32_t>(const char16_t*, size_t) noexcept;
        template<> uint32_t hash_string_fnv<uint32_t>(const char32_t*, size_t) noexcept;
        template<> uint64_t hash_string_fnv<uint64_t>(const char*, size_t) noexcept;
        template<> uint64_t hash_string_fnv<uint64_t>(const wchar_t*, size_t) noexcept;
        template<> uint64_t hash_string_fnv<uint64_t>(const char16_t*, size_t) noexcept;
        template<> uint64_t hash_string_fnv<uint64_t>(const char32_t*, size_t) noexcept;

        template<> inline int32_t hash_buffer_fnv<int32_t>(const uint8_t* buffer, size_t size) noexcept {
            return static_cast<int32_t>(hash_buffer_fnv<uint32_t>(buffer, size));
        }
        template<> inline int64_t hash_buffer_fnv<int64_t>(const uint8_t* buffer, size_t size) noexcept {
            return static_cast<int64_t>(hash_buffer_fnv<uint64_t>(buffer, size));
        }
        template<> inline int32_t hash_string_fnv<int32_t>(const char* buffer, size_t size) noexcept {
            return static_cast<int32_t>(hash_string_fnv<uint32_t>(buffer, size));
        }
        template<> inline int32_t hash_string_fnv<int32_t>(const wchar_t* buffer, size_t size) noexcept {
            return static_cast<int32_t>(hash_string_fnv<uint32_t>(buffer, size));
        }
        template<> inline int32_t hash_string_fnv<int32_t>(const char16_t* buffer, size_t size) noexcept {
            return static_cast<int32_t>(hash_string_fnv<uint32_t>(buffer, size));
        }
        template<> inline int32_t hash_string_fnv<int32_t>(const char32_t* buffer, size_t size) noexcept {
            return static_cast<int32_t>(hash_string_fnv<uint32_t>(buffer, size));
        }
        template<> inline int64_t hash_string_fnv<int64_t>(const char* buffer, size_t size) noexcept {
            return static_cast<int64_t>(hash_string_fnv<uint64_t>(buffer, size));
        }
        template<> inline int64_t hash_string_fnv<int64_t>(const wchar_t* buffer, size_t size) noexcept {
            return static_cast<int64_t>(hash_string_fnv<uint64_t>(buffer, size));
        }
        template<> inline int64_t hash_string_fnv<int64_t>(const char16_t* buffer, size_t size) noexcept {
            return static_cast<int64_t>(hash_string_fnv<uint64_t>(buffer, size));
        }
        template<> inline int64_t hash_string_fnv<int64_t>(const char32_t* buffer, size_t size) noexcept {
            return static_cast<int64_t>(hash_string_fnv<uint64_t>(buffer, size));
        }


        template<typename T>
        struct identity_hash
        {
            using argument_type = T;
            using result_type = size_t;
            static_assert(sizeof(T) <= sizeof(result_type), "");

            result_type operator()(argument_type value) noexcept {
                return static_cast<result_type>(value);
            }
        };

        template<typename T>
        struct buffer_hash
        {
            using argument_type = T;
            using result_type = size_t;

            static_assert(is_standard_layout<T>::value, "");

            result_type operator()(argument_type value) noexcept {
                return hash_buffer_fnv<result_type>(&value, sizeof(value));
            }
        };

        template<typename T>
        struct string_hash
        {
            using argument_type = T*;
            using result_type = size_t;

            result_type operator()(const T* value, size_t size) noexcept {
                return hash_string_fnv<result_type>(value, size);
            }
        };

        template<typename T>
        struct disabled_hash
        {
            disabled_hash() = delete;
            disabled_hash(const disabled_hash&) = delete;
            disabled_hash(disabled_hash&&) = delete;
            disabled_hash& operator=(const disabled_hash&) = delete;
            disabled_hash& operator=(disabled_hash&&) = delete;
        };

        template<typename T>
        struct default_hash
        {
            using _RT = remove_cv_t<T>;
            using type = conditional_t<
                is_scalar<_RT>::value && sizeof(_RT) <= sizeof(size_t),
                identity_hash<_RT>, buffer_hash<_RT>>;
        };
    }

    template<typename Key>
    struct hash : conditional_t<is_enum<Key>::value,
        typename __detail::default_hash<Key>, __detail::disabled_hash<Key>> { };

    template<> struct hash<bool> : __detail::default_hash<bool>::type { };
    template<> struct hash<char> : __detail::default_hash<char>::type { };
    template<> struct hash<signed char> : __detail::default_hash<signed char>::type { };
    template<> struct hash<unsigned char> : __detail::default_hash<unsigned char>::type { };
    template<> struct hash<char16_t> : __detail::default_hash<char16_t>::type { };
    template<> struct hash<char32_t> : __detail::default_hash<char32_t>::type { };
    template<> struct hash<wchar_t> : __detail::default_hash<wchar_t>::type { };
    template<> struct hash<short> : __detail::default_hash<short>::type { };
    template<> struct hash<unsigned short> : __detail::default_hash<unsigned short>::type { };
    template<> struct hash<int> : __detail::default_hash<int>::type { };
    template<> struct hash<unsigned int> : __detail::default_hash<unsigned int>::type { };
    template<> struct hash<long> : __detail::default_hash<long>::type { };
    template<> struct hash<unsigned long> : __detail::default_hash<unsigned long>::type { };
    template<> struct hash<long long> : __detail::default_hash<long long>::type { };
    template<> struct hash<unsigned long long> : __detail::default_hash<unsigned long long>::type { };
    template<> struct hash<float> : __detail::default_hash<float>::type { };
    template<> struct hash<double> : __detail::default_hash<double>::type { };
    template<> struct hash<long double> : __detail::default_hash<long double>::type { };
    template<class T> struct hash<T*> : __detail::default_hash<T*>::type { };

}
}