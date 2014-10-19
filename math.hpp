/* math.hpp - (c) James S Renwick 2013
   -----------------------------------
   Version 1.0.4
*/
#pragma once
#include "std"

namespace std
{
	/*
	Raises the integer 2 to the given power.
	Returns 0 if the power is too great.

	power - The power to which to raise the integer 2.
	*/
	UInt pow2(UInt power) noexcept;

	/*
	Returns the log to the base 2 of a given integer.
	Returns 0 given 0.

	i - The integer for which to find the log.
	*/
	UInt32 log2(UInt32 i) noexcept;

	/* Returns the smallest of the two given integers. */
	inline UInt min(UInt i1, UInt i2) noexcept{
		return (i1 > i2) ? i2 : i1;
	}

	/* Returns the largest of the two given integers. */
	inline UInt max(UInt i1, UInt i2) noexcept{
		return (i1 > i2) ? i1 : i2;
	}

	
	/* 
	Approximates the floating-point square root of a number with the given precision. 
	The precision value indicates the number of times the approximation will be honed.
	
	f         - The number (> 0) for which to approximate the sqare root.
	precision - The iterations to perform.
	*/
	float sqrt(float f, UInt16 precision) noexcept;
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
	double sqrt(double d, UInt16 precision) noexcept;
	/* 
	Approximates the floating-point square root of a number.
	
	d - The number for which to approximate the sqare root.
	*/
	inline double sqrt(double d) noexcept {
		return sqrt(d, 5);
	}

	/* 
	Finds the integer square root of a number.

	(taken from http://en.wikipedia.org/wiki/Methods_of_computing_square_roots)
	*/
	UInt16 sqrt(UInt16 num) noexcept;

	/*
	Approximates the integer square root of a number.

	(taken from http://ccpssolutions.com/nogdusforums/index.php?topic=696.0)
	*/
	UInt32 fastSqrt(UInt32 value) noexcept;

}
