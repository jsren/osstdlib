/* cstdio.hpp - (c) 2018 James Renwick */
#pragma once
#include <cstring>

namespace std
{
    namespace __detail
    {
        template<typename T> struct promoter {
            const T& value;
        };
        template<typename T> struct promoter<T*> {
            const void* value;
            promoter(const T*& v) : value(v) { }
        };
        template<> struct promoter<bool> {
            int value;
            promoter(bool v) : value(v) { }
        };
        template<> struct promoter<char> {
            int value;
            promoter(char v) : value(v) { }
        };
        template<> struct promoter<signed char> {
            int value;
            promoter(signed char v) : value(v) { }
        };
        template<> struct promoter<unsigned char> {
            int value;
            promoter(unsigned char v) : value(v) { }
        };
        template<> struct promoter<signed short> {
            int value;
            promoter(signed short v) : value(v) { }
        };
        template<> struct promoter<unsigned short> {
            int value;
            promoter(unsigned short v) : value(v) { }
        };
        template<> struct promoter<signed int> {
            const signed int& value;
            promoter(const signed int& v) : value(v) { }
        };
        template<> struct promoter<unsigned int> {
            const unsigned int& value;
            promoter(const unsigned int& v) : value(v) { }
        };
        template<> struct promoter<signed long> {
            const signed long& value;
            promoter(const signed long& v) : value(v) { }
        };
        template<> struct promoter<unsigned long> {
            const unsigned long& value;
            promoter(const unsigned long& v) : value(v) { }
        };
        template<> struct promoter<signed long long> {
            const signed long long& value;
            promoter(const signed long long& v) : value(v) { }
        };
        template<> struct promoter<unsigned long long> {
            const unsigned long long& value;
            promoter(const unsigned long long& v) : value(v) { }
        };
        template<> struct promoter<float> {
            double value;
            promoter(float v) : value(v) { }
        };
        template<> struct promoter<double> {
            const double& value;
            promoter(const double& v) : value(v) { }
        };
        template<> struct promoter<long double> {
            const long double& value;
            promoter(const long double& v) : value(v) { }
        };
        template<> struct promoter<wchar_t> {
            int value;
            promoter(wchar_t v) : value((int)v) { }
        };
        template<>
        struct promoter<char16_t>
        {
            int value;
            promoter(char16_t v) : value(v) { }
        };
        template<>
        struct promoter<char32_t>
        {
            int value;
            promoter(char32_t v) : value(v) { }
        };

    }

    void build_args(const void** arg_addresses) noexcept
    {
    }

    template<typename Arg, typename ...Args>
    void build_args(const void** arg_addresses, const Arg& arg, const Args&... args) noexcept
    {
        *arg_addresses = &arg.value;
        build_args(arg_addresses + 1, args...);
    }

    template<typename Arg, typename ...Args>
    void printf(const char* format, const Arg& arg, const Args&... args) noexcept
    {
        // Convert template parameters into array of addresses, performing
        // the necessary integer/float promotions
        void* addresses[sizeof...(Args) + 1];
        build_args(addresses, promoter<Arg>(arg), promoter<Args>(args)...);

        snprintf(format, addresses);
        // Call actual print function
        _printf1(format, addresses);
    }
}
