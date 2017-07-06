/* math.cpp - (c) James S Renwick 2013 - 2016
   ------------------------------------------
   Version 1.0.1
   
   See comments for logic attributions.
*/
#include "math.hpp"

namespace std
{
	UInt pow2(UInt power) noexcept
	{
		if (power > (sizeof(UInt) * 8 - 1)) {
			return 0;
		}
		else if (power < 8) { return 1 << power; }

		UInt output = 1;
		for (UInt i = 0; i < power; i++) {
			output <<= 1;
		}
		return output;
	}

	UInt32 log2(UInt32 i) noexcept
	{
		if (i == 0) return 0;

		// GCC has a nice built-in function for this
#ifdef _GNUC_
		return 31 - (UInt32)__builtin_clz(i);
#else
		for (UInt32 n = 0x80000000, t = 0; n != 0; n >> 1, t++) {
			if ((i & n) != 0) return t;
		}
		return 0;
#endif
	}

	UInt64 log2(UInt64 i) noexcept
	{
		if (i == 0) return 0;

		// GCC has a nice built-in function for this
#ifdef _GNUC_
		return 63 - (UInt64)__builtin_clzl(i);
#else
		for (UInt64 n = 0x80000000 << 32, t = 0; n != 0; n >> 1, t++) {
			if ((i & n) != 0) return t;
		}
		return 0;
#endif
	}

	float sqrt(float f, UInt16 precision) noexcept
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

	double sqrt(double d, UInt16 precision) noexcept
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

	UInt16 sqrt(UInt16 num) noexcept
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

	UInt32 fastSqrt(UInt32 value) noexcept
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