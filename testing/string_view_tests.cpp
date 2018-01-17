#include "ostest/ostest.h"
#include <string_view>

TEST_SUITE(StringViewSuite);

TEST(StringViewSuite, EmptyView)
{
    using namespace std::string_view_literals;

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
