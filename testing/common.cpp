#include <common>
#include <cstdio>

using namespace ostest;

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

void ostest::handleTestComplete(const TestInfo& test, const TestResult& result)
{
    static const char* passStr = "PASS";
    static const char* failStr = "FAIL";

    // Print test result
    std::printf("[%s] [%s::%s] at %s:%i\n", result ? passStr : failStr, test.suiteName,
        test.testName, test.file, test.line);

	if (!result.succeeded())
	{
		auto enum_ = result.getAssertions();
		while (enum_.next())
		{
			const Assertion& result = enum_.current();
			if (result.passed()) continue;

			std::printf("\t%s:%i: %s\n", result.file, result.line, result.getMessage());
		}
	}
}

int main()
{
    auto tests = ostest::getUnitTests();
    while (tests.next()) {
        TestRunner(tests.current()).run();
    }
    return 0;
}
