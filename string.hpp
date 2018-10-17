/* string.hpp - (c) 2017-18 James Renwick */
#pragma once
#include <__config>
#include <__iterator>
#include <cstring>
#include <allocator>
#include <memory>
#include <stdexcept>
#include <__string-decl>
#include <string_view>
#include <limits>
#include <initializer_list>
#include <__smallbuff>
#include <type_traits>
#include <new>

namespace std
{
    namespace __detail
    {
        template<typename Char, typename Allocator>
        struct string_sbo_config
        {
            using value_type = Char;
            using alloc_traits = allocator_traits<Allocator>;
            using allocator_type = Allocator;
            using size_type = typename alloc_traits::size_type;
            using pointer = typename alloc_traits::pointer;
            using const_pointer = typename alloc_traits::const_pointer;

            static constexpr size_t get_max_size() {
                auto v = numeric_limits<size_type>::max() - 1;
                return ((v & 0b1) == 0) ? v : v - 1;
            }

            static constexpr const auto realloc_factor = _config::string_realloc_factor;
            static constexpr const size_t extra = _config::string_sso_extra;
            static constexpr const size_t max_size = get_max_size();

            static void move(pointer to, pointer from, size_type size)
            {
                for (size_type i = 0; i < size; i++) {
                    __detail::move_construct_if_nothrow<Char>::invoke(to, *from);
                }
            }

            static pointer allocate(allocator_type alloc, size_type size)
            {
                return alloc_traits::allocate(alloc, size);
            }

            static void destroy(allocator_type alloc, pointer ptr)
            {
                alloc_traits::destroy(alloc, ptr);
            }

            static void deallocate(allocator_type alloc, pointer ptr, size_type size)
            {
                alloc_traits::deallocate(alloc, ptr, size);
            }
        };
    }


    template<typename Char, typename Traits, typename Allocator>
    class basic_string : __detail::sbo_type<__detail::string_sbo_config<Char, Allocator>>
    {
        static_assert(is_same<Char, typename Traits::char_type>::value,"");
        using base = __detail::sbo_type<__detail::string_sbo_config<Char, Allocator>>;

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

    public:
        /**
         * @brief Construct a new basic string object
         * Creates a new string with a default-constructed allocator.
         */
        basic_string() noexcept(noexcept(allocator_type())) : base({})
        {
            static_assert(base::sbo_capacity >= 1, "");
            base::_set_size(1);
            data()[0] = '\0';
        };

        /**
         * @brief Construct a new basic string object
         * Creates a new string with the given allocator.
         * @param alloc The allocator to use
         */
        explicit basic_string(const allocator_type& alloc) noexcept : base({}, alloc)
        {
            static_assert(base::sbo_capacity >= 1, "");
            base::_set_size(1);
            data()[0] = '\0';
        }

    private:
        pointer reallocate(size_type newCapacity)
        {
            return base::_reallocate(newCapacity);
        }

    public:
        basic_string(size_type length, Char character,
            const allocator_type& alloc = allocator_type()) : base({}, alloc)
        {
            auto data = reallocate(length + 1);
            for (size_t i = 0; i < length; i++) {
                data[i] = character;
            }
            data[length] = '\0';
            base::_set_size(length + 1);
        }

        basic_string(const Char* cstr, size_type length, const allocator_type& alloc = allocator_type())
            : base({}, alloc)
        {
            auto data = reallocate(length + 1);
            for (size_t i = 0; i < length; i++) {
                data[i] = cstr[i];
            }
            data[length] = '\0';
            base::_set_size(length + 1);
        }

        basic_string(const Char* cstr, const allocator_type& alloc = allocator_type())
            : basic_string(cstr, traits_type::length(cstr), alloc) { }

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
            return base::_data();
        }
        const_pointer data() const {
            return base::_data();
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
            return base::_size() - 1;
        }
        size_type max_size() const noexcept {
            return base::max_size;
        }
    public:
        size_type capacity() const noexcept {
            return base::_capacity() - 1;
        }

        void reserve(size_type capacity = 0) {
            reallocate(capacity);
        }

        void shrink_to_fit()
        {
            if (!base::is_sbo() && capacity() > size()) {
                reallocate(size());
            }
        }

        void clear() noexcept
        {
            base::_destroy();
            base::_set_size(0);
        }

    private:
        basic_string& append(const Char* otherData, size_type otherLength)
        {
            if (otherLength != 0)
            {
                pointer data = this->data();
                auto finalLength = size() + otherLength;

                if (finalLength > capacity()) {
                    data = reallocate(finalLength);
                }
                traits_type::copy(data + size(), otherData, otherLength);
                data[finalLength] = '\0';
                base::_set_size(finalLength + 1);
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

            if (index + count > capacity()) resize(index + count);

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
            #pragma GCC system_header
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
