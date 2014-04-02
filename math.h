/* math.h - (c) James S Renwick 2013
   ---------------------------------
   Version 1.0.2
*/
#pragma once
#include "std.h"
#include "string.h"

namespace std
{
	/*
	Raises the integer 2 to the given power.
	Returns 0 if the power is too great.

	power - The power to which to raise the integer 2.
	*/
	UInt pow2(UInt power)
	{
		if (power > (sizeof(UInt) * 8 - 1)) return 0;
		else if (power < 8) return 1 << power;

		UInt output = 1;
		for (UInt i = 0; i < power; i++) {
			output <<= 1;
		}
		return output;
	}

	/* Returns the smallest of the two given integers. */
	inline UInt min(UInt i1, UInt i2) {
		return (i1 > i2) ? i2 : i1;
	}

	/* Returns the largest of the two given integers. */
	inline UInt max(UInt i1, UInt i2) {
		return (i1 > i2) ? i1 : i2;
	}

	
	/* 
	Approximates the floating-point square root of a number with the given precision. 
	The precision value indicates the number of times the approximation will be honed.
	
	f         - The number (> 0) for which to approximate the sqare root.
	precision - The iterations to perform.
	*/
	float sqrt(float f, UInt16 precision)
	{
		if (f < 0)  return -1;
		if (f == 0) return 0;

		// Pick a starting value, from which we will find the difference
		float x;

		if      (f < 1)    x = 0.5f;
		else if (f < 10)   x = 3.0f;
		else if (f < 100)  x = 7.0f;
		else if (f < 1000) x = 22.0f;
		else               x = 50.0f;

		// Now iteratively improve our estimate
		for (UInt16 i = 0; i < precision; i++)
		{
			x += (f - (x * x)) / (2 * x);
		}
		return x;
	}
	/* 
	Approximates the floating-point square root of a number.
	
	f - The number for which to approximate the sqare root.
	*/
	float sqrt(float f) {
		return sqrt(f, 4);
	}

	
	/* 
	Approximates the double precision floating-point square root of a number with the given precision. 
	The precision value indicates the number of times the approximation will be honed.
	
	d         - The number (> 0) for which to approximate the sqare root.
	precision - The iterations to perform.
	*/
	double sqrt(double d, UInt16 precision)
	{
		if (d < 0)  return -1;
		if (d == 0) return 0;

		// Pick a starting value, from which we will find the difference
		double x;

		if      (d < 1)      x = 0.5;
		else if (d < 10)     x = 3.0;
		else if (d < 100)    x = 7.0;
		else if (d < 1000)   x = 22.0;
		else if (d < 10000)  x = 70.7;
		else if (d < 100000) x = 223.6;
		else                 x = 1000;

		// Now iteratively improve our estimate
		for (UInt16 i = 0; i < precision; i++)
		{
			x += (d - (x * x)) / (2 * x);
		}
		return x;
	}
	/* 
	Approximates the floating-point square root of a number.
	
	d - The number for which to approximate the sqare root.
	*/
	double sqrt(double d){
		return sqrt(d, 5);
	}

	/* 
	Finds the integer square root of a number.

	(taken from http://en.wikipedia.org/wiki/Methods_of_computing_square_roots)
	*/
	UInt16 sqrt(UInt16 num)
	{
		UInt16 output = 0;
		UInt16 bit = 1 << 14; // The second-to-top bit is set: 1L<<30 for long
 
		// "bit" starts at the highest power of four <= the argument.
		while (bit > num) bit >>= 2;
 
		while (bit != 0) 
		{
			if (num >= output + bit) 
			{
				num -= output + bit;
				output = (output >> 1) + bit;
			}
			else output >>= 1;

			bit >>= 2;
		}
		return output;
	}

	/*
	Approximates the integer square root of a number.

	(taken from http://ccpssolutions.com/nogdusforums/index.php?topic=696.0)
	*/
	UInt32 fastSqrt(UInt32 value)
	{
		UInt32 result = 0;
		for (unsigned int i = 128; i != 0; i >>= 1)
		{
			result += i;
			if (result * result > value) result -= i;
		}
		return result;
	}

}
