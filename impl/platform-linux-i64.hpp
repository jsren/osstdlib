#pragma once
#include <.limits-decl>

namespace __platform
{
    using size_t = decltype(sizeof(int));
    using ssize_t = signed long;
    using ptrdiff_t = signed long long;

    using max_align_t = long double;

    typedef signed char int8_t;
    typedef short       int16_t;
    typedef long        int32_t;
    typedef long long   int64_t;

    typedef unsigned char      uint8_t;
    typedef unsigned short     uint16_t;
    typedef unsigned long      uint32_t;
    typedef unsigned long long uint64_t;

    struct __file_handle
    {
        ssize_t value;
    };

    struct __result_t
    {
        ssize_t rc{};

        operator bool() const noexcept {
            return rc == 0;
        }
    };


    /* -- Limits -- */
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
    constexpr const std::float_denorm_style float_denorm_st = std::denorm_present;
    constexpr const std::float_denorm_style double_denorm_st = std::denorm_present;
    constexpr const std::float_denorm_style ldouble_denorm_st = std::denorm_present;
    constexpr const bool float_has_dnorm_loss = true;
    constexpr const bool double_has_dnorm_loss = true;
    constexpr const bool ldouble_has_dnorm_loss = true;
    constexpr const std::float_round_style float_round_st = std::round_to_nearest;
    constexpr const std::float_round_style double_round_st = std::round_to_nearest;
    constexpr const std::float_round_style ldouble_round_st = std::round_to_nearest;
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
    constexpr const char char_min = static_cast<char>(char_is_signed ? schar_min : 0);
    constexpr const char char_max = static_cast<char>(char_is_signed ? schar_max : uchar_max);
    constexpr const wchar_t wchar_min = 0;
    constexpr const wchar_t wchar_max = static_cast<wchar_t>(-1);
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
    constexpr const float float_infty = 1;
    constexpr const float float_qnan = 1;
    constexpr const float float_snan = 1;
    constexpr const double double_min = -1;
    constexpr const double double_max = 1;
    constexpr const double double_epsilon = 1;
    constexpr const double double_infty = 1;
    constexpr const double double_qnan = 1;
    constexpr const double double_snan = 1;
    constexpr const long double ldouble_min = -1;
    constexpr const long double ldouble_max = 1;
    constexpr const long double ldouble_epsilon = 1;
    constexpr const long double ldouble_infty = 1;
    constexpr const long double ldouble_qnan = 1;
    constexpr const long double ldouble_snan = 1;
}
