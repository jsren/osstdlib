#include "ostest/ostest.hpp"
#include <optional>

using T1 = bool;
using T2 = unsigned long;

struct no_copy {
    no_copy() = default;
    no_copy(const no_copy&) = delete;
    no_copy(no_copy&&) = default;
};

struct no_move {
    no_move() = default;
    no_move(const no_move&) = default;
    no_move(no_move&&) = delete;
};

struct construct_int {
    construct_int(int i) { (void)i; };
};

struct construct_int_nocopy {
    construct_int_nocopy(int i, no_copy c) { (void)i; (void)c; };
};


TEST_SUITE(OptionalSuite)

TEST(OptionalSuite, EmptyOptional)
{
    std::optional<T1> v1{};
    std::optional<T2> v2{};
    std::optional<no_copy> v3{};
    //static_assert(std::is_trivially_constructible<decltype(v1)>::value, "");
    //static_assert(std::is_trivially_constructible<decltype(v2)>::value, "");

    ASSERT(!v1);
    ASSERT(!v2);
    ASSERT(!v3);
    ASSERT(!v1.has_value());
    ASSERT(!v2.has_value());
    ASSERT(!v3.has_value());

    ASSERT_EQ(v1.value_or(T1(3)), T1(3));
    ASSERT_EQ(v2.value_or(T2(5)), T2(5));
}

TEST(OptionalSuite, OptionalWithValue)
{
    constexpr T1 c1{true};
    constexpr T2 c2{65};

    std::optional<T1> v1{c1};
    std::optional<T2> v2{c2};

    ASSERT(static_cast<bool>(v1));
    ASSERT(static_cast<bool>(v2));
    ASSERT(v1.has_value());
    ASSERT(v2.has_value());

    ASSERT_EQ(v1.value(), c1);
    ASSERT_EQ(v2.value(), c2);
    ASSERT_EQ(*v1, c1);
    ASSERT_EQ(*v2, c2);

    ASSERT_EQ(v1.value_or(false), c1);
    ASSERT_EQ(v2.value_or(T2(5)), c2);
}
