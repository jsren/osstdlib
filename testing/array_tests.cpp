#include "ostest/ostest.hpp"
#include <array>
#include <__platform>
#include <string>

using T1 = bool;
using T2 = unsigned long;

TEST_SUITE(ArraySuite)

__platform::size_t print(const std::string& string);

TEST(ArraySuite, EmptyArray)
{
    std::array<T2, 0> a1{};
    EXPECT_EQ(a1.size(), 0);
    EXPECT(a1.empty());
    EXPECT_LTEQ(sizeof(a1), 1);
    EXPECT_EQ(a1.begin(), a1.end());
    EXPECT_EQ(a1, (std::array<T2, 0>()));
    EXPECT(!(a1 != std::array<T2, 0>()));
    EXPECT(!(a1 < std::array<T2, 0>()));
    EXPECT(!(a1 > std::array<T2, 0>()));
    EXPECT_LTEQ(a1, (std::array<T2, 0>()));
    EXPECT_GTEQ(a1, (std::array<T2, 0>()));
}
