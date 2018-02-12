#include <system_error>

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
        to_chars_result _to_chars(unsigned long long value, bool negative,
            unsigned base, char* first, char* last);
    }

    inline to_chars_result to_chars(char* first, char* last, long long value, int base = 10)
    {
        if (value < 0) {
            return __detail::_to_chars(static_cast<unsigned long long>(-value),
                true, static_cast<unsigned>(base), first, last);
        }
        else return __detail::_to_chars(static_cast<unsigned long long>(value),
            false, static_cast<unsigned>(base), first, last);
    }

    inline to_chars_result to_chars(char* first, char* last, unsigned long long value, int base = 10)
    {
        return __detail::_to_chars(static_cast<unsigned long long>(value),
            false, static_cast<unsigned>(base), first, last);
    }

    inline to_chars_result to_chars(char* first, char* last, char value, int base = 10) {
        static_assert(sizeof(char) != sizeof(long long), "Unsupported on this platform.");
        return to_chars(first, last, static_cast<long long>(value), base);
    }
    inline to_chars_result to_chars(char* first, char* last, signed char value, int base = 10) {
        return to_chars(first, last, static_cast<long long>(value), base);
    }
    inline to_chars_result to_chars(char* first, char* last, unsigned char value, int base = 10) {
        return to_chars(first, last, static_cast<unsigned long long>(value), base);
    }
    inline to_chars_result to_chars(char* first, char* last, unsigned int value, int base = 10) {
        return to_chars(first, last, static_cast<unsigned long long>(value), base);
    }
    inline to_chars_result to_chars(char* first, char* last, signed int value, int base = 10) {
        return to_chars(first, last, static_cast<long long>(value), base);
    }
    inline to_chars_result to_chars(char* first, char* last, unsigned long value, int base = 10) {
        return to_chars(first, last, static_cast<unsigned long long>(value), base);
    }
    inline to_chars_result to_chars(char* first, char* last, signed long value, int base = 10) {
        return to_chars(first, last, static_cast<long long>(value), base);
    }
}
