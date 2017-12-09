#include "functional.hpp"


namespace std
{
    namespace __detail
    {
        template<typename T>
        static constexpr uint64_t getInitialBasis() noexcept;

        template<typename T>
        static constexpr uint64_t getPrime() noexcept;

        template<> constexpr uint64_t getInitialBasis<uint32_t>() noexcept {
            return 0x811c9dc5;
        } template<> constexpr uint64_t getInitialBasis<uint64_t>() noexcept {
            return 0xcbf29ce484222325ULL;
        }
        template<> constexpr uint64_t getPrime<uint32_t>() noexcept {
            return 0x01000193;
        } template<> constexpr uint64_t getPrime<uint64_t>() noexcept {
            return 0x100000001b3ULL;
        }


        template<typename T>
        static T _hash_buffer_fnv(const uint8_t* buffer, size_t size) noexcept
        {
            auto value = static_cast<T>(getInitialBasis<T>());
            for (size_t i = 0; i < size; i++) {
                value ^= buffer[i];
                value *= static_cast<T>(getPrime<T>());
            }
            return value;
        }

        template<typename T, typename Char>
        static T _hash_string_fnv(const Char* string, size_t size) noexcept
        {
            auto value = static_cast<T>(getInitialBasis<T>());
            for (size_t i = 0; i < size; i++) {
                value ^= static_cast<T>(string[i]);
                value *= static_cast<T>(getPrime<T>());
            }
            return value;
        }

        template<>
        uint32_t hash_buffer_fnv<uint32_t>(const uint8_t* buffer, size_t size) noexcept {
            return _hash_buffer_fnv<uint32_t>(buffer, size);
        }
        template<>
        uint64_t hash_buffer_fnv<uint64_t>(const uint8_t* buffer, size_t size) noexcept {
            return _hash_buffer_fnv<uint64_t>(buffer, size);
        }
        template<>
        uint32_t hash_string_fnv<uint32_t>(const char* string, size_t size) noexcept {
            return _hash_string_fnv<uint32_t>(string, size);
        }
        template<>
        uint32_t hash_string_fnv<uint32_t>(const wchar_t* string, size_t size) noexcept {
            return _hash_string_fnv<uint32_t>(string, size);
        }
        template<>
        uint32_t hash_string_fnv<uint32_t>(const char16_t* string, size_t size) noexcept {
            return _hash_string_fnv<uint32_t>(string, size);
        }
        template<>
        uint32_t hash_string_fnv<uint32_t>(const char32_t* string, size_t size) noexcept {
            return _hash_string_fnv<uint32_t>(string, size);
        }
        template<>
        uint64_t hash_string_fnv<uint64_t>(const char* string, size_t size) noexcept {
            return _hash_string_fnv<uint64_t>(string, size);
        }
        template<>
        uint64_t hash_string_fnv<uint64_t>(const wchar_t* string, size_t size) noexcept {
            return _hash_string_fnv<uint64_t>(string, size);
        }
        template<>
        uint64_t hash_string_fnv<uint64_t>(const char16_t* string, size_t size) noexcept {
            return _hash_string_fnv<uint64_t>(string, size);
        }
        template<>
        uint64_t hash_string_fnv<uint64_t>(const char32_t* string, size_t size) noexcept {
            return _hash_string_fnv<uint64_t>(string, size);
        }
    }
}
