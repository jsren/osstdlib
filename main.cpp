#if 1
#include "string.hpp"
#include "tuple.hpp"
#include "_platform.hpp"

template<typename T>
void print_int(T value)
{
    if (value < 0) {
        __platform::__write(__platform::__stdout, "-", 1);
        value = -value;
    }
    while (value >= 10) {
        auto digit = value % 10;
        value /= 10;
        if (digit != 0) {
            char d = '0' + digit;
            __platform::__write(__platform::__stdout, &d, 1);
        }
    }
    char d = '0' + value;
    __platform::__write(__platform::__stdout, &d, 1);
}

int main()
{
    std::string lala = "Hello, ";

    char buffer[124]{};

    //auto count = __platform::__read(__platform::__stdin, buffer, sizeof(buffer));

    __platform::__open_options opts{};
    opts.read = true;
    opts.write = true;
    opts.create = true;
    opts.failExists = false;
    //opts.append = true;
    opts.truncate = false;

    __platform::__file_acl mode{};
    mode.userRead = true;
    mode.userWrite = true;
    mode.groupRead = true;
    mode.groupWrite = true;
    mode.othersRead = true;
    mode.othersWrite = true;

    auto handle = __platform::__open("build.json", opts, __platform::__file_acl());
    print_int(handle.value);
    __platform::__write(__platform::__stdout, "\n", 1 * sizeof(char));

    __platform::__seek(handle, 3, __platform::__seek_whence::start);

    std::string msg{"Hello, World!"};
    auto res = __platform::__write(handle, msg.c_str(), msg.size());
    print_int(res);
    __platform::__write(__platform::__stdout, "\n", 1 * sizeof(char));

    //__platform::ssize_t res = __platform::__read(handle, buffer, 123);
    //__platform::__write(__platform::__stdout, buffer, 123);

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
