/* iterator_tests.cpp - (c) 2017 James Renwick */
#include <array.hpp>
#include <iterator.hpp>
#include <memory.hpp>
#include <string.hpp>
#include <tuple.hpp>
#include <type_traits.hpp>

#include "ostest/ostest.h"


static int nums[] = { 1, 2, 3, 4 };
static constexpr const size_t numsCount = sizeof(nums)/sizeof(nums[0]);

TEST_SUITE(IteratorSuite)

TEST(IteratorSuite, PointerIterator)
{
    auto iter = std::__detail::pointer_iterator<int>(nums);
    auto end = std::__detail::pointer_iterator<int>(nums + sizeof(nums)/sizeof(nums[0]));

    size_t i = 0;
    for (; iter != end; iter++, i++)
    {
        EXPECT_ALL(nums[i] == *iter);
		ASSERT_LT(i, numsCount);
    }
    EXPECT(i == numsCount);
}

TEST(IteratorSuite, ArrayIterator)
{
    const auto array = std::make_array(1, 2, 3, 4);
    size_t i = 0;

    for (auto& value : array)
    {
        EXPECT_ALL(value == nums[i]);
        i++;
    }
    EXPECT(i == numsCount);
}

TEST(IteratorSuite, ReverseAdapter)
{
	auto iter = std::__detail::pointer_iterator<int>(nums);
	auto end = std::__detail::pointer_iterator<int>(nums + sizeof(nums) / sizeof(nums[0]));

	auto riter = std::make_reverse_iterator(end);
	auto rend = std::make_reverse_iterator(iter);

	EXPECT((std::is_same<int, std::remove_reference_t<decltype(*riter)>>::value));

	size_t i = 0;
	for (; riter != rend; riter++, i++)
	{
		EXPECT_ALL(nums[numsCount - i - 1] == *riter);
		ASSERT_LT(i, numsCount);
	}
	EXPECT(i == numsCount);
}
