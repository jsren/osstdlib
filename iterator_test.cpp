#include "array.hpp"
#include "iterator.hpp"
#include "memory.hpp"
#include <stdio.h>

int nums[] = { 1, 2, 3, 4 };

int main()
{
    auto iter1 = std::__detail::pointer_iterator<int>(nums);
    auto iter2 = std::__detail::pointer_iterator<int>(nums + sizeof(nums)/sizeof(nums[0]));

    for (; iter1 != iter2; iter1++) {
        printf("%d, ", *iter1);
    }
    printf("\n");

    auto array = std::make_array(5, 6, 7, 8);
    for (auto& i : array) {
        printf("%d, ", i);
    }
    printf("\n");    

    return std::get<0>(array);
}
