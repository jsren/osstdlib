/* __config.hpp
 * ----------------------------
 * You may use this file to configure various aspects of the standard library.
 */
#pragma once
#include <cstddef>

namespace std
{
    namespace _config
    {
        /**
         * The number of extra bytes by which to extend the size of std::vector
         * to facilitate small buffer optimisation (SBO).
         * Default: 0
         */
        static constexpr const size_t vector_sbo_extra = 0;
        /**
         * The inverse factor (1/n) for the minimum size with which the capacity
         * of std::vector will increase when reallocation is performed.
         *
         * This should be declared as an unsigned integer if floats are not supported.
         * Default: float 1.5
         */
		static constexpr const float vector_realloc_factor = 1.5;
        /**
         * The number of extra bytes by which to extend the size of std::string
         * to facilitate small string optimisation (SSO).
         * Default: 0
         */
        static constexpr const std::size_t string_sso_extra = 0;
        /**
         * The inverse factor (1/n) for the minimum size with which the capacity
         * of std::string will increase when reallocation is performed.
         *
         * This should be declared as an unsigned integer if floats are not supported.
         * Default: float 1.5
         */
        static constexpr const float string_realloc_factor = 1.5;
    }
}
