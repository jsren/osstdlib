/* math.hpp - (c) 2016 James S Renwick
   -----------------------------------
   Authors: James S Renwick
*/
#pragma once
#include "_std.hpp"
#include "_feats.hpp"
#include "type_traits.hpp"

namespace std
{
	template<typename T> struct max_value { };
	template<> struct max_value<uint8_t>  { static constexpr const auto value = static_cast<uint8_t>(-1); };
	template<> struct max_value<uint16_t> { static constexpr const auto value = static_cast<uint16_t>(-1); };
	template<> struct max_value<uint32_t> { static constexpr const auto value = static_cast<uint32_t>(-1); };
	template<> struct max_value<uint64_t> { static constexpr const auto value = static_cast<uint64_t>(-1); };

	template<> struct max_value<int8_t>  { static constexpr const int8_t value = static_cast<int8_t>(-1) >> 1; };
	template<> struct max_value<int16_t> { static constexpr const int16_t value = static_cast<int16_t>(-1) >> 1; };
	template<> struct max_value<int32_t> { static constexpr const int32_t value = static_cast<int32_t>(-1) >> 1; };
	template<> struct max_value<int64_t> { static constexpr const int64_t value = static_cast<int64_t>(-1) >> 1; };


	template<typename T> struct min_value { };
	template<> struct min_value<uint8_t>  { static constexpr const uint8_t  value = 0; };
	template<> struct min_value<uint16_t> { static constexpr const uint16_t value = 0; };
	template<> struct min_value<uint32_t> { static constexpr const uint32_t value = 0; };
	template<> struct min_value<uint64_t> { static constexpr const uint64_t value = 0; };

	template<> struct min_value<int8_t> { static constexpr const int8_t value = ~max_value<int8_t>::value; };
	template<> struct min_value<int16_t> { static constexpr const int16_t value = ~max_value<int16_t>::value; };
	template<> struct min_value<int32_t> { static constexpr const int32_t value = ~max_value<int32_t>::value; };
	template<> struct min_value<int64_t> { static constexpr const int64_t value = ~max_value<int64_t>::value; };

#if defined(__cpp_variable_templates)
    template<typename T> constexpr T max_value_v = max_value<T>::value;
    template<typename T> constexpr T min_value_v = min_value<T>::value;
#endif

	/*
		Raises the integer 2 to the given power.
		Returns 0 if the power is too great.

		power - The power to which to raise the integer 2.
	*/
	uint32_t pow2(uint32_t power) noexcept;

	/*
		Returns the log to the base 2 of a given integer.
		Returns 0 given 0.

		i - The integer for which to find the log.
	*/
	uint32_t log2(uint32_t i) noexcept;


	/* Returns the smallest of the two given integers. */
	template<typename T1, typename T2>
	constexpr inline decltype(auto) min(T1&& i1, T2&& i2) _noexc_copy_from(i1 > i2) {
		return ((i1 > i2) ? std::forward<T2>(i2) : std::forward<T1>(i1));
	}

	/* Returns the largest of the two given integers. */
	template<typename T1, typename T2>
	constexpr inline auto max(T1&& i1, T2&& i2) _noexc_copy_from(i1 > i2) {
		return ((i1 > i2) ? std::forward<T1>(i1) : std::forward<T2>(i2));
	}

	/* 
		Returns the sum of the two given unsigned integers, 
		or the maximum value if the result overflows.
	*/
	template<typename T1, typename T2, 
		class=std::enable_if<std::is_unsigned<T1>, std::is_unsigned<T2>>>
	constexpr inline uint32_t nooverflow_add(T1&& i1, T2&& i2) 
        _noexc_copy_from((i1 + i2) < max(i1, i2))
	{
		auto sum = i1 + i2;
		return (sum < max(i1, i2)) ? std::max_value<T1>::value : sum;
	}
	
	/* 
		Approximates the floating-point square root of a number with the given precision. 
		The precision value indicates the number of times the approximation will be honed.
	
		f         - The number (> 0) for which to approximate the sqare root.
		precision - The iterations to perform.
	*/
	float sqrt(float f, uint16_t precision) noexcept;
	/* 
	Approximates the floating-point square root of a number.
	
	f - The number for which to approximate the sqare root.
	*/
	inline float sqrt(float f) noexcept {
		return sqrt(f, 4);
	}

	
	/* 
		Approximates the double precision floating-point square root of a number with the given precision. 
		The precision value indicates the number of times the approximation will be honed.
	
		d         - The number (> 0) for which to approximate the sqare root.
		precision - The iterations to perform.
	*/
	double sqrt(double d, uint16_t precision) noexcept;
	/* 
		Approximates the floating-point square root of a number.
	
		d - The number for which to approximate the sqare root.
	*/
	inline double sqrt(double d) noexcept {
		return sqrt(d, 5);
	}

	/* 
	Finds the integer square root of a number.
	*/
	uint16_t sqrt(uint16_t num) noexcept;

	/*
	Approximates the integer square root of a number.
	*/
	uint32_t fastSqrt(uint32_t value) noexcept;

}
