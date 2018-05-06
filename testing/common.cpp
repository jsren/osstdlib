#include "ostest/ostest.hpp"
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

    // Print test result
    print("[");
    print(result ? passStr : failStr);
    print("] ");
    print("[");
    print(test.suite.name);
    print("::");
    print(test.name);
    print("] at ");
    print(test.file);
    print(":");
    print(std::to_string(test.line));
    print("\n");
    //std::printf("[%s] [%s::%s] at %s:%i\n", result ? passStr : failStr, test.suiteName,
    //    test.testName, test.file, test.line);

	if (!result.succeeded())
	{
		for (auto& assert : result.getAssertions())
        {
			if (assert.passed()) continue;

            print("\t");
            print(assert.file);
            print(":");
            print(std::to_string(assert.line));
            print(": ");
            print(assert.getMessage());
            print("\n");
			//std::printf("\t%s:%i: %s\n", result.file, result.line, result.getMessage());
		}
	}
}

int main()
{
    for (auto& suiteInfo : ostest::getSuites())
    {
        auto suite = suiteInfo.getSingletonSmartPtr();
        for (auto& test : suiteInfo.tests()) {
            TestRunner(*suite, test).run();
        }
    }
    return 0;
}
