#include "ostest/ostest.h"
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
