#pragma once
#include "iterator.hpp"
#include "cstring.hpp"
#include "functional.hpp"
#include "limits.hpp"

namespace std
{
    template<typename Char, typename Traits = char_traits<Char>>
    class basic_string_view
    {
    public:
        using traits_type = Traits;
        using value_type = Char;
        using pointer = Char*;
        using const_pointer = const Char*;
        using reference = Char&;
        using const_reference = const Char&;
        using const_iterator = __detail::pointer_iterator<const Char>;
        using iterator = const_iterator;
        using const_reverse_iterator = std::reverse_iterator<const_iterator>;
        using reverse_iterator = const_reverse_iterator;
        using size_type = size_t;
        using difference_type = ptrdiff_t;

    private:
        const_pointer _data{};
        size_type _size{};

    public:
        constexpr basic_string_view() noexcept;
        constexpr basic_string_view(const basic_string_view& other) noexcept = default;
        constexpr basic_string_view(const Char* string, size_type size) : _data(string), _size(size) { }
        constexpr basic_string_view(const Char* string) : _data(string), _size(std::strlen(string)) { }

        constexpr basic_string_view& operator=(const basic_string_view& view) noexcept = default;

        constexpr iterator begin() noexcept {
            return iterator(_data);
        }
        constexpr const_iterator begin() const noexcept {
            return const_iterator(_data);
        }
        constexpr const_iterator cbegin() const noexcept {
            return const_iterator(_data);
        }
        constexpr reverse_iterator rbegin() const noexcept {
            return reverse_iterator(begin());
        }
        constexpr const_reverse_iterator crbegin() const noexcept {
            return const_reverse_iterator(cbegin());
        }

        constexpr iterator end() noexcept {
            return iterator(_data + size());
        }
        constexpr const_iterator end() const {
            return const_iterator(_data + size());
        }
        constexpr const_iterator cend() const {
            return const_iterator(_data + size());
        }
        constexpr reverse_iterator rend() const noexcept {
            return reverse_iterator(end());
        }
        constexpr const_reverse_iterator crend() const noexcept {
            return const_reverse_iterator(cend());
        }

        constexpr const_reference operator[](size_type index) const {
            return _data[index];
        }
        constexpr const_reference at(size_type index) const {
            if (index < 0 || index >= _size) {
                __abi::__throw_exception(std::out_of_range("index"));
            }
            return _data[index];
        }
        constexpr const_reference front() const {
            return _data[0];
        }
        constexpr const_reference back() const {
            return _data[_size - 1];
        }

        constexpr const_pointer data() const noexcept {
            return _data;
        }

        constexpr size_type size() const noexcept {
            return _size;
        }
        constexpr size_type length() const noexcept {
            return _size;
        }
        constexpr size_type max_size() const noexcept {
            return numeric_limits<size_type>::max() - 1;
        }
        [[nodiscard]] constexpr bool empty() const noexcept {
            return _size == 0;
        }

        constexpr void remove_prefix(size_type size) {
            _size -= size; _data += size;
        }
        constexpr void remove_suffix(size_type size) {
            _size -= size;
        }

        constexpr void swap(basic_string_view& other) noexcept {
            swap(_data, other._data);
            swap(_size, other._size);
        }

        size_type copy(Char* destination, size_type count,
            size_type startIndex = 0) const
        {
            if (startIndex < 0 || startIndex >= _size) {
                __abi::__throw_exception(std::out_of_range("startIndex"));
            }
            std::memcpy(destination, _data + startIndex, count - startIndex);
        }

        constexpr basic_string_view substr(size_type startIndex = 0,
            size_type count = npos) const
        {
            if (startIndex < 0 || startIndex >= _size) {
                __abi::__throw_exception(std::out_of_range("startIndex"));
            }
            auto size = _size - startIndex;
            return basic_string_view(_data, count > size ? size : count);
        }

        constexpr int compare(basic_string_view other) const noexcept
        {
            auto len = _size <= other._size ? _size : other._size;
            auto rv = Traits::compare(data(), other.data(), len);
            if (rv != 0 || _size == other._size) return rv;
            else return _size < other._size ? -1 : 1;
        }
        constexpr int compare(size_type startIndex, size_type count,
            basic_string_view other) const
        {
            return substr(startIndex, count).compare(other);
        }
        constexpr int compare(size_type startIndex1, size_type count1,
            basic_string_view other, size_type startIndex2, size_type count2) const
        {
            return substr(startIndex1, count1).compare(
                other.substr(startIndex2, count2));
        }
        constexpr int compare(const Char* string) const {
            return compare(basic_string_view(string));
        }
        constexpr int compare(size_type startIndex, size_type count, const Char* string) const {
            return substr(startIndex, count).compare(basic_string_view(string));
        }
        constexpr int compare(size_type startIndex1, size_type count1, const Char* other,
            size_type count2) const
        {
            return substr(startIndex1, count1).compare(basic_string_view(other, count2));
        }

        constexpr bool starts_with(basic_string_view prefix) const noexcept {
            return _size >= prefix._size && compare(0, prefix._size, prefix) == 0;
        }
        constexpr bool starts_with(Char prefix) const noexcept {
            return _size >= 1 && _data[0] == prefix;
        }
        constexpr bool starts_with(const Char* prefix) const {
            return starts_with(basic_string_view(prefix));
        }

        constexpr bool ends_with(basic_string_view suffix) const noexcept {
            return _size >= suffix._size && compare(_size - suffix._size, npos, suffix) == 0;
        }
        constexpr bool ends_with(Char suffix) const noexcept {
            return _size >= 1 && _data[_size - 1] == suffix;
        }
        constexpr bool ends_with(const Char* suffix) const {
            return ends_with(basic_string_view(suffix));
        }

        constexpr size_type find(basic_string_view substring, size_type startIndex = npos) const noexcept;
        constexpr size_type find(Char character, size_type startIndex = npos) const noexcept;
        constexpr size_type find(const Char* substring, size_type startIndex, size_type count) const;
        constexpr size_type find(const Char* substring, size_type startIndex = npos) const;

        constexpr size_type rfind(basic_string_view substring, size_type startIndex = npos) const noexcept;
        constexpr size_type rfind(Char character, size_type startIndex = npos) const noexcept;
        constexpr size_type rfind(const Char* substring, size_type startIndex, size_type count) const;
        constexpr size_type rfind(const Char* substring, size_type startIndex = npos) const;

        constexpr size_type find_first_of(basic_string_view substring, size_type startIndex = npos) const noexcept;
        constexpr size_type find_first_of(Char character, size_type startIndex = npos) const noexcept;
        constexpr size_type find_first_of(const Char* substring, size_type startIndex, size_type count) const;
        constexpr size_type find_first_of(const Char* substring, size_type startIndex = npos) const;

        constexpr size_type find_first_not_of(basic_string_view substring, size_type startIndex = npos) const noexcept;
        constexpr size_type find_first_not_of(Char character, size_type startIndex = npos) const noexcept;
        constexpr size_type find_first_not_of(const Char* substring, size_type startIndex, size_type count) const;
        constexpr size_type find_first_not_of(const Char* substring, size_type startIndex = npos) const;

        constexpr size_type find_last_of(basic_string_view substring, size_type startIndex = npos) const noexcept;
        constexpr size_type find_last_of(Char character, size_type startIndex = npos) const noexcept;
        constexpr size_type find_last_of(const Char* substring, size_type startIndex, size_type count) const;
        constexpr size_type find_last_of(const Char* substring, size_type startIndex = npos) const;

        constexpr size_type find_last_not_of(basic_string_view substring, size_type startIndex = npos) const noexcept;
        constexpr size_type find_last_not_of(Char character, size_type startIndex = npos) const noexcept;
        constexpr size_type find_last_not_of(const Char* substring, size_type startIndex, size_type count) const;
        constexpr size_type find_last_not_of(const Char* substring, size_type startIndex = npos) const;

        static constexpr size_type npos = numeric_limits<size_type>::max();
    };

    template<typename Char, typename Traits>
    constexpr bool operator ==(basic_string_view<Char, Traits> lhs,
        basic_string_view<Char, Traits> rhs) noexcept
    {
        return lhs.compare(rhs) == 0;
    }
    template<typename Char, typename Traits>
    constexpr bool operator !=(basic_string_view<Char, Traits> lhs,
        basic_string_view<Char, Traits> rhs) noexcept
    {
        return !(lhs == rhs);
    }
    template<typename Char, typename Traits>
    constexpr bool operator <(basic_string_view<Char, Traits> lhs,
        basic_string_view<Char, Traits> rhs) noexcept
    {
        return lhs.compare(rhs) < 0;
    }
    template<typename Char, typename Traits>
    constexpr bool operator <=(basic_string_view<Char, Traits> lhs,
        basic_string_view<Char, Traits> rhs) noexcept
    {
        return lhs.compare(rhs) <= 0;
    }
    template<typename Char, typename Traits>
    constexpr bool operator >(basic_string_view<Char, Traits> lhs,
        basic_string_view<Char, Traits> rhs) noexcept
    {
        return !(lhs <= rhs);
    }
    template<typename Char, typename Traits>
    constexpr bool operator >=(basic_string_view<Char, Traits> lhs,
        basic_string_view<Char, Traits> rhs) noexcept
    {
        return !(lhs < rhs);
    }


    typedef basic_string_view<char> string_view;
    typedef basic_string_view<wchar_t> wstring_view;
    typedef basic_string_view<char16_t> u16string_view;
    typedef basic_string_view<char32_t> u32string_view;

    template<typename Char>
    struct hash<basic_string_view<Char>>
    {
        using argument_type = basic_string_view<Char>;
        using result_type = size_t;

        result_type operator()(argument_type value) const {
            return __detail::string_hash<Char>{}(value.data(), value.size());
        }
    };
}

namespace std
{
    inline namespace literals
    {
        inline namespace string_view_literals
        {
            #pragma GCC diagnostic ignored "-Wliteral-suffix"
            inline constexpr string_view operator"" sv(const char* string, size_t length) noexcept {
                return std::string_view(string, length);
            }
            inline constexpr wstring_view operator"" sv(const wchar_t* string, size_t length) noexcept {
                return std::wstring_view(string, length);
            }
            inline constexpr u16string_view operator"" sv(const char16_t* string, size_t length) noexcept {
                return std::u16string_view(string, length);
            }
            inline constexpr u32string_view operator"" sv(const char32_t* string, size_t length) noexcept {
                return std::u32string_view(string, length);
            }
        }
        using namespace string_view_literals;
    }
    using namespace literals;
}
