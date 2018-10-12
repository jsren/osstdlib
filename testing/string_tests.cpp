#include "ostest/ostest.hpp"
#include <string>

TEST_SUITE(StringSuite)

/*#include <__platform>
static __platform::size_t print1(const char* msg, std::size_t size)
{
    __platform::size_t count = 0;
    __platform::__write(__platform::__stdout, msg, size, count);
    return count;
}*/

namespace std
{
    int printf(const char* format) noexcept
    {

    }



    int _snprintf(const char* format, const void** args) noexcept
    {
        auto len = strlen(format);
        int len_out = 0;
        size_t arg_index = 0;
        for (size_t i = 0; i < len; i++)
        {
            if (format[i] == '%' && ++i < len)
            {
                bool flag_plus = false;
                bool flag_space = false;
                bool flag_hash = false;

                // Skip left-align flag and zero flag
                for (; i < len; i++)
                {
                    if (format[i] == '-') continue;
                    else if (format[i] == '0') continue;
                    else if (format[i] == '+') {
                        flag_plus = true;
                    }
                    else if (format[i] == ' ') {
                        flag_space = true;
                    }
                    else if (format[i] == '#') {
                        flag_hash = true;
                    }
                    else break;
                }
                if (i == len) break;

                

                if (format[i] == 'i' || format[i] == 'd')
                {
                    int value = *reinterpret_cast<const int*>(args[arg_index++]);
                    
                }
                else if (format[i] == )

            }
            else len_out++;
        }
    }

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
    struct promoter<wchar_t>
    {
        int value;
        promoter(wchar_t v) : value((int)v) { }
    };

    template<>
    struct promoter<wchar_t>
    {
        int value;
        promoter(wchar_t v) : value(v) { }
    };

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
        // Call actual print function
        _printf1(format, addresses);
    }

}

TEST(StringSuite, EmptyString)
{
    using namespace std::literals;

    std::string s1{};
    EXPECT(s1.empty());
    EXPECT_EQ(s1.cbegin(), s1.cend());
    EXPECT_EQ(s1.crbegin(), s1.crend());
    EXPECT_EQ(s1.length(), 0);
    EXPECT_EQ(s1.size(), 0);
    EXPECT_EQ(s1, "");
    EXPECT_EQ(s1, ""s);
    EXPECT_EQ(s1, std::string{});
    EXPECT_EQ(s1.substr(0, 0), s1);
    EXPECT_EQ(s1.c_str()[0], '\0');
    EXPECT_EQ(s1.data()[0], '\0');
}

TEST(StringSuite, SingleCharString)
{
    using namespace std::literals;
    static const char t1[] = "a";

    std::string s1{t1};
    EXPECT_EQ(s1.at(0), t1[0]);
    EXPECT_EQ(s1[0], t1[0]);
    EXPECT_EQ(s1, "a"s);
    EXPECT_EQ(s1.back(), t1[0]);
    EXPECT_EQ(s1.front(), t1[0]);
    EXPECT_EQ(*s1.begin(), t1[0]);
    EXPECT_NEQ(s1.begin(), s1.end());
    EXPECT_EQ(s1.begin() + 1, s1.end());

    EXPECT_EQ(s1.c_str()[0], t1[0]);
    EXPECT_EQ(s1.c_str()[1], t1[1]);
    EXPECT_EQ(s1.data()[0], t1[0]);
    EXPECT_EQ(s1.data()[1], t1[1]);
    EXPECT(!s1.empty());
    EXPECT_EQ(s1.length(), 1);
    EXPECT_EQ(s1.size(), 1);
}

TEST(StringSuite, MultiCharString)
{
    using namespace std::literals;
    static const char t1[] = "\x21\tdeadbe";
    static const std::size_t l1 = sizeof(t1) - 1;

    std::string s1{t1};

    for (std::size_t i = 0; i < l1; i++)
    {
        EXPECT_EQ(s1.at(i), t1[i]);
        EXPECT_EQ(s1[i], t1[i]);
    }
    EXPECT_EQ(s1, "\x21\tdeadbe"s);
    EXPECT_EQ(s1.back(), t1[l1 - 1]);

    EXPECT_EQ(s1.front(), t1[0]);
    EXPECT_EQ(*s1.begin(), t1[0]);
    EXPECT_NEQ(s1.begin(), s1.end());
    EXPECT_EQ(s1.begin() + l1, s1.end());

    for (std::size_t i = 0; i < sizeof(t1); i++)
    {
        EXPECT_EQ(s1.c_str()[i], t1[i]);
        EXPECT_EQ(s1.data()[i], t1[i]);
    }
    EXPECT(!s1.empty());
    EXPECT_EQ(s1.length(), l1);
    EXPECT_EQ(s1.size(), l1);
}

/*TEST(StringSuite, AppendChar)
{
    std::string s1{};
    std::string s2{"a"};
    std::string s3{"deadbeef"};

    for (std::size_t i = 0; i < l1; i++)
    {
        EXPECT_EQ(s1.at(i), t1[i]);
        EXPECT_EQ(s1[i], t1[i]);
    }
    EXPECT_EQ(s1, "\x21\tdeadbe"s);
    EXPECT_EQ(s1.back(), t1[l1 - 1]);

    EXPECT_EQ(s1.front(), t1[0]);
    EXPECT_EQ(*s1.begin(), t1[0]);
    EXPECT_NEQ(s1.begin(), s1.end());
    EXPECT_EQ(s1.begin() + l1, s1.end());

    for (std::size_t i = 0; i < sizeof(t1); i++)
    {
        EXPECT_EQ(s1.c_str()[i], t1[i]);
        EXPECT_EQ(s1.data()[i], t1[i]);
    }
    EXPECT(!s1.empty());
    EXPECT_EQ(s1.length(), l1);
    EXPECT_EQ(s1.size(), l1);
}*/
