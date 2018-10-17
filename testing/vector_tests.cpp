#include "ostest/ostest.hpp"
#include <vector>
#include <string>

using T1 = int;

#include <__platform>
static __platform::size_t print1(const char* msg, std::size_t size)
{
    __platform::size_t count = 0;
    __platform::__write(__platform::__stdout, msg, size, count);
    return count;
}

TEST_SUITE(VectorSuite)

TEST(VectorSuite, EmptyVector)
{
    std::vector<T1> v1{};
    EXPECT_ZERO(v1.size());
    EXPECT(v1.empty());
    EXPECT_NONZERO(v1.max_size());
    EXPECT_NONZERO(v1.capacity());
}

TEST(VectorSuite, VectorConstruction)
{
    constexpr std::size_t count1 = 5;
    constexpr T1 value1 = 53;
    std::vector<T1> v1(count1, value1);

    ASSERT_EQ(v1.size(), count1);
    ASSERT_GTEQ(v1.capacity(), count1);
    EXPECT(!v1.empty());

    EXPECT_EQ(v1.data(), &v1.front());
    EXPECT_EQ(v1.front(), value1);
    EXPECT_EQ(v1.back(), value1);
    EXPECT_NEQ(&v1.front(), &v1.back());
    EXPECT_EQ(&v1.back() - &v1.front() + 1, count1);

    for (std::size_t i = 0; i < v1.size(); i++)
    {
        EXPECT_EQ(v1[i], value1);
        EXPECT_EQ(&v1[i], &v1.data()[i]);
        EXPECT_EQ(v1.at(i), v1.data()[i]);
        EXPECT_EQ(&v1.at(i), &v1.data()[i]);
    }

    // Construction from initialiser list
    std::vector<T1> v2{count1, value1};

    ASSERT_EQ(v2.size(), 2);
    ASSERT_GTEQ(v2.capacity(), 2);
    EXPECT(!v2.empty());

    EXPECT_EQ(v2.data(), &v2.front());
    EXPECT_EQ(v2.front(), count1);
    EXPECT_EQ(v2.back(), value1);
    EXPECT_NEQ(&v2.front(), &v2.back());
    EXPECT_EQ(&v2.back() - &v2.front() + 1, 2);

    EXPECT_EQ(v2[0], count1);
    EXPECT_EQ(&v2[0], &v2.data()[0]);
    EXPECT_EQ(v2.at(0), v2.data()[0]);
    EXPECT_EQ(&v2.at(0), &v2.data()[0]);
    EXPECT_EQ(v2[1], value1);
    EXPECT_EQ(&v2[1], &v2.data()[1]);
    EXPECT_EQ(v2.at(1), v2.data()[1]);
    EXPECT_EQ(&v2.at(1), &v2.data()[1]);

    // Construction with n default-constructed elements
    std::vector<T1> v3(count1);

    ASSERT_EQ(v3.size(), count1);
    ASSERT_GTEQ(v3.capacity(), count1);
    EXPECT(!v3.empty());

    EXPECT_EQ(v3.data(), &v3.front());
    EXPECT_EQ(v3.front(), T1());
    EXPECT_EQ(v3.back(), T1());
    EXPECT_NEQ(&v3.front(), &v3.back());
    EXPECT_EQ(&v3.back() - &v3.front() + 1, count1);

    for (std::size_t i = 0; i < v3.size(); i++)
    {
        EXPECT_EQ(v3[i], T1());
        EXPECT_EQ(&v3[i], &v3.data()[i]);
        EXPECT_EQ(v3.at(i), v3.data()[i]);
        EXPECT_EQ(&v3.at(i), &v3.data()[i]);
    }

    // Copy-construction
    std::vector<T1> v4(v2);

    EXPECT_EQ(v4.front(), v2.front());
    EXPECT_EQ(v4.back(), v2.back());
    EXPECT_NEQ(v4.data(), v2.data());
    EXPECT_EQ(v4.empty(), v2.empty());
    ASSERT_EQ(v4.size(), v2.size());
    ASSERT_EQ(v4.max_size(), v2.max_size());
    ASSERT_GTEQ(v4.capacity(), v4.size());

    for (std::size_t i = 0; i < v4.size(); i++)
    {
        EXPECT_EQ(v4[i], v2[i]);
        EXPECT_NEQ(&v4[i], &v2[i]);
        EXPECT_EQ(v4.at(i), v2.at(i));
        EXPECT_NEQ(&v4.at(i), &v2.at(i));
    }
}

TEST(VectorSuite, PushBack100)
{
    std::vector<std::size_t> v1{};

    // Fill SBO capacity
    void* startAddress = v1.data();
    auto startCapacity = v1.capacity();
    EXPECT(startAddress >= &v1 && startAddress < &v1 + sizeof(v1));

    std::size_t reallocs = 0;

    for (std::size_t i = 0; i < v1.capacity(); i++)
    {
        void* startAddress = v1.data();
        auto startCapacity = v1.capacity();

        v1.push_back(i);

        if (startAddress != v1.data() || startCapacity != v1.capacity()) {
            reallocs++;
        }
    }
    EXPECT_ZERO(reallocs);
    EXPECT_EQ(startAddress, v1.data());
    ASSERT_EQ(v1.capacity(), startCapacity);
    for (std::size_t i = 0; i < v1.capacity(); i++)
    {
        EXPECT_EQ(v1[i], i);
    }

    // Begin reallocation
    auto startIndex = v1.size();
    for (std::size_t i = 0; i < 100; i++)
    {
        void* startAddress = v1.data();
        auto startCapacity = v1.capacity();

        v1.push_back(i + startIndex);

        if (startAddress != v1.data() || startCapacity != v1.capacity()) {
            reallocs++;
        }
    }

    // Simluate the number of reallocs necessary
    std::size_t expectedReallocs = 0;
    for (auto cap = startIndex;
         cap < 100;
         cap += cap / std::_config::vector_realloc_factor,
         expectedReallocs++) { }

    EXPECT_LTEQ(reallocs, expectedReallocs);

    for (std::size_t i = 0; i < 100; i++)
    {
        EXPECT_EQ(v1[startIndex + i], i);
    }
}
