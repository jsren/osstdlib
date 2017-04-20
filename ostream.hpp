#pragma once

namespace std
{

    template <class charT, class traits = char_traits<charT>>
    class basic_ostream : virtual public basic_ios<charT, traits> {
    public:
        typedef charT                     char_type;
        typedef typename traits::int_type int_type;
        typedef typename traits::pos_type pos_type;
        typedef typename traits::off_type off_type;
        typedef traits                    traits_type;

        // Constructor/destructor:
        explicit basic_ostream(basic_streambuf<char_type, traits>* sb);
        virtual ~basic_ostream();

        // Prefix/suffix:
        class sentry;

        // Formatted output:
        basic_ostream<charT, traits>& operator<<(
            basic_ostream<charT, traits>& (*pf)(basic_ostream<charT, traits>&));
        basic_ostream<charT, traits>& operator<<(
            basic_ios<charT, traits>& (*pf)(basic_ios<charT, traits>&));
        basic_ostream<charT, traits>& operator<<(
            ios_base& (*pf)(ios_base&));

        basic_ostream<charT, traits>& operator<<(bool n);
        basic_ostream<charT, traits>& operator<<(short n);
        basic_ostream<charT, traits>& operator<<(unsigned short n);
        basic_ostream<charT, traits>& operator<<(int n);
        basic_ostream<charT, traits>& operator<<(unsigned int n);
        basic_ostream<charT, traits>& operator<<(long n);
        basic_ostream<charT, traits>& operator<<(unsigned long n);
        basic_ostream<charT, traits>& operator<<(long long n);
        basic_ostream<charT, traits>& operator<<(unsigned long long n);
        basic_ostream<charT, traits>& operator<<(float f);
        basic_ostream<charT, traits>& operator<<(double f);
        basic_ostream<charT, traits>& operator<<(long double f);

        basic_ostream<charT, traits>& operator<<(const void* p);
        basic_ostream<charT, traits>& operator<<(
            basic_streambuf<char_type, traits>* sb);

        // Unformatted output:
        basic_ostream<charT, traits>& put(char_type c);
        basic_ostream<charT, traits>& write(const char_type* s, streamsize n);

        basic_ostream<charT, traits>& flush();

        // seeks:
        pos_type tellp();
        basic_ostream<charT, traits>& seekp(pos_type);
        basic_ostream<charT, traits>& seekp(off_type, ios_base::seekdir);

    protected:
        basic_ostream(const basic_ostream& rhs) = delete;
        basic_ostream(basic_ostream&& rhs);

        // Assign/swap
        basic_ostream& operator=(basic_ostream& rhs) = delete;
        basic_ostream& operator=(const basic_ostream&& rhs);
        void swap(basic_ostream& rhs);
    };
}
