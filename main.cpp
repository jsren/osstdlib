#if 1
#include "string.hpp"
#include "tuple.hpp"
#include "_platform.hpp"

int main()
{
    std::string lala = "Hello, ";

    char buffer[124]{};

    auto count = __platform::__read(__platform::__stdin, buffer, sizeof(buffer));

    __platform::__write(__platform::__stdout, lala.c_str(), lala.size() * sizeof(char));
    __platform::__write(__platform::__stdout, buffer, count * sizeof(char));
    //__platform::__write(__platform::__stdout, "\n", 1 * sizeof(char));
}
#else

#include <stdio.h>

int main()
{
    char buffer[125];
    auto count = scanf("%s", buffer);
    buffer[count] = '\0';

    printf("Hello, %s\n", buffer);
    return 0;
}
#endif