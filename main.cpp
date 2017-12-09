#include "string.hpp"
#include "tuple.hpp"
#include "abi.hpp"

int main()
{
    std::string lala = "Hello\n";

    return abi::__write(abi::__stdout, lala.data(), lala.size() * sizeof(char));
}
