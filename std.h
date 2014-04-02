#pragma once

/*
	This file contains the standard library type definitions.
*/

#define null 0
#define ptrnull (void*)-1

typedef unsigned char byte;
typedef signed   char sbyte;

typedef long      Int32;
typedef short     Int16;
typedef long long Int64;

typedef unsigned long      UInt32;
typedef unsigned short     UInt16;
typedef unsigned long long UInt64;

namespace std
{
	typedef Int64  Long;
	typedef Int16  Short;
	typedef UInt64 ULong;
	typedef UInt16 UShort;

#if BITS64
	typedef Int64  Int;
	typedef UInt64 UInt;
#else
	typedef Int32  Int;
	typedef UInt32 UInt;
#endif

	/* The maximum UInt value. */
	const UInt UIntMax = (UInt)-1;

	typedef byte  Byte;
	typedef sbyte SByte;

	// Gets the null pointer for the specified pointer type.
	#define nullptr(T) (T)(void*)-1

	// Converts the given address into a pointer-to-T.
	#define raw_ref(T,address) (T*)(void*)address
	// Converts the data at the given address into a value of type T.
	#define raw_deref(T,address) *(raw_ref(T,address))

	// Efficiently zeroes an area of memory. Returns 0 if successful.
	UInt zero(void* address, UInt size)
	{
		byte* bptr  = (byte*)address;

		// If very small, zero byte-by-byte
		if (size < (sizeof(UInt) << 4))
		{
			for (UInt i = 0; i < size; i++)	{
				bptr[i] = 0;
			}
			return 0; // Finished
		}

		// ======================================================
		
		// (Q/D)word-align address
		UInt iaddr = (UInt) address;
		UInt trim = sizeof(UInt) - (iaddr % sizeof(UInt));
		size -= trim;

		// Get the number of integers to zero
		UInt  intCount = size / sizeof(UInt);
		UInt* iptr     = (UInt*)(bptr + trim);

		// Zero int-wise
		for (UInt i = 0; i < intCount; i++)
		{
			iptr[i] = 0; 
		}
		size -= intCount * sizeof(UInt);

		// Zero leading bits
		for (UInt i = 0; i < trim; i++)	{
			bptr[i] = 0;
		}

		// Get trailing pointer
		bptr = bptr + trim + intCount * sizeof(UInt);

		// Zero trailing bits
		for (UInt i = 0; i < size; i++)	{
			bptr[i] = 0;
		}

		return size; // Return bits not zeroed (should be 0!)
	}
}
