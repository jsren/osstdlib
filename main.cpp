#if 1
#include "string.hpp"
#include "tuple.hpp"
#include "_platform.hpp"

int main()
{
    std::string lala = "Hello, ";

    char buffer[124]{};

    //auto count = __platform::__read(__platform::__stdin, buffer, sizeof(buffer));

    __platform::__open_options opts{};
    opts.read = true;

    auto handle = __platform::__open("build.json", opts, __platform::__file_acl());

    __platform::ssize_t res = __platform::__read(handle, buffer, 123);
    __platform::__write(__platform::__stdout, buffer, 123);
    __platform::__write(__platform::__stdout, "\n", 1 * sizeof(char));

    __platform::__close(handle);
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
