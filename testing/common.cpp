#include "ostest/ostest.h"
#include <string>
#include <__platform>
#include <ostream>

using namespace ostest;

__platform::size_t print(const std::string& string)
{
    __platform::size_t count = 0;
    __platform::__write(__platform::__stdout, string.c_str(), string.size(), count);
    return count;
}


void ostest::handleTestComplete(const TestInfo& test, const TestResult& result)
{
    static const char* passStr = "PASS";
    static const char* failStr = "FAIL";

    //std::cout << "[";

    // Print test result
    print("[");
    print(result ? passStr : failStr);
    print("] ");
    print("[");
    print(test.suiteName);
    print("::");
    print(test.testName);
    print("] at ");
    print(test.file);
    print(":");
    print(std::to_string(test.line));
    print("\n");
    //std::printf("[%s] [%s::%s] at %s:%i\n", result ? passStr : failStr, test.suiteName,
    //    test.testName, test.file, test.line);

	if (!result.succeeded())
	{
		auto enum_ = result.getAssertions();
		while (enum_.next())
		{
			const Assertion& result = enum_.current();
			if (result.passed()) continue;

            print("\t");
            print(result.file);
            print(":");
            print(std::to_string(result.line));
            print(": ");
            print(result.getMessage());
            print("\n");
			//std::printf("\t%s:%i: %s\n", result.file, result.line, result.getMessage());
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
