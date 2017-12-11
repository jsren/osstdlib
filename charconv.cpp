#include "charconv.hpp"

namespace std
{
    to_chars_result to_chars(char* first, char* last, char value, int base = 10);
}