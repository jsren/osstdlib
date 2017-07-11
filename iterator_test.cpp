#include "array.hpp"
#include "iterator.hpp"
#include "memory.hpp"
#include "string.hpp"
#include <stdio.h>
#include <malloc.h>
#include <string.h>

void* operator new(unsigned long size) noexcept {
    return malloc(size);
}
void* operator new[](unsigned long size) noexcept {
    return malloc(size);
}
void operator delete(void* ptr) noexcept {
    free(ptr);
}
void operator delete[](void* ptr) noexcept {
    free(ptr);
}
void operator delete(void* ptr, long unsigned int) noexcept {
    free(ptr);
}
void operator delete[](void* ptr, long unsigned int) noexcept {
    free(ptr);
}


void* std::memcpy(void* dest, const void* src, size_t count) {
    return ::memcpy(dest, src, count);
}


int nums[] = { 1, 2, 3, 4 };


int main()
{
    auto iter1 = std::__detail::pointer_iterator<int>(nums);
    auto iter2 = std::__detail::pointer_iterator<int>(nums + sizeof(nums)/sizeof(nums[0]));

    for (; iter1 != iter2; iter1++) {
        printf("%d, ", *iter1);
    }
    printf("\n");

    const auto array = std::make_array(5, 6, 7, 8);
    for (auto& i : array) {
        static_assert(std::is_same_v<decltype(i), const int&>, "");
        printf("%d, ", i);
    }
    printf("\n");

    std::string test("hello, world!");
    printf("%s\n", test.c_str());

    for (auto& s : test) {
        printf("%c ", s);
    }
    printf("\n");

    return std::get<0>(array);
}
