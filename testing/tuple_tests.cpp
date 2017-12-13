/* tuple_tests.cpp - (c) 2017 James Renwick */
#include "ostest/ostest.h"
#include <tuple>

TEST_SUITE(TupleSuite)

constexpr const int testValue = 0xC0FFEE;
struct test_struct
{
	int i = testValue;

	test_struct() = default;
	test_struct(int v) : i(v) { }

	bool operator ==(const test_struct& other) {
		return other.i == i;
	}
};

using T1 = bool;
using T2 = int;
using T3 = long;
using T4 = test_struct;
using T5 = const char *;
using T6 = float;

#define V1 T1{true}
#define V2 T2{123456}
#define V3 T3{-1}
#define V4 T4{0xEEFF0C}
#define V5 T5{"hello"}
#define V6 T6{0.125}


TEST(TupleSuite, EmptyTuple)
{
	std::tuple<> tuple{};
	std::tuple<std::tuple<>> tuple2{};
}

TEST(TupleSuite, SingleTuple)
{
	std::tuple<bool> tuple{};
}

TEST(TupleSuite, ComponentTypes)
{
	std::tuple<T1, T2, T3, T4, T5, T6> tuple1{};

	using R1 = std::decay_t<decltype(std::get<0>(tuple1))>;
	using R2 = std::decay_t<decltype(std::get<1>(tuple1))>;
	using R3 = std::decay_t<decltype(std::get<2>(tuple1))>;
	using R4 = std::decay_t<decltype(std::get<3>(tuple1))>;
	using R5 = std::decay_t<decltype(std::get<4>(tuple1))>;
	using R6 = std::decay_t<decltype(std::get<5>(tuple1))>;

	EXPECT((std::is_same<T1, R1>::value));
	EXPECT((std::is_same<T2, R2>::value));
	EXPECT((std::is_same<T3, R3>::value));
	EXPECT((std::is_same<T4, R4>::value));
	EXPECT((std::is_same<T5, R5>::value));
	EXPECT((std::is_same<T6, R6>::value));
}

TEST(TupleSuite, DefaultConstruction)
{
	std::tuple<test_struct> tuple1{};
	EXPECT(std::get<0>(tuple1).i == testValue);

	std::tuple<T1, T2, T3, T4, T5, T6> tuple2{};

	EXPECT(std::get<0>(tuple2) == T1{});
	EXPECT(std::get<1>(tuple2) == T2{});
	EXPECT(std::get<2>(tuple2) == T3{});
	EXPECT(std::get<3>(tuple2) == T4{});
	EXPECT(std::get<4>(tuple2) == T5{});
	EXPECT(std::get<5>(tuple2) == T6{});
}

TEST(TupleSuite, DirectConstructor)
{
	std::tuple<T1, T2, T3, T4, T5, T6> tuple1(V1, V2, V3, V4, V5, V6);

	EXPECT(std::get<0>(tuple1) == V1);
	EXPECT(std::get<1>(tuple1) == V2);
	EXPECT(std::get<2>(tuple1) == V3);
	EXPECT(std::get<3>(tuple1) == V4);
	EXPECT(std::get<4>(tuple1) == V5);
	EXPECT(std::get<5>(tuple1) == V6);
}

TEST(TupleSuite, CreateFunctions)
{
	auto tuple1 = std::make_tuple();
	EXPECT_ZERO(std::tuple_size<decltype(tuple1)>::value);

	auto tuple2 = std::make_tuple(T1{}, T2{}, T3{}, T4{}, T5{}, T6{});
	EXPECT(std::get<0>(tuple2) == T1{});
	EXPECT(std::get<1>(tuple2) == T2{});
	EXPECT(std::get<2>(tuple2) == T3{});
	EXPECT(std::get<3>(tuple2) == T4{});
	EXPECT(std::get<4>(tuple2) == T5{});
	EXPECT(std::get<5>(tuple2) == T6{});

	auto tuple3 = std::make_tuple(V1, V2, V3, V4, V5, V6);
	EXPECT(std::get<0>(tuple3) == V1);
	EXPECT(std::get<1>(tuple3) == V2);
	EXPECT(std::get<2>(tuple3) == V3);
	EXPECT(std::get<3>(tuple3) == V4);
	EXPECT(std::get<4>(tuple3) == V5);
	EXPECT(std::get<5>(tuple3) == V6);
}

TEST(TupleSuite, TupleSize)
{
	EXPECT(std::tuple_size<std::tuple<>>::value == 0);
	EXPECT(std::tuple_size<std::tuple<bool>>::value == 1);
	EXPECT((std::tuple_size<std::tuple<bool, int>>::value) == 2);
}

TEST(TupleSuite, TupleAssign)
{
	std::tuple<T1, T2, T3, T4, T5, T6> tuple1(V1, V2, V3, V4, V5, V6);
	std::tuple<T1, T2, T3, T4, T5, T6> tuple2{};

	// Ensure that at least one of these values is not the default
	ASSERT(V1 != T1{} || V2 != T2{} || V3 != T3{} || !(V4 == T4{}) || V5 != T5{} || V6 != T6{});

	tuple2 = tuple1;

	EXPECT(std::get<0>(tuple1) == std::get<0>(tuple2));
	EXPECT(std::get<1>(tuple1) == std::get<1>(tuple2));
	EXPECT(std::get<2>(tuple1) == std::get<2>(tuple2));
	EXPECT(std::get<3>(tuple1) == std::get<3>(tuple2));
	EXPECT(std::get<4>(tuple1) == std::get<4>(tuple2));
	EXPECT(std::get<5>(tuple1) == std::get<5>(tuple2));
}


std::tuple<T1, T2, T3, T4, T5, T6> example_function(T1 v1, T2 v2, T3 v3, T4 v4, T5 v5, T6 v6)
{
	return std::make_tuple(v1, v2, v3, v4, v5, v6);
}

TEST(TupleSuite, TupleTie)
{
	// Ensure that those values not ignored differ from their default
	ASSERT(V2 != T2{} || !(V4 == T4{}) || V6 != T6{});

	// Ignore all
	std::tie(std::ignore, std::ignore, std::ignore, std::ignore, std::ignore, std::ignore)
		= example_function(V1, V2, V3, V4, V5, V6);

	// Tie all
	{
		T1 v1{};
		T2 v2{};
		T3 v3{};
		T4 v4{};
		T5 v5{};
		T6 v6{};

		std::tie(v1, v2, v3, v4, v5, v6) = example_function(V1, V2, V3, V4, V5, V6);
		EXPECT_EQ(v1, V1);
		EXPECT_EQ(v2, V2);
		EXPECT_EQ(v3, V3);
		EXPECT_EQ(v4, V4);
		EXPECT_EQ(v5, V5);
		EXPECT_EQ(v6, V6);
	}

	// Tie some
	{
		T2 v2{};
		T4 v4{};
		T6 v6{};

		std::tie(std::ignore, v2, std::ignore, v4, std::ignore, v6) = example_function(V1, V2, V3, V4, V5, V6);
		EXPECT_EQ(v2, V2);
		EXPECT_EQ(v4, V4);
		EXPECT_EQ(v6, V6);
	}
}

TEST(TupleSuite, TupleApply)
{
	std::tuple<T1, T2, T3, T4, T5, T6> tuple1(V1, V2, V3, V4, V5, V6);

	auto tuple2 = std::apply(example_function, tuple1);

	EXPECT_EQ(std::get<0>(tuple1), std::get<0>(tuple2));
	EXPECT_EQ(std::get<1>(tuple1), std::get<1>(tuple2));
	EXPECT_EQ(std::get<2>(tuple1), std::get<2>(tuple2));
	EXPECT_EQ(std::get<3>(tuple1), std::get<3>(tuple2));
	EXPECT_EQ(std::get<4>(tuple1), std::get<4>(tuple2));
	EXPECT_EQ(std::get<5>(tuple1), std::get<5>(tuple2));
}
