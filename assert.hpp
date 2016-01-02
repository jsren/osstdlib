#pragma once
#include "exception.hpp"

namespace std
{
    class AssertionException : public Exception
    {
    private:
        const static String assertMsg;

    public:
        const int line;
        const char *file;
    public:
        AssertionException(int line);
        AssertionException(String &msg, int line);
        AssertionException(String &msg, int line, const char *file);
    };
}

#define assert(cnd, msg) { if (cnd) ::std::throwException( }
