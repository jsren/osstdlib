#if 1
#include <string>
#include <tuple>
#include <__platform>

namespace std
{
}


auto print(const std::string& string)
{
     return __platform::__write(__platform::__stdout, string.c_str(), string.size());
}


int main()
{
    char buffer[124]{};
    //auto count = __platform::__read(__platform::__stdin, buffer, sizeof(buffer));

   /* __platform::__open_options opts{};
    opts.read = true;
    opts.write = true;

    __platform::__file_acl mode{};
    mode.userRead = true;
    mode.userWrite = true;
    mode.groupRead = true;
    mode.groupWrite = true;
    mode.othersRead = true;
    mode.othersWrite = true;

    auto handle = __platform::__open("test.txt", opts, __platform::__file_acl());
    auto read1 = __platform::__read(handle, buffer, sizeof(buffer));
    auto seek1 = __platform::__seek(handle, 0, __platform::__seek_whence::end);
    buffer[read1] = '1';
    auto write2 = __platform::__write(handle, buffer, read1 + 1);
    __platform::__close(handle);

    std::string msg{"Handle 1: "};
    msg += std::to_string(handle.value);
    msg += "\n";

    std::string wmsg{"Wrote "};
    wmsg += std::to_string(read1 + 1);
    wmsg += " bytes\n";
    auto write3 = print(wmsg);

    msg += "\nRead 1: "; msg += std::to_string(read1);
    msg += "\nSeek 1: "; msg += std::to_string(seek1);
    msg += "\nWrite 2: "; msg += std::to_string(write2);
    msg += "\nWrite 3: "; msg += std::to_string(write3);
    msg += "\n";
    print(msg);*/

    print(std::to_string(0) += "\n");
    print(std::to_string(1) += "\n");
    print(std::to_string(9) += "\n");
    print(std::to_string(10) += "\n");
    print(std::to_string(11) += "\n");
    print(std::to_string(99) += "\n");
    print(std::to_string(100) += "\n");
    print(std::to_string(101) += "\n");
    print(std::to_string(109) += "\n");
    print(std::to_string(110) += "\n");
    print(std::to_string(111) += "\n");
    print(std::to_string(999) += "\n");
    print(std::to_string(1000) += "\n");
    print(std::to_string(-0) += "\n");
    print(std::to_string(-1) += "\n");
    print(std::to_string(-9) += "\n");
    print(std::to_string(-10) += "\n");
    print(std::to_string(-11) += "\n");
    print(std::to_string(-99) += "\n");
    print(std::to_string(-100) += "\n");
    print(std::to_string(-101) += "\n");
    print(std::to_string(-109) += "\n");
    print(std::to_string(-110) += "\n");
    print(std::to_string(-111) += "\n");
    print(std::to_string(-999) += "\n");
    print(std::to_string(-1000) += "\n");
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
