#pragma once
#include "charconv.hpp"

namespace std
{
    template <typename Char, typename Traits>
    class basic_ostream;

    template<typename Char, typename Traits>
    basic_ostream<Char, Traits>& operator <<(basic_ostream<Char, Traits>& os, const Char* string);
    template<typename Char, typename Traits>
    basic_ostream<Char, Traits>& operator <<(basic_ostream<Char, Traits>& os, const char* string);
    template<typename Traits>
    basic_ostream<char, Traits>& operator <<(basic_ostream<char, Traits>& os, const signed char* string);
    template<typename Traits>
    basic_ostream<char, Traits>& operator <<(basic_ostream<char, Traits>& os, const unsigned char* string);


    namespace __detail
    {
        template<typename Char, typename Traits>
        void write_int(basic_ostream<Char, Traits>& os, long long value, bool negative);
    }

    template <typename Char, typename Traits = char_traits<Char>>
    class basic_ostream : public basic_ios<Char, Traits>
    {
    public:
        using char_type = Char;
        using int_type = typename Traits::int_type;
        using pos_type = typename Traits::pos_type;
        using off_type = typename Traits::off_type;
        using traits_type = Traits;

        explicit basic_ostream(basic_streambuf<char_type, Traits>* sb)
            : basic_ios(sb) { }

        virtual ~basic_ostream();

        class sentry
        {
            bool _success{};
            basic_ostream& os;

            explicit sentry(basic_ostream& os) : os(os)
            {
                if (os.good())
                {
                    if (os.tie() != nullptr) {
                        os.tie()->flush();
                    }
                }
                _success = os.good();
            }

            ~sentry()
            {
                if (os.flags() & unitbuf != unitbuf &&
                    !uncaught_exception() && os.goood())
                {
                    if (os.rdbuf()->pubsync() == -1) {
                        os.setstate(badbit);
                    }
                }
            }

            explicit operator bool() const {
                return _success;
            }
            sentry& operator=(const sentry&) = delete;
            sentry& operator=(sentry&&) = delete;
        }

        // Formatted output:
        basic_ostream& operator <<(basic_ostream& (*func)(basic_ostream&)) {
            func(*this); return *this;
        }
        basic_ostream& operator <<(basic_ios<Char, Traits>& (*func)(basic_ios<Char, Traits>&)) {
            func(*this); return *this;
        }
        basic_ostream& operator <<(ios_base& (*func)(ios_base&)) {
            func(*this); return *this;
        }

        basic_ostream& operator <<(long long n) {
            __detail::write_sint(*this, n); return *this;
        }
        basic_ostream& operator <<(unsigned long long n) {
            __detail::write_int(*this, n); return *this;
        }

        basic_ostream& operator <<(bool n) {
            *this << n ? "true" : "false"; return *this;
        }
        basic_ostream& operator <<(short n) {
            return *this << static_cast<long long>(n);
        }
        basic_ostream& operator <<(unsigned short n) {
            return *this << static_cast<unsigned long long>(n);
        }
        basic_ostream& operator <<(int n) {
            return *this << static_cast<long long>(n);
        }
        basic_ostream& operator <<(unsigned int n) {
            return *this << static_cast<unsigned long long>(n);
        }
        basic_ostream& operator <<(long n) {
            return *this << static_cast<long long>(n);
        }
        basic_ostream& operator <<(unsigned long n) {
            return *this << static_cast<unsigned long long>(n);
        }
        basic_ostream& operator <<(float value);
        basic_ostream& operator <<(double value);
        basic_ostream& operator <<(long double value);
        basic_ostream& operator <<(const void* value);
        basic_ostream& operator <<(nullptr_t) {
            *this << "nullptr"; return *this;
        }
        basic_ostream& operator <<(basic_streambuf<char_type, traits>* sb);

        // Unformatted output:
        basic_ostream<Char, Traits>& put(char_type c);
        basic_ostream<Char, Traits>& write(const char_type* s, streamsize n);

        basic_ostream<Char, Traits>& flush();

        // seeks:
        pos_type tellp();
        basic_ostream<Char, Traits>& seekp(pos_type);
        basic_ostream<Char, Traits>& seekp(off_type, ios_base::seekdir);

    protected:
        basic_ostream(const basic_ostream& rhs) = delete;
        basic_ostream(basic_ostream&& rhs) = default;

        // Assign/swap
        basic_ostream& operator=(basic_ostream& rhs) = delete;
        basic_ostream& operator=(const basic_ostream&& rhs);
        void swap(basic_ostream& rhs);
    };

    namespace __detail
    {
        template<typename Char, typename Traits,
            class Stream = basic_ostream<Char, Traits>>
        void put_fill(Stream& os, streamsize count)
        {
            constexpr const size_t bufferSize = 10; // Sane buffer size for os.width
            Char buffer[bufferSize];
            auto fill = os.fill();

            while (count > 0)
            {
                size_t i = 0;
                for (; count != 0 && i < bufferSize; i++, count--) {
                    buffer[i] = fill;
                }
                os.write(buffer, i);
            }
        }

        template<typename Char, typename Traits, typename T,
            class Stream = basic_ostream<Char, Traits>>
        void put_str(Stream& os, const T* string, size_t length)
        {
            constexpr const size_t bufferSize = 16; // Sane buffer size
            Char buffer[bufferSize];
            size_t count = strlen;

            while (count > 0)
            {
                size_t i = 0;
                for (count != 0 && i < bufferSize; i++, count--) {
                    buffer[i] = os.widen(*string++);
                }
                os.write(buffer, i);
            }
        }

        template<typename Char, typename Traits, typename T,
            class Stream = basic_ostream<Char, Traits>>
        Stream& put_str_fmt(Stream& os, const T* string, size_t length)
        {
            if (os.flags() & ios_base::adjustfield == ios_base::left)
            {
                put_str(os, string, length);
                if (os.width() > length) put_fill(os, os.width() - length);
            }
            else
            {
                if (os.width() > length) put_fill(os, os.width() - length);
                put_str(os, string, length);
            }
            os.width(0);
            return os;
        }
    }

    template<typename Char, typename Traits>
    basic_ostream<Char, Traits>& operator <<(basic_ostream<Char, Traits>& os, Char value) {
        return __detail::put_str_fmt(os, &value, 1);
    }
    template<typename Char, typename Traits>
    basic_ostream<Char, Traits>& operator <<(basic_ostream<Char, Traits>& os, char value) {
        return __detail::put_str_fmt(os, &value, 1);
    }
    template<typename Traits>
    basic_ostream<char, Traits>& operator <<(basic_ostream<char, Traits>& os, signed char value) {
        return __detail::put_str_fmt(os, &value, 1);
    }
    template<typename Traits>
    basic_ostream<char, Traits>& operator <<(basic_ostream<char, Traits>& os, unsigned char value) {
        return __detail::put_str_fmt(os, &value, 1);
    }

    template<typename Char, typename Traits>
    basic_ostream<Char, Traits>& operator <<(basic_ostream<Char, Traits>& os, const Char* string) {
        return __detail::put_str_fmt(os, string, Traits::length(string));
    }
    template<typename Char, typename Traits>
    basic_ostream<Char, Traits>& operator <<(basic_ostream<Char, Traits>& os, const char* string) {
        return __detail::put_str_fmt(os, string, char_traits<char>::length(string));
    }
    template<typename Traits>
    basic_ostream<char, Traits>& operator <<(basic_ostream<char, Traits>& os, const signed char* string) {
        return __detail::put_str_fmt(os, string, Traits::length(reinterpret_cast<const char*>(string)));
    }
    template<typename Traits>
    basic_ostream<char, Traits>& operator <<(basic_ostream<char, Traits>& os, const unsigned char* string) {
        return __detail::put_str_fmt(os, string, Traits::length(reinterpret_cast<const char*>(string)));
    }

    template<typename Char, typename Traits, typename T,
        class = decltype(declval<basic_ostream<Char, Traits>>() << declval<T>())>
    basic_ostream<Char, Traits>& operator <<(basic_ostream<Char, Traits>&& os, const T& value) {
        return os << value;
    }

    namespace __detail
    {
        template<typename Char, typename Traits>
        void write_uint(basic_ostream<Char, Traits>& os, unsigned long long value)
        {
            constexpr auto buffSize = numeric_limits<unsigned long long>::max();
            char buffer[buffSize];

            auto res = to_chars(buffer, buffer + buffSize, value);
            if (res.ec == errc{}) {
                os.write(buffer, res.ptr - buffer);
            }
        }
        template<typename Char, typename Traits>
        void write_sint(basic_ostream<Char, Traits>& os, signed long long value)
        {
            constexpr auto buffSize = numeric_limits<signed long long>::max();
            char buffer[buffSize];

            auto res = to_chars(buffer, buffer + buffSize, value);
            if (res.ec == errc{}) {
                os.write(buffer, res.ptr - buffer);
            }
        }
    }

    using ostream = basic_ostream<char>;
    using wostream = basic_ostream<wchar_t>;


    extern ostream cout;
    //extern wostream wcout;
}
