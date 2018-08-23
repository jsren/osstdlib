#include <string>
#include <limits>
#include <charconv>

extern "C++" {
namespace std
{
	namespace __detail
	{
        string int_to_string(signed long long value)
        {
            constexpr auto maxDigits = numeric_limits<signed long long>::digits10;
            char buffer[maxDigits + 1];

            auto res = to_chars(buffer, buffer + maxDigits, value);
            *res.ptr = '\0';

            return (static_cast<bool>(res.ec)) ? string() : string(buffer);
        }

        string uint_to_string(unsigned long long value)
        {
            constexpr auto maxDigits = numeric_limits<unsigned long long>::digits10;
            char buffer[maxDigits + 1];

            auto res = to_chars(buffer, buffer + maxDigits, value);
            *res.ptr = '\0';

            return (static_cast<bool>(res.ec)) ? string() : string(buffer);
        }
	}
}
}
