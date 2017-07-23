#include "array.hpp"
#include "iterator.hpp"
#include "memory.hpp"
#include "string.hpp"
#include "tuple.hpp"
#include "type_traits.hpp"

#include <stdio.h>
#include <malloc.h>
#include <string.h>

void* operator new(size_t size) noexcept {
    return malloc(size);
}
void* operator new[](size_t size) noexcept {
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

int nums[] = { 1, 2, 3, 4 };

template<typename T>
T identity(T value, bool) {
	return std::forward<T>(value);
}

std::tuple<bool, int> doTheThing()
{
	return std::tuple<bool, int>(false, -3);
}

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
	printf("%d\n", std::get<0>(array));

    std::string test("hello, world!");
    printf("%s\n", test.c_str());

    for (auto& s : test) {
        printf("%c ", s);
    }
    printf("\n");

	std::tuple<int, bool> t{ 3, false };

	t = std::tuple<short, bool>{ 4, false };
	std::tuple<int, bool> haha{ 450, false };

	t = haha;
	printf("%d\n", std::get<0>(t));

	bool success;
	int rc;

	std::tie(success, rc) = doTheThing();
	printf("%d/%d\n", success, rc);

	std::tie(std::ignore, std::ignore) = doTheThing();

	return 0;
}
