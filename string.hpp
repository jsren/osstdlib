#pragma once
#include "cstring.hpp"
#include "allocator.hpp"
#include "memory.hpp"
#include "stdexcept.hpp"
#include "string-bits.hpp"

namespace __std
{
    template<typename Char>
    inline const Char* __get_empty_str();

    template<> inline const char* __get_empty_str<char>() {
        extern const char* __empty_cstring; return __empty_cstring;
    }
    template<> inline const wchar_t* __get_empty_str<wchar_t>() {
        extern const wchar_t* __empty_wstring; return __empty_wstring;
    }
    template<> inline const char16_t* __get_empty_str<char16_t>() {
        extern const char16_t* __empty_ustring; return __empty_ustring;
    }
    template<> inline const char32_t* __get_empty_str<char32_t>() {
        extern const char32_t* __empty_Ustring; return __empty_Ustring;
    }
}


namespace std
{
    template<typename Char, typename Traits = char_traits<Char>,
        typename Allocator = allocator<Char>>
    class __basic_string_base
    {
    protected:
        using alloc_traits = allocator_traits<Allocator>;

    public:
        using traits_type     = Traits;
        using value_type      = typename Traits::value_type;
        using allocator_type  = Allocator;
        using size_type       = typename alloc_traits::size_type;
        using difference_type = typename alloc_traits::difference_type;
        using reference       = value_type&;
        using const_reference = const value_type&;
        using pointer         = typename alloc_traits::pointer;
        using const_pointer   = typename alloc_traits::const_pointer;

    protected:
        size_type _length = 0;
        pointer _data = nullptr;

        __basic_string_base() = default;
        __basic_string_base(size_type length, pointer data)
            : _length(length), _data(data) { }

    public:
        inline reference operator[](size_type pos) {
            return _data[pos];
        }
        const_reference operator[](size_type pos) const {
            return _data[pos];
        }

        inline reference at(size_type pos) {
            if (pos < 0 || pos >= length()) {
                __abi::__throw_exception(std::out_of_range("pos"));
            }
            else return _data[pos];
        }
        const_reference at(size_type pos) const {
            if (pos < 0 || pos >= length()) {
                __abi::__throw_exception(std::out_of_range("pos"));
            }
            else return _data[pos];
        }

        inline reference front() {
            return operator[](0);
        }
        inline const_reference front() const {
            return operator[](0);
        }

        inline reference back() {
            return operator[](size() - 1);
        }
        inline const_reference back() const {
            return operator[](size() - 1);
        }

        inline pointer data() {
            return _data;
        }
        inline const_pointer data() const {
            return _data;
        }
        inline const_pointer c_str() const {
            return _data;
        }

        inline size_type length() const noexcept {
            return _length;
        }
        inline size_type size() const noexcept {
            return _length;
        }
    };



    template<typename Char, typename Traits = char_traits<Char>, 
        typename Allocator = allocator<Char>>
    class basic_string : public __basic_string_base<Char, Traits, Allocator>
    {
    public:
        using typename __basic_string_base<Char, Traits, Allocator>::traits_type;
        using typename __basic_string_base<Char, Traits, Allocator>::value_type;
        using typename __basic_string_base<Char, Traits, Allocator>::allocator_type;
        using typename __basic_string_base<Char, Traits, Allocator>::size_type;
        using typename __basic_string_base<Char, Traits, Allocator>::difference_type;
        using typename __basic_string_base<Char, Traits, Allocator>::reference;
        using typename __basic_string_base<Char, Traits, Allocator>::const_reference;
        using typename __basic_string_base<Char, Traits, Allocator>::pointer;
        using typename __basic_string_base<Char, Traits, Allocator>::const_pointer;

    private:
        allocator_type _alloc;

    public:
        inline basic_string() noexcept(noexcept(allocator_type()))  { }

        inline explicit basic_string(const allocator_type& alloc) noexcept : _alloc(alloc) { }

        basic_string(size_type length, Char character, const allocator_type& alloc = allocator_type())
            : __basic_string_base(length, nullptr), _alloc(alloc)
        {
            _data = alloc_traits::allocate(_alloc, length+1);
            _data[length] = '\0';
        }

        basic_string(const Char* cstr, size_type length, const allocator_type& alloc = allocator_type())
            : __basic_string_base(length, nullptr), _alloc(alloc)
        {
            if (length > 0) {
                _data = alloc_traits::allocate(_alloc, length+1);
                std::memcpy(_data, cstr, length * sizeof(Char));
            }
        }

        inline basic_string(const Char* cstr, const allocator_type& alloc = allocator_type())
            : basic_string(cstr, std::strlen(cstr), alloc) { }


        ~basic_string() {
            if (_data != 0) alloc_traits::deallocate(_alloc, _data, _length+1);
        }
    };



    template<typename Char>
    class basic_string<Char, char_traits<Char>, allocator<Char>>
        : public __basic_string_base<Char, char_traits<Char>, allocator<Char>>
    {
        using Traits = char_traits<Char>;
        using Allocator = allocator<Char>;

        using typename __basic_string_base<Char, Traits, Allocator>::traits_type;
        using typename __basic_string_base<Char, Traits, Allocator>::value_type;
        using typename __basic_string_base<Char, Traits, Allocator>::allocator_type;
        using typename __basic_string_base<Char, Traits, Allocator>::size_type;
        using typename __basic_string_base<Char, Traits, Allocator>::difference_type;
        using typename __basic_string_base<Char, Traits, Allocator>::reference;
        using typename __basic_string_base<Char, Traits, Allocator>::const_reference;
        using typename __basic_string_base<Char, Traits, Allocator>::pointer;
        using typename __basic_string_base<Char, Traits, Allocator>::const_pointer;

    private:
        allocator_type _alloc{};

    public:
        inline basic_string() noexcept(noexcept(allocator_type())) { }

        inline explicit basic_string(const allocator_type& alloc) noexcept : _alloc(alloc) { }

        basic_string(size_type length, Char character, const allocator_type& alloc = allocator_type())
            : __basic_string_base(length, nullptr), _alloc(alloc)
        {
            if (length > 0) {
                _data = new char[length + 1];
            }
        }

        basic_string(const Char* cstr, size_type length, const allocator_type& alloc = allocator_type())
            : __basic_string_base(length, nullptr), _alloc(alloc)
        {
            if (length > 0) {
                auto* data = new char[length + 1];
                std::memcpy(data, cstr, length * sizeof(Char));
                _data = data;
            }
        }

        inline basic_string(const Char* cstr, const allocator_type& alloc = allocator_type())
            : basic_string(cstr, std::strlen(cstr), alloc) { }


        ~basic_string() {
            if (_length != 0) delete[] _data;
        }
    };



    typedef basic_string<char> string;
    typedef basic_string<wchar_t> wstring;
    typedef basic_string<char16_t> u16string;
    typedef basic_string<char32_t> u32string;
}
