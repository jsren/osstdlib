#pragma once
#include "iterator.hpp"
#include "cstring.hpp"

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
        using const_reverse_iterator = reverse_iterator<const_iterator>;
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
            if (index < 0 || index >= _size) {
                __abi::__throw_exception(std::out_of_range("startIndex"));
            }
            std::memcpy(destination, _data + startIndex, count - startIndex);
        }

        constexpr basic_string_view substr(size_type startIndex, size_type )
    };

    typedef basic_string_view<char> string_view;
    typedef basic_string_view<wchar_t> wstring_view;
    typedef basic_string_view<char16_t> u16string_view;
    typedef basic_string_view<char32_t> u32string_view;
}
