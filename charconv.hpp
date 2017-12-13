#include "system_error.hpp"

namespace std
{
    enum class chars_format
    {
        scientific = 1,
        fixed      = 2,
        hex        = 4,
        general    = 3
    };

    struct to_chars_result
    {
        char* ptr;
        errc ec;
    };

    namespace __detail
    {
        static constexpr const char characters[] = "0123456789abcdefghijklmnopqrstuvwxyz";

        to_chars_result _to_chars(unsigned long long value, bool negative,
            int base, char* first, char* last)
        {
            if (base < 2 || base > 36) return to_chars_result{last, errc::value_too_large};

            // Perform to-string as usual
            constexpr auto maxDigits = numeric_limits<unsigned long long>::digits10 + 1;
            char buffer[maxDigits];
            auto end = buffer + maxDigits - 1;
            auto cursor = end;

            auto digit = value % base;
            *cursor = characters[digit];

            while (value >= base) {
                value /= base;
                auto digit = value % base;
                *--cursor = characters[digit];
            }
            if (negative) { *--cursor = '-'; }

            // Now copy
            for (; cursor != end && first != last; cursor++, first++) {
                *first = *cursor;
            }
            return {first, static_cast<errc>(false)};
        }
    }

    to_chars_result to_chars(char* first, char* last, char value, int base = 10);
}
