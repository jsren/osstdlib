/* string.hpp - (c) 2017-18 James Renwick */
#pragma once
#include <__iterator>
#include <cstring>
#include <allocator>
#include <memory>
#include <stdexcept>
#include <__string-decl>
#include <string_view>
#include <limits>
#include <initializer_list>


namespace std
{
    template<typename Char, typename Traits, typename Allocator>
    class basic_string
    {
        static_assert(is_same<Char, typename Traits::char_type>::value,"");

    private:
        using alloc_traits = allocator_traits<Allocator>;

    public:
        using traits_type            = Traits;
        using value_type             = typename Traits::char_type;
        using allocator_type         = Allocator;
        using size_type              = typename alloc_traits::size_type;
        using difference_type        = typename alloc_traits::difference_type;
        using reference              = value_type&;
        using const_reference        = const value_type&;
        using pointer                = typename alloc_traits::pointer;
        using const_pointer          = typename alloc_traits::const_pointer;
        using iterator               = __detail::pointer_iterator<Char>;
        using const_iterator         = __detail::pointer_iterator<const Char>;
        using reverse_iterator       = std::reverse_iterator<iterator>;
        using const_reverse_iterator = std::reverse_iterator<const_iterator>;

    private:
        struct long_string_data
        {
            size_type _capacity;
            size_type _length;
            pointer _data;
        };

        static constexpr const size_type sso_capacity =
            (sizeof(long_string_data) - sizeof(uint8_t)) / sizeof(Char);

        struct short_string_data
        {
            uint8_t _length;
            aligned_storage_t<sso_capacity, alignof(Char)> _data;
        };

        static_assert(sizeof(short_string_data) <= sizeof(long_string_data),
            "Invalid sizing for Small-String Optimisation");

    private:
        allocator_type _alloc{};
        union {
            long_string_data _long;
            short_string_data _short;
        } _repr{};

        constexpr bool is_sso() const noexcept {
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
            if (is_sso()) _repr._short._length = 0b1 | (size << 1);
            else _repr._long._length = size;
        }

    public:
        basic_string() noexcept(noexcept(allocator_type())) : _alloc() {
            _repr._short._length = 1;
        };

        explicit basic_string(const allocator_type& alloc) noexcept : _alloc(alloc) {
            _repr._short._length = 1;
        }

        void _destroy(pointer data, size_type size, size_type capacity)
        {
            for (size_type i = 0; i < size; i++) {
                alloc_traits::destroy(_alloc, &data[i]);
            }
            alloc_traits::deallocate(_alloc, data, capacity);
        }

        void _destroy()
        {
            if (is_sso()) {
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
            bool prevHeap = !is_sso() && prevData != nullptr;
            size_type newSize = (prevSize + 1 > newCapacity) ?
                newCapacity - 1 : prevSize;

            // Short string
            if (newCapacity <= sso_capacity)
            {
                _repr._short._length = 0b1 | (newSize << 1); // Set SSO flag
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

        basic_string(size_type length, Char character,
            const allocator_type& alloc = allocator_type()) : _alloc(alloc)
        {
            auto data = _reallocate(length + 1);
            for (size_t i = 0; i < length; i++) {
                data[i] = character;
            }
            data[length] = '\0';
            _set_size(length);
        }

        basic_string(const Char* cstr, size_type length, const allocator_type& alloc = allocator_type())
            : _alloc(alloc)
        {
            auto data = _reallocate(length + 1);
            for (size_t i = 0; i < length; i++) {
                data[i] = cstr[i];
            }
            data[length] = '\0';
            _set_size(length);
        }

        basic_string(const Char* cstr, const allocator_type& alloc = allocator_type())
            : basic_string(cstr, traits_type::length(cstr), alloc) { }


        ~basic_string()
        {
            _destroy();
            alloc_traits::deallocate(_alloc, data(), capacity());
        }

        static constexpr size_type npos = numeric_limits<size_type>::max();

        reference at(size_type pos) {
            if (pos < 0 || pos >= size()) {
                __abi::__throw_exception(std::out_of_range("pos"));
            }
            else return data()[pos];
        }
        const_reference at(size_type pos) const {
            if (pos < 0 || pos >= length()) {
                __abi::__throw_exception(std::out_of_range("pos"));
            }
            else return data()[pos];
        }

        reference operator[](size_type index) {
            return data()[index];
        }
        const_reference operator[](size_type index) const {
            return data()[index];
        }

        reference front() {
            return operator[](0);
        }
        const_reference front() const {
            return operator[](0);
        }

        reference back() {
            return operator[](size() - 1);
        }
        const_reference back() const {
            return operator[](size() - 1);
        }

        pointer data() {
            return is_sso() ? reinterpret_cast<pointer>(&_repr._short._data)
                : _repr._long._data;
        }
        const_pointer data() const {
            return is_sso() ? reinterpret_cast<const_pointer>(&_repr._short._data)
                : _repr._long._data;
        }
        const_pointer c_str() const {
            return data();
        }

        operator basic_string_view<Char, Traits>() const noexcept {
            return basic_string_view<Char, Traits>(data(), size());
        }

        iterator begin() noexcept {
            return iterator(data());
        }
        const_iterator begin() const noexcept {
            return const_iterator(data());
        }
        const_iterator cbegin() const noexcept {
            return const_iterator(data());
        }
        reverse_iterator rbegin() const noexcept {
            return reverse_iterator(begin());
        }
        const_reverse_iterator crbegin() const noexcept {
            return const_reverse_iterator(cbegin());
        }

        iterator end() noexcept {
            return iterator(data() + size());
        }
        const_iterator end() const {
            return const_iterator(data() + size());
        }
        const_iterator cend() const {
            return const_iterator(data() + size());
        }
        reverse_iterator rend() const noexcept {
            return reverse_iterator(end());
        }
        const_reverse_iterator crend() const noexcept {
            return const_reverse_iterator(cend());
        }

        bool empty() const noexcept {
            return size() == 0;
        }
        size_type length() const noexcept {
            return size();
        }
        size_type size() const noexcept {
            return is_sso() ? (_repr._short._length >> 1) : _repr._long._length;
        }
        size_type max_size() const noexcept {
            auto v = numeric_limits<size_type>::max() - 1;
            return ((v & 0b1) == 0) ? v : v - 1;
        }
    private:
        size_type _capacity() const noexcept {
            return is_sso() ? sso_capacity : _repr._long._capacity;
        }
    public:
        size_type capacity() const noexcept {
            return _capacity() - 1;
        }

        void reserve(size_type capacity = 0) {
            _reallocate(capacity + 1);
        }

        void shrink_to_fit()
        {
            if (!is_sso() && capacity() > size()) {
                _reallocate(size() + 1);
            }
        }

        void clear() noexcept
        {
            _destroy();
            _set_size(0);
        }

    private:
        basic_string& append(const Char* otherData, size_type otherLength)
        {
            if (otherLength != 0)
            {
                pointer data = this->data();
                auto finalLength = size() + otherLength;

                if (finalLength > capacity()) {
                    data = _reallocate(finalLength);
                }
                traits_type::copy(data + size(), otherData, otherLength);
                data[finalLength] = '\0';
                _set_size(finalLength);
            }
            return *this;
        }

    public:
        basic_string& operator +=(const basic_string& other)
        {
            return append(other.data(), other.size());
        }
        basic_string& operator +=(const Char* other)
        {
            return append(other, traits_type::length(other));
        }
        basic_string& operator +=(Char other)
        {
            return append(&other, 1);
        }
        basic_string& operator +=(initializer_list<Char> list)
        {
            return append(list.begin(), list.end());
        }
        basic_string& operator +=(basic_string_view<Char, Traits> view)
        {
            return append(view.data(), view.size());
        }

        /*basic_string& insert(size_type index, size_type count, Char value)
        {
            if (index > _length) __abi::__throw_exception(std::out_of_range("index"));

            if (index + count > _capacity) resize(index + count);

            return *this;
        }*/

        basic_string substr(size_type startIndex = 0, size_type count = npos) const
        {
            if (startIndex > size()) {
                __abi::__throw_exception(std::out_of_range("startIndex"));
            }
            return basic_string(data() + startIndex, count);
        }
    };

    template<typename Char, typename Traits, typename Alloc>
    bool operator ==(const basic_string<Char, Traits, Alloc>& lhs,
        const basic_string<Char, Traits, Alloc>& rhs)
    {
        return lhs.size() == rhs.size() &&
            Traits::compare(lhs.data(), rhs.data(), lhs.size()) == 0;
    }

    template<typename Char, typename Traits, typename Alloc>
    bool operator ==(const basic_string<Char, Traits, Alloc>& lhs, const Char* rhs)
    {
        return lhs.size() == Traits::length(rhs) &&
            Traits::compare(lhs.data(), rhs, lhs.size()) == 0;
    }


    typedef basic_string<char> string;
    typedef basic_string<wchar_t> wstring;
    typedef basic_string<char16_t> u16string;
    typedef basic_string<char32_t> u32string;


    namespace __detail
    {
        string uint_to_string(unsigned long long value);
        string int_to_string(long long value);
    }

    inline string to_string(int value) {
        return __detail::int_to_string(value);
    }
    inline string to_string(long value) {
        return __detail::int_to_string(value);
    }
    inline string to_string(long long value) {
        return __detail::int_to_string(value);
    }
    inline string to_string(unsigned int value) {
        return __detail::uint_to_string(value);
    }
    inline string to_string(unsigned long value) {
        return __detail::uint_to_string(value);
    }
    inline string to_string(unsigned long long value) {
        return __detail::uint_to_string(value);
    }
}

namespace std
{
    inline namespace literals
    {
        inline namespace string_literals
        {
            #pragma GCC diagnostic ignored "-Wliteral-suffix"
            #pragma GCC diagnostic ignored "-Wuser-defined-literals"
            inline string operator"" s(const char* string, size_t length) {
                return std::string(string, length);
            }
            inline wstring operator"" s(const wchar_t* string, size_t length) {
                return std::wstring(string, length);
            }
            inline u16string operator"" s(const char16_t* string, size_t length) {
                return std::u16string(string, length);
            }
            inline u32string operator"" s(const char32_t* string, size_t length) {
                return std::u32string(string, length);
            }
        }
        using namespace string_view_literals;
    }
    using namespace literals;
}
