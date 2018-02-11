#include <string>
#include <limits>
#include <charconv>

namespace std
{
	namespace __detail
	{
		char* __empty_cstring = "\0";
		wchar_t* __empty_wstring = L"\0";
		char16_t* __empty_ustring = u"\0";
		char32_t* __empty_Ustring = U"\0";


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
