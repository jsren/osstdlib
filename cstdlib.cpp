#include <__platform>

namespace std
{
    void abort()
    {
        __platform::__exit(1);
    }
}
