#include "string.hpp"
#include "limits.hpp"

namespace std
{
	namespace __detail
	{
		const char* __empty_cstring = "\0";
		const wchar_t* __empty_wstring = L"\0";
		const char16_t* __empty_ustring = u"\0";
		const char32_t* __empty_Ustring = U"\0";


        string _int_to_string(unsigned long long value, bool negative)
        {
            constexpr auto maxDigits = numeric_limits<unsigned long long>::digits10;
            char buffer[maxDigits + 2];
            auto cursor = buffer + maxDigits + 1;
            *cursor-- = '\0';

            auto digit = value % 10;
            *cursor-- = '0' + digit;

            while (value >= 10) {
                value /= 10;
                auto digit = value % 10;
                *cursor-- = '0' + digit;
            }
            if (negative) { *cursor-- = '-'; }
            return string(cursor + 1);
        }
	}
}
