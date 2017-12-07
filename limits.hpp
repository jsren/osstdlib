#include <type_traits>

namespace std
{
    enum float_round_style
    {
        round_indeterminate       = -1,
        round_toward_zero         = 0,
        round_to_nearest          = 1,
        round_toward_infinity     = 2,
        round_toward_neg_infinity = 3
    };

    enum float_denorm_style
    {
        denorm_indeterminate = -1,
        denorm_absent        = 0,
        denorm_present       = 1
    };

    namespace __detail
    {
        template<typename T,
            bool _specialized = false,
            bool _signed = false,
            bool _integer = false,
            bool _exact = false,
            bool _infinity = false,
            bool _quiet_NaN = false,
            bool _signaling_NaN = false,
            float_denorm_style _denorm = denorm_absent,
            bool _denorm_loss = false,
            float_round_style _round_style = round_toward_zero,
            bool _iec559 = false,
            bool _bounded = false,
            bool _modulo = false,
            int _digits = 0,
            int _digits10 = 0,
            int _max_digits10 = 0,
            int _radix = 0,
            int _min_exponent = 0,
            int _min_exponent10 = 0,
            int _max_exponent = 0,
            int _max_exponent10 = 0,
            bool _traps = false,
            bool _tinyness_before = false>
        struct numeric_limits_base
        {
            static constexpr const auto is_specialized = _specialized;
            static constexpr const auto is_signed = _signed;
            static constexpr const auto is_integer = _integer;
            static constexpr const auto is_exact = _exact;
            static constexpr const auto has_infinity = _infinity;
            static constexpr const auto has_quiet_NaN = _quiet_NaN;
            static constexpr const auto has_signaling_NaN = _signaling_NaN;
            static constexpr const auto has_denorm = _denorm;
            static constexpr const auto has_denorm_loss = _denorm_loss;
            static constexpr const auto round_style = _round_style;
            static constexpr const auto is_iec559 = _iec559;
            static constexpr const auto is_bounded = _bounded;
            static constexpr const auto digits = _digits;
            static constexpr const auto digits10 = _digits10;
            static constexpr const auto max_digits10 = _max_digits10;
            static constexpr const auto radix = _radix;
            static constexpr const auto min_exponent = _min_exponent;
            static constexpr const auto min_exponent10 = _min_exponent10;
            static constexpr const auto max_exponent = _max_exponent;
            static constexpr const auto max_exponent10 = _max_exponent10;
            static constexpr const auto traps = _traps;
            static constexpr const auto tinyness_before = _tinyness_before;
        };

        template<typename T>
        struct numeric_limits : numeric_limits_base<T>
        {
            static constexpr auto min() noexcept { return T{}; }
            static constexpr auto lowest() noexcept { return T{}; }
            static constexpr auto max() noexcept { return T{}; }
            static constexpr auto epsilon() noexcept { return T{}; }
            static constexpr auto round_error() noexcept { return T{}; }
            static constexpr auto infinity() noexcept { return T{}; }
            static constexpr auto quiet_NaN() noexcept { return T{}; }
            static constexpr auto signaling_NaN() noexcept { return T{}; }
        };

        constexpr const unsigned char bitsPerByte = 8;

        constexpr const bool char_is_signed = false;
        constexpr const bool wchar_is_signed = false;
        constexpr const bool float_has_infty = true;
        constexpr const bool double_has_infty = true;
        constexpr const bool ldouble_has_infty = true;
        constexpr const bool float_has_qnan = true;
        constexpr const bool double_has_qnan = true;
        constexpr const bool ldouble_has_qnan = true;
        constexpr const bool float_has_snan = true;
        constexpr const bool double_has_snan = true;
        constexpr const bool ldouble_has_snan = true;
        constexpr const float_denorm_style float_denorm_st = denorm_present;
        constexpr const float_denorm_style double_denorm_st = denorm_present;
        constexpr const float_denorm_style ldouble_denorm_st = denorm_present;
        constexpr const bool float_has_dnorm_loss = true;
        constexpr const bool double_has_dnorm_loss = true;
        constexpr const bool ldouble_has_dnorm_loss = true;
        constexpr const float_round_style float_round_st = round_to_nearest;
        constexpr const float_round_style double_round_st = round_to_nearest;
        constexpr const float_round_style ldouble_round_st = round_to_nearest;
        constexpr const bool float_iec559 = true;
        constexpr const bool double_iec559 = true;
        constexpr const bool ldouble_iec559 = true;

        constexpr const bool char_is_modulo = false;
        constexpr const bool schar_is_modulo = false;
        constexpr const bool wchar_is_modulo = false;
        constexpr const bool char16_is_modulo = false;
        constexpr const bool char32_is_modulo = false;
        constexpr const bool short_is_modulo = false;
        constexpr const bool int_is_modulo = false;
        constexpr const bool long_is_modulo = false;
        constexpr const bool llong_is_modulo = false;

        constexpr const int float_mantissa_digits = 0;
        constexpr const int double_mantissa_digits = 0;
        constexpr const int ldouble_mantissa_digits = 0;
        constexpr const int float_b10_digits = 6;
        constexpr const int double_b10_digits = 15;
        constexpr const int ldouble_b10_digits = 18;
        constexpr const int float_max_b10_digits = 9;
        constexpr const int double_max_b10_digits = 17;
        constexpr const int ldouble_max_b10_digits = 19;
        constexpr const int float_radix = 2;
        constexpr const int double_radix = 2;
        constexpr const int ldouble_radix = 2;
        constexpr const int float_min_exp = 0;
        constexpr const int double_min_exp = 0;
        constexpr const int ldouble_min_exp = 0;
        constexpr const int float_min_b10_exp = 0;
        constexpr const int double_min_b10_exp = 0;
        constexpr const int ldouble_min_b10_exp = 0;
        constexpr const int float_max_exp = 0;
        constexpr const int double_max_exp = 0;
        constexpr const int ldouble_max_exp = 0;
        constexpr const int float_max_b10_exp = 0;
        constexpr const int double_max_b10_exp = 0;
        constexpr const int ldouble_max_b10_exp = 0;

        constexpr const int char_padding = char_is_signed ? 1 : 0;
        constexpr const int schar_padding = 0;
        constexpr const int uchar_padding = 0;
        constexpr const int wchar_padding = wchar_is_signed ? 1 : 0;
        constexpr const int char16_padding = 0;
        constexpr const int char32_padding = 0;
        constexpr const int short_padding = 1;
        constexpr const int ushort_padding = 0;
        constexpr const int int_padding = 1;
        constexpr const int uint_padding = 0;
        constexpr const int long_padding = 1;
        constexpr const int ulong_padding = 0;
        constexpr const int llong_padding = 1;
        constexpr const int ullong_padding = 0;

        constexpr const int char_traps = true;
        constexpr const int schar_traps = true;
        constexpr const int uchar_traps = true;
        constexpr const int wchar_traps = true;
        constexpr const int char16_traps = true;
        constexpr const int char32_traps = true;
        constexpr const int short_traps = true;
        constexpr const int ushort_traps = true;
        constexpr const int int_traps = true;
        constexpr const int uint_traps = true;
        constexpr const int long_traps = true;
        constexpr const int ulong_traps = true;
        constexpr const int llong_traps = true;
        constexpr const int ullong_traps = true;
        constexpr const int float_traps = false;
        constexpr const int double_traps = false;
        constexpr const int ldouble_traps = false;

        constexpr const int float_tinyness_before = false;
        constexpr const int double_tinyness_before = false;
        constexpr const int ldouble_tinyness_before = false;

        constexpr const signed char schar_min = -128;
        constexpr const signed char schar_max = 127;
        constexpr const unsigned char uchar_max = 255;
        constexpr const char char_min = char_is_signed ? schar_min : 0;
        constexpr const char char_max = char_is_signed ? schar_max : uchar_max;
        constexpr const wchar_t wchar_min = 0;
        constexpr const wchar_t wchar_max = static_cast<wchar_t>(-1);
        static_assert(!wchar_is_signed,"");
        constexpr const char16_t char16_max = 0xFFFF;
        constexpr const char32_t char32_max = 0xFFFFFFFF;
        constexpr const short short_min = static_cast<short>(0x8000);
        constexpr const short short_max = static_cast<short>(0x7FFF);
        constexpr const unsigned short ushort_max = 0xFFFF;
        constexpr const int int_min = static_cast<short>(0x80000000);
        constexpr const int int_max = static_cast<short>(0x7FFFFFFF);
        constexpr const unsigned int uint_max = 0xFFFFFFFF;
        constexpr const long long_min = static_cast<short>(0x8000000000000000);
        constexpr const long long_max = static_cast<short>(0x7FFFFFFFFFFFFFFF);
        constexpr const unsigned long ulong_max = 0xFFFFFFFFFFFFFFFF;
        constexpr const long long llong_min = static_cast<short>(0x8000000000000000);
        constexpr const long long llong_max = static_cast<short>(0x7FFFFFFFFFFFFFFF);
        constexpr const unsigned long long ullong_max = 0xFFFFFFFFFFFFFFFF;

        constexpr const float float_min = -1;
        constexpr const float float_max = 1;
        constexpr const float float_epsilon = 1;
        constexpr const double double_min = -1;
        constexpr const double double_max = 1;
        constexpr const double double_epsilon = 1;
        constexpr const long double ldouble_min = -1;
        constexpr const long double ldouble_max = 1;
        constexpr const long double ldouble_epsilon = 1;


        static constexpr const int char_digits = bitsPerByte*sizeof(char) - char_padding;
        static constexpr const int schar_digits = bitsPerByte*sizeof(signed char) - schar_padding;
        static constexpr const int uchar_digits = bitsPerByte*sizeof(unsigned char) - uchar_padding;
        static constexpr const int wchar_digits = bitsPerByte*sizeof(wchar_t) - wchar_padding;
        static constexpr const int char16_digits = bitsPerByte*sizeof(char16_t) - char16_padding;
        static constexpr const int char32_digits = bitsPerByte*sizeof(char32_t) - char32_padding;
        static constexpr const int short_digits = bitsPerByte*sizeof(short) - short_padding;
        static constexpr const int ushort_digits = bitsPerByte*sizeof(unsigned short) - ushort_padding;
        static constexpr const int int_digits = bitsPerByte*sizeof(int) - int_padding;
        static constexpr const int uint_digits = bitsPerByte*sizeof(unsigned int) - uint_padding;
        static constexpr const int long_digits = bitsPerByte*sizeof(long) - long_padding;
        static constexpr const int ulong_digits = bitsPerByte*sizeof(unsigned long) - ulong_padding;
        static constexpr const int llong_digits = bitsPerByte*sizeof(long long) - llong_padding;
        static constexpr const int ullong_digits = bitsPerByte*sizeof(unsigned long long) - ullong_padding;


        static constexpr const double log10_2 = 0.30102999566;

        static constexpr const int char_b10_digits = char_digits * log10_2;
        static constexpr const int schar_b10_digits = schar_digits * log10_2;
        static constexpr const int uchar_b10_digits = uchar_digits * log10_2;
        static constexpr const int wchar_b10_digits = wchar_digits * log10_2;
        static constexpr const int char16_b10_digits = char16_digits * log10_2;
        static constexpr const int char32_b10_digits = char32_digits * log10_2;
        static constexpr const int short_b10_digits = short_digits * log10_2;
        static constexpr const int ushort_b10_digits = ushort_digits * log10_2;
        static constexpr const int int_b10_digits = int_digits * log10_2;
        static constexpr const int uint_b10_digits = uint_digits * log10_2;
        static constexpr const int long_b10_digits = long_digits * log10_2;
        static constexpr const int ulong_b10_digits = ulong_digits * log10_2;
        static constexpr const int llong_b10_digits = llong_digits * log10_2;
        static constexpr const int ullong_b10_digits = ullong_digits * log10_2;


        template<> struct numeric_limits<bool> : numeric_limits_base<bool,
            true, false, true, true, false, false, false, denorm_absent, false,
            round_toward_zero, false, true, false, 1, 0, 0, 2, 0, 0, 0, 0, false, false>
        {
            static constexpr auto min() noexcept { return false; }
            static constexpr auto lowest() noexcept { return false; }
            static constexpr auto max() noexcept { return true; }
            static constexpr auto epsilon() noexcept { return false; }
            static constexpr auto round_error() noexcept { return false; }
            static constexpr auto infinity() noexcept { return false; }
            static constexpr auto quiet_NaN() noexcept { return false; }
            static constexpr auto signaling_NaN() noexcept { return false; }
        };

        template<> struct numeric_limits<char> : numeric_limits_base<char,
            true, char_is_signed, true, true, false, false, false, denorm_absent, false,
            round_toward_zero, false, true, char_is_modulo, char_digits, char_b10_digits,
            0, 2, 0, 0, 0, 0, char_traps, false>
        {
            static constexpr auto min() noexcept { return false; }
            static constexpr auto lowest() noexcept { return false; }
            static constexpr auto max() noexcept { return true; }
            static constexpr auto epsilon() noexcept { return false; }
            static constexpr auto round_error() noexcept { return false; }
            static constexpr auto infinity() noexcept { return false; }
            static constexpr auto quiet_NaN() noexcept { return false; }
            static constexpr auto signaling_NaN() noexcept { return false; }
        };

        template<> struct numeric_limits<signed char> : numeric_limits_base<signed char,
            true, true, true, true, false, false, false, denorm_absent, false,
            round_toward_zero, false, true, schar_is_modulo, schar_digits, schar_b10_digits,
            0, 2, 0, 0, 0, 0, schar_traps, false>
        {
            static constexpr auto min() noexcept { return false; }
            static constexpr auto lowest() noexcept { return false; }
            static constexpr auto max() noexcept { return true; }
            static constexpr auto epsilon() noexcept { return false; }
            static constexpr auto round_error() noexcept { return false; }
            static constexpr auto infinity() noexcept { return false; }
            static constexpr auto quiet_NaN() noexcept { return false; }
            static constexpr auto signaling_NaN() noexcept { return false; }
        };

        template<> struct numeric_limits<unsigned char> : numeric_limits_base<unsigned char,
            true, true, true, true, false, false, false, denorm_absent, false,
            round_toward_zero, false, true, true, uchar_digits, uchar_b10_digits, 0, 2, 0, 0,
            0, 0, uchar_traps, false>
        {
            static constexpr auto min() noexcept { return false; }
            static constexpr auto lowest() noexcept { return false; }
            static constexpr auto max() noexcept { return true; }
            static constexpr auto epsilon() noexcept { return false; }
            static constexpr auto round_error() noexcept { return false; }
            static constexpr auto infinity() noexcept { return false; }
            static constexpr auto quiet_NaN() noexcept { return false; }
            static constexpr auto signaling_NaN() noexcept { return false; }
        };

        template<> struct numeric_limits<wchar_t> : numeric_limits_base<wchar_t,
            true, wchar_is_signed, true, true, false, false, false, denorm_absent, false,
            round_toward_zero, false, true, wchar_is_modulo, wchar_digits, wchar_b10_digits,
            0, 2, 0, 0, 0, 0, wchar_traps, false>
        {
            static constexpr auto min() noexcept { return false; }
            static constexpr auto lowest() noexcept { return false; }
            static constexpr auto max() noexcept { return true; }
            static constexpr auto epsilon() noexcept { return false; }
            static constexpr auto round_error() noexcept { return false; }
            static constexpr auto infinity() noexcept { return false; }
            static constexpr auto quiet_NaN() noexcept { return false; }
            static constexpr auto signaling_NaN() noexcept { return false; }
        };

        template<> struct numeric_limits<char16_t> : numeric_limits_base<char16_t,
            true, false, true, true, false, false, false, denorm_absent, false,
            round_toward_zero, false, true, char16_is_modulo, char16_digits, char16_b10_digits,
            0, 2, 0, 0, 0, 0, char16_traps, false>
        {
            static constexpr auto min() noexcept { return false; }
            static constexpr auto lowest() noexcept { return false; }
            static constexpr auto max() noexcept { return true; }
            static constexpr auto epsilon() noexcept { return false; }
            static constexpr auto round_error() noexcept { return false; }
            static constexpr auto infinity() noexcept { return false; }
            static constexpr auto quiet_NaN() noexcept { return false; }
            static constexpr auto signaling_NaN() noexcept { return false; }
        };

        template<> struct numeric_limits<char32_t> : numeric_limits_base<char32_t,
            true, false, true, true, false, false, false, denorm_absent, false,
            round_toward_zero, false, true, char32_is_modulo, char32_digits, char32_b10_digits,
            0, 2, 0, 0, 0, 0, char32_traps, false>
        {
            static constexpr auto min() noexcept { return false; }
            static constexpr auto lowest() noexcept { return false; }
            static constexpr auto max() noexcept { return true; }
            static constexpr auto epsilon() noexcept { return false; }
            static constexpr auto round_error() noexcept { return false; }
            static constexpr auto infinity() noexcept { return false; }
            static constexpr auto quiet_NaN() noexcept { return false; }
            static constexpr auto signaling_NaN() noexcept { return false; }
        };

        template<> struct numeric_limits<short> : numeric_limits_base<short,
            true, true, true, true, false, false, false, denorm_absent, false,
            round_toward_zero, false, true, short_is_modulo, short_digits, short_b10_digits, 0,
            2, 0, 0, 0, 0, short_traps, false>
        {
            static constexpr auto min() noexcept { return false; }
            static constexpr auto lowest() noexcept { return false; }
            static constexpr auto max() noexcept { return true; }
            static constexpr auto epsilon() noexcept { return false; }
            static constexpr auto round_error() noexcept { return false; }
            static constexpr auto infinity() noexcept { return false; }
            static constexpr auto quiet_NaN() noexcept { return false; }
            static constexpr auto signaling_NaN() noexcept { return false; }
        };

        template<> struct numeric_limits<unsigned short> : numeric_limits_base<unsigned short,
            true, false, true, true, false, false, false, denorm_absent, false,
            round_toward_zero, false, true, true, ushort_digits, ushort_b10_digits, 0, 2, 0, 0,
            0, 0, ushort_traps, false>
        {
            static constexpr auto min() noexcept { return false; }
            static constexpr auto lowest() noexcept { return false; }
            static constexpr auto max() noexcept { return true; }
            static constexpr auto epsilon() noexcept { return false; }
            static constexpr auto round_error() noexcept { return false; }
            static constexpr auto infinity() noexcept { return false; }
            static constexpr auto quiet_NaN() noexcept { return false; }
            static constexpr auto signaling_NaN() noexcept { return false; }
        };

        template<> struct numeric_limits<int> : numeric_limits_base<int,
            true, true, true, true, false, false, false, denorm_absent, false,
            round_toward_zero, false, true, int_is_modulo, int_digits, int_b10_digits, 0, 2, 0,
            0, 0, 0, int_traps, false>
        {
            static constexpr auto min() noexcept { return false; }
            static constexpr auto lowest() noexcept { return false; }
            static constexpr auto max() noexcept { return true; }
            static constexpr auto epsilon() noexcept { return false; }
            static constexpr auto round_error() noexcept { return false; }
            static constexpr auto infinity() noexcept { return false; }
            static constexpr auto quiet_NaN() noexcept { return false; }
            static constexpr auto signaling_NaN() noexcept { return false; }
        };

        template<> struct numeric_limits<unsigned int> : numeric_limits_base<unsigned int,
            true, false, true, true, false, false, false, denorm_absent, false,
            round_toward_zero, false, true, true, uint_digits, uint_b10_digits, 0, 2, 0, 0, 0,
            0, uint_traps, false>
        {
            static constexpr auto min() noexcept { return false; }
            static constexpr auto lowest() noexcept { return false; }
            static constexpr auto max() noexcept { return true; }
            static constexpr auto epsilon() noexcept { return false; }
            static constexpr auto round_error() noexcept { return false; }
            static constexpr auto infinity() noexcept { return false; }
            static constexpr auto quiet_NaN() noexcept { return false; }
            static constexpr auto signaling_NaN() noexcept { return false; }
        };

        template<> struct numeric_limits<long> : numeric_limits_base<long,
            true, true, true, true, false, false, false, denorm_absent, false,
            round_toward_zero, false, true, long_is_modulo, long_digits, long_b10_digits, 0, 2,
            0, 0, 0, 0, long_traps, false>
        {
            static constexpr auto min() noexcept { return false; }
            static constexpr auto lowest() noexcept { return false; }
            static constexpr auto max() noexcept { return true; }
            static constexpr auto epsilon() noexcept { return false; }
            static constexpr auto round_error() noexcept { return false; }
            static constexpr auto infinity() noexcept { return false; }
            static constexpr auto quiet_NaN() noexcept { return false; }
            static constexpr auto signaling_NaN() noexcept { return false; }
        };

        template<> struct numeric_limits<unsigned long> : numeric_limits_base<unsigned long,
            true, false, true, true, false, false, false, denorm_absent, false,
            round_toward_zero, false, true, true, ulong_digits, ullong_b10_digits, 0, 2, 0, 0,
            0, 0, ulong_traps, false>
        {
            static constexpr auto min() noexcept { return false; }
            static constexpr auto lowest() noexcept { return false; }
            static constexpr auto max() noexcept { return true; }
            static constexpr auto epsilon() noexcept { return false; }
            static constexpr auto round_error() noexcept { return false; }
            static constexpr auto infinity() noexcept { return false; }
            static constexpr auto quiet_NaN() noexcept { return false; }
            static constexpr auto signaling_NaN() noexcept { return false; }
        };

        template<> struct numeric_limits<long long> : numeric_limits_base<long long,
            true, true, true, true, false, false, false, denorm_absent, false,
            round_toward_zero, false, true, llong_is_modulo, llong_digits, llong_b10_digits, 0,
            2, 0, 0, 0, 0, llong_traps, false>
        {
            static constexpr auto min() noexcept { return false; }
            static constexpr auto lowest() noexcept { return false; }
            static constexpr auto max() noexcept { return true; }
            static constexpr auto epsilon() noexcept { return false; }
            static constexpr auto round_error() noexcept { return false; }
            static constexpr auto infinity() noexcept { return false; }
            static constexpr auto quiet_NaN() noexcept { return false; }
            static constexpr auto signaling_NaN() noexcept { return false; }
        };

        template<> struct numeric_limits<unsigned long long> : numeric_limits_base<unsigned long long,
            true, false, true, true, false, false, false, denorm_absent, false,
            round_toward_zero, false, true, true, ullong_digits, ullong_b10_digits, 0, 2, 0, 0,
            0, 0, ullong_traps, false>
        {
            static constexpr auto min() noexcept { return false; }
            static constexpr auto lowest() noexcept { return false; }
            static constexpr auto max() noexcept { return true; }
            static constexpr auto epsilon() noexcept { return false; }
            static constexpr auto round_error() noexcept { return false; }
            static constexpr auto infinity() noexcept { return false; }
            static constexpr auto quiet_NaN() noexcept { return false; }
            static constexpr auto signaling_NaN() noexcept { return false; }
        };

        template<> struct numeric_limits<float> : numeric_limits_base<float,
            true, true, false, false, float_has_infty, float_has_qnan, float_has_snan,
            float_denorm_st, float_has_dnorm_loss, float_round_st, float_iec559, true,
            false, float_mantissa_digits, float_b10_digits, float_max_b10_digits, float_radix,
            float_min_exp, float_min_b10_exp, float_max_exp, float_max_b10_exp, float_traps,
            float_tinyness_before>
        {
            static constexpr auto min() noexcept { return false; }
            static constexpr auto lowest() noexcept { return false; }
            static constexpr auto max() noexcept { return true; }
            static constexpr auto epsilon() noexcept { return false; }
            static constexpr auto round_error() noexcept { return false; }
            static constexpr auto infinity() noexcept { return false; }
            static constexpr auto quiet_NaN() noexcept { return false; }
            static constexpr auto signaling_NaN() noexcept { return false; }
        };

        template<> struct numeric_limits<double> : numeric_limits_base<double,
            true, true, false, false, double_has_infty, double_has_qnan,
            double_has_snan, double_denorm_st, double_has_dnorm_loss, double_round_st,
            double_iec559, true, false, double_mantissa_digits, double_b10_digits,
            double_max_b10_digits, double_radix, double_min_exp, double_min_b10_exp,
            double_max_exp, double_max_b10_exp, double_traps, double_tinyness_before>
        {
            static constexpr auto min() noexcept { return false; }
            static constexpr auto lowest() noexcept { return false; }
            static constexpr auto max() noexcept { return true; }
            static constexpr auto epsilon() noexcept { return false; }
            static constexpr auto round_error() noexcept { return false; }
            static constexpr auto infinity() noexcept { return false; }
            static constexpr auto quiet_NaN() noexcept { return false; }
            static constexpr auto signaling_NaN() noexcept { return false; }
        };

        template<> struct numeric_limits<long double> : numeric_limits_base<long double,
            true, true, false, false, ldouble_has_infty, ldouble_has_qnan,
            ldouble_has_snan, ldouble_denorm_st, ldouble_has_dnorm_loss,
            ldouble_round_st, ldouble_iec559, true, false, ldouble_mantissa_digits,
            ldouble_b10_digits, ldouble_max_b10_digits, ldouble_radix, ldouble_min_exp,
            ldouble_min_b10_exp, ldouble_max_exp, ldouble_max_b10_exp, ldouble_traps,
            ldouble_tinyness_before>
        {
            static constexpr auto min() noexcept { return false; }
            static constexpr auto lowest() noexcept { return false; }
            static constexpr auto max() noexcept { return true; }
            static constexpr auto epsilon() noexcept { return false; }
            static constexpr auto round_error() noexcept { return false; }
            static constexpr auto infinity() noexcept { return false; }
            static constexpr auto quiet_NaN() noexcept { return false; }
            static constexpr auto signaling_NaN() noexcept { return false; }
        };

    }

    template<typename T>
    struct numeric_limits : __detail::numeric_limits<remove_cv_t<T>>  { };
}
