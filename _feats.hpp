#pragma once

#if !defined(__STD_DECLTYPE_AUTO)
    #if defined(__cpp_decltype_auto)
        #define __STD_DECLTYPE_AUTO decltype(auto)
    #else
        #define __STD_DECLTYPE_AUTO auto
    #endif
#endif

// Handle no constexpr support
#if !defined(__STD_CONSTEXPR)
    // MSVC wisely decided that "constexpr == constexpr const"
    // Thanks, Microsoft
    #if !defined(__cpp_constexpr) || defined(_MSC_VER)
        #define __STD_CONSTEXPR
        #define __STD_CONSTEXPR_CONST constexpr
    #else
        #define __STD_CONSTEXPR constexpr
        #define __STD_CONSTEXPR_CONST constexpr
    #endif
#endif

