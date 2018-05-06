#include "ostest/ostest.hpp"
#include <string>

TEST_SUITE(StringSuite)

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

#include <__platform>
static __platform::size_t print1(const char* msg, std::size_t size)
{
    __platform::size_t count = 0;
    __platform::__write(__platform::__stdout, msg, size, count);
    return count;
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
