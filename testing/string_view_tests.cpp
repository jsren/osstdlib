#include "ostest/ostest.h"
#include <string_view>

using namespace std::string_view_literals;


TEST_SUITE(StringViewSuite)

TEST(StringViewSuite, EmptyView)
{
    std::string_view view{};

    EXPECT_ZERO(view.length());
    EXPECT_ZERO(view.size());
    EXPECT_EQ(view.begin(), view.end());
    EXPECT_EQ(view.cbegin(), view.cend());
    EXPECT_EQ(view, std::string_view{});
    EXPECT_EQ(view.rbegin(), view.rend());
    EXPECT(view.empty());
    EXPECT_EQ(view, ""sv);
}

TEST(StringViewSuite, SingleCharView)
{
    static const char* v1 = "i";

    std::string_view a1{v1};
    EXPECT_EQ(a1.begin() + 1, a1.end());
    EXPECT_EQ(a1[0], v1[0]);
    EXPECT_EQ(a1.at(0), v1[0]);
    EXPECT_EQ(a1.front(), v1[0]);
    EXPECT_EQ(a1.back(), v1[0]);
    EXPECT_EQ(a1.data()[0], v1[0]);
    EXPECT_EQ(a1.length(), 1);
    EXPECT_EQ(a1.size(), 1);
    EXPECT(!a1.empty());

    std::string_view a2{v1, 1};
    EXPECT_EQ(a2.begin() + 1, a2.end());
    EXPECT_EQ(a2[0], v1[0]);
    EXPECT_EQ(a2.at(0), v1[0]);
    EXPECT_EQ(a2.front(), v1[0]);
    EXPECT_EQ(a2.back(), v1[0]);
    EXPECT_EQ(a2.data()[0], v1[0]);
    EXPECT_EQ(a2.length(), 1);
    EXPECT_EQ(a2.size(), 1);
    EXPECT(!a2.empty());

    EXPECT_EQ(a1, a2);
}

TEST(StringViewSuite, MultiCharView)
{
    static const char s1[] = ".abcD345\t";
    static const constexpr std::size_t l1 = sizeof(s1) - 1;

    std::string_view a1{s1};

    for (std::size_t i = 0; i < l1; i++)
    {
        EXPECT_EQ(a1[i], s1[i]);
        EXPECT_EQ(a1.at(i), s1[i]);
    }

    std::size_t i = 0;
    for (auto iter = a1.cbegin(); iter != a1.cend(); iter++, i++)
    {
        ASSERT_LT(i, l1);
        EXPECT_EQ(*iter, s1[i]);
    }

    EXPECT_EQ(a1, std::string_view{s1});

    EXPECT_EQ(a1.front(), s1[0]);
    EXPECT_EQ(a1.back(), s1[l1 - 1]);
    EXPECT_EQ(a1.data()[0], s1[0]);
    EXPECT_EQ(a1.length(), l1);
    EXPECT_EQ(a1.size(), l1);
    EXPECT(!a1.empty());
}

TEST(StringViewSuite, RemovePrefix)
{
    static const char s1[] = "";
    static const char s2[] = "A";

    std::string_view a1{s1};
    a1.remove_prefix(0);

    EXPECT_ZERO(a1.length());
    EXPECT_ZERO(a1.size());
    EXPECT_EQ(a1.begin(), a1.end());
    EXPECT_EQ(a1.cbegin(), a1.cend());
    EXPECT_EQ(a1, std::string_view{});
    EXPECT_EQ(a1.rbegin(), a1.rend());
    EXPECT(a1.empty());
    EXPECT_EQ(a1, ""sv);

    std::string_view a2{s1};
    a2.remove_suffix(0);

    EXPECT_ZERO(a2.length());
    EXPECT_ZERO(a2.size());
    EXPECT_EQ(a2.begin(), a2.end());
    EXPECT_EQ(a2.cbegin(), a2.cend());
    EXPECT_EQ(a2, std::string_view{});
    EXPECT_EQ(a2.rbegin(), a2.rend());
    EXPECT(a2.empty());
    EXPECT_EQ(a2, ""sv);


    std::string_view a3{s2};
    a3.remove_prefix(1);

    EXPECT_ZERO(a3.length());
    EXPECT_ZERO(a3.size());
    EXPECT_EQ(a3.begin(), a3.end());
    EXPECT_EQ(a3.cbegin(), a3.cend());
    EXPECT_EQ(a3, std::string_view{});
    EXPECT_EQ(a3.rbegin(), a3.rend());
    EXPECT(a3.empty());
    EXPECT_EQ(a3, ""sv);

    std::string_view a4{s2};
    a4.remove_suffix(1);

    EXPECT_ZERO(a4.length());
    EXPECT_ZERO(a4.size());
    EXPECT_EQ(a4.begin(), a4.end());
    EXPECT_EQ(a4.cbegin(), a4.cend());
    EXPECT_EQ(a4, std::string_view{});
    EXPECT_EQ(a4.rbegin(), a4.rend());
    EXPECT(a4.empty());
    EXPECT_EQ(a4, ""sv);

    static const char s3[] = "abcDEF123";

    std::string_view a5{s3};
    a5.remove_prefix(3);

    for (std::size_t i = 0; i < sizeof(s3) - 4; i++) {
        EXPECT_EQ(a5[i], s3[i + 3]);
    }
    EXPECT_EQ(a5.length(), sizeof(s3) - 4);
    EXPECT_EQ(a5.size(), sizeof(s3) - 4);
    EXPECT_EQ(a5, std::string_view{s3 + 3});

    a5.remove_suffix(3);

    ASSERT_EQ(a5.length(), sizeof(s3) - 7);
    ASSERT_EQ(a5.size(), sizeof(s3) - 7);
    for (std::size_t i = 0; i < sizeof(s3) - 7; i++) {
        EXPECT_EQ(a5[i], s3[i + 3]);
    }
    EXPECT_EQ(a5, (std::string_view{s3 + 3, sizeof(s3) - 7}));
}
