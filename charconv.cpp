#include <charconv>

namespace std
{
    namespace __detail
    {
        static constexpr const char characters[] = "0123456789abcdefghijklmnopqrstuvwxyz";

        to_chars_result _to_chars(unsigned long long value, bool negative,
            unsigned base, char* first, char* last)
        {
            if (base < 2 || base > 36) return to_chars_result{last, errc::value_too_large};

            // Perform to-string as usual
            constexpr auto maxDigits = numeric_limits<unsigned long long>::digits10 + 1;
            char buffer[maxDigits];
            auto end = buffer + maxDigits;
            auto cursor = end - 1;

            auto digit = value % base;
            *cursor = characters[digit];

            while (value >= base) {
                value /= base;
                auto digit = value % base;
                *--cursor = characters[digit];
            }
            if (negative) { *--cursor = '-'; }

            // If exceeds available buffer, return error
            if (end - cursor > last - first) {
                return to_chars_result{last, errc::value_too_large};
            }

            // Now copy
            for (; cursor < end && first < last; cursor++, first++) {
                *first = *cursor;
            }
            return {first, static_cast<errc>(false)};
        }
    }
}
