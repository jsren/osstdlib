#include "ostest/ostest.hpp"
#include <vector>

using T1 = int;
using T2 = bool;


TEST_SUITE(VectorSuite);

TEST(VectorSuite, EmptyVector)
{
    std::vector<T1> v1{};
    EXPECT_ZERO(v1.size());
    EXPECT(v1.empty());
}

TEST(VectorSuite, VectorConstruction)
{
    constexpr std::size_t count1 = 5;
    constexpr T1 value1 = 53;
    std::vector<T1> v1{count1, value1};

    ASSERT_EQ(v1.size(), count1);
    EXPECT(!v1.empty());

    EXPECT_EQ(v1.front(), value1);
    EXPECT_EQ(v1.back(), value1);
    EXPECT_NEQ(&v1.front(), &v1.back());
    EXPECT_EQ(&v1.back() - &v1.front(), count1);

    for (std::size_t i = 0; i < v1.size(); i++)
    {
        EXPECT_EQ(v1[i], value1);
        EXPECT_EQ(&v1[i], &v1.data()[i]);
        EXPECT_EQ(&v1.at(i), &v1.data()[i]);
    }
}
