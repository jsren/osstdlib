#pragma once
#include <__iterator>
#include <cstring>
#include <allocator>
#include <memory>
#include <stdexcept>
#include <__string-decl>
#include <string_view>


namespace std
{
	namespace __detail
	{
		template<typename Char>
		const Char* __get_empty_str();

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

    template<typename Char, typename Traits = char_traits<Char>,
        typename Allocator = allocator<Char>>
    class __basic_string_base
    {
        static_assert(is_same<Char, typename Traits::char_type>::value,"");

    protected:
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

    protected:
        size_type _length{};
        size_type _capacity{};
        pointer _data{};

        __basic_string_base() = default;
        __basic_string_base(size_type length, pointer data)
            : _length(length), _capacity(length), _data(data) { }

    public:
        reference at(size_type pos) {
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

        reference operator[](size_type index) {
            return _data[index];
        }
        const_reference operator[](size_type index) const {
            return _data[index];
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
            return _data;
        }
        const_pointer data() const {
            return _data;
        }
        const_pointer c_str() const {
            return _data;
        }

        operator basic_string_view<Char, Traits>() const noexcept {
            return basic_string_view<Char, Traits>(data(), size());
        }

        iterator begin() noexcept {
            return iterator(_data);
        }
        const_iterator begin() const noexcept {
            return const_iterator(_data);
        }
        const_iterator cbegin() const noexcept {
            return const_iterator(_data);
        }
        reverse_iterator rbegin() const noexcept {
            return reverse_iterator(begin());
        }
        const_reverse_iterator crbegin() const noexcept {
            return const_reverse_iterator(cbegin());
        }

        iterator end() noexcept {
            return iterator(_data + size());
        }
        const_iterator end() const {
            return const_iterator(_data + size());
        }
        const_iterator cend() const {
            return const_iterator(_data + size());
        }
        reverse_iterator rend() const noexcept {
            return reverse_iterator(end());
        }
        const_reverse_iterator crend() const noexcept {
            return const_reverse_iterator(cend());
        }

        bool empty() const noexcept {
            return _length == 0;
        }
        size_type length() const noexcept {
            return _length;
        }
        size_type size() const noexcept {
            return _length;
        }
        size_type max_size() const noexcept {
            return static_cast<size_type>(-1);
        }

        void reserve(size_type capacity = 0);

        void capacity() const noexcept {
            return _capacity;
        }

        void shrink_to_fit() { }

        void clear() noexcept {
            _length = 0;
        }



        /*basic_string& insert(size_type index, size_type count, Char value)
        {
            if (index > _length) __abi::__throw_exception(std::out_of_range("index"));

            if (index + count > _capacity) resize(index + count);

            return *this;
        }*/
    };



    template<typename Char, typename Traits, typename Allocator>
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
        using typename __basic_string_base<Char, Traits, Allocator>::alloc_traits;

    private:
        allocator_type _alloc;

        using __basic_string_base<Char, Traits, Allocator>::_data;
        using __basic_string_base<Char, Traits, Allocator>::_length;

    public:
        basic_string() noexcept(noexcept(allocator_type()))  { }

        explicit basic_string(const allocator_type& alloc) noexcept : _alloc(alloc) { }

        basic_string(size_type length, Char character, const allocator_type& alloc = allocator_type())
            : __basic_string_base<Char, Traits, Allocator>(length, nullptr), _alloc(alloc)
        {
            _data = alloc_traits::allocate(_alloc, length+1);
            for (size_t i = 0; i < length; i++) {
                _data[i] = character;
            }
            _data[length] = '\0';
        }

        basic_string(const Char* cstr, size_type length, const allocator_type& alloc = allocator_type())
            : __basic_string_base<Char, Traits, Allocator>(length, nullptr), _alloc(alloc)
        {
            if (length > 0) {
                _data = alloc_traits::allocate(_alloc, length+1);
                std::memcpy(_data, cstr, length * sizeof(Char));
            }
        }

        basic_string(const Char* cstr, const allocator_type& alloc = allocator_type())
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

        using __basic_string_base<Char, Traits, Allocator>::_data;
        using __basic_string_base<Char, Traits, Allocator>::_length;

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
        basic_string() noexcept(noexcept(allocator_type())) { }

        explicit basic_string(const allocator_type& alloc) noexcept : _alloc(alloc) { }

        basic_string(size_type length, Char character, const allocator_type& alloc = allocator_type())
            : __basic_string_base<Char, Traits, Allocator>(length, nullptr), _alloc(alloc)
        {
            if (length > 0) {
                _data = new char[length + 1];
            }
            for (size_t i = 0; i < length; i++) {
                _data[i] = character;
            }
        }

        basic_string(const Char* cstr, size_type length, const allocator_type& alloc = allocator_type())
            : __basic_string_base<Char, Traits, Allocator>(length, nullptr), _alloc(alloc)
        {
            if (length > 0)
			{
                auto* data = new char[length + 1];
                std::memcpy(data, cstr, length * sizeof(Char));

				this->_data = data;
				this->_data[length] = '\0';
            }
        }

        basic_string(const Char* cstr, const allocator_type& alloc = allocator_type())
            : basic_string(cstr, std::strlen(cstr), alloc) { }


        ~basic_string() {
            if (_length != 0) delete[] _data;
        }

        basic_string& operator +=(const basic_string& other)
        {
            auto finalLength = _length + other._length;
            if (finalLength != 0)
            {
                auto* data = new char[finalLength + 1];
                std::memcpy(data, this->_data, _length);
                std::memcpy(data + _length, other._data, other._length);
                data[finalLength] = '\0';

                if (_length != 0) { delete[] this->_data; }
                this->_data     = data;
                this->_length   = finalLength;
                this->_capacity = finalLength;
            }
            return *this;
        }
        basic_string& operator +=(const char* other)
        {
            auto othersize = std::strlen(other);
            auto finalLength = _length + othersize;
            if (finalLength != 0)
            {
                auto* data = new char[finalLength + 1];
                std::memcpy(data, this->_data, _length);
                std::memcpy(data + _length, other, othersize);
                data[finalLength] = '\0';

                if (_length != 0) { delete[] this->_data; }
                this->_data      = data;
                this->_length   = finalLength;
                this->_capacity = finalLength;
            }
            return *this;
        }
    };


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
