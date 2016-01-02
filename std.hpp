/* std.hpp - (c) James S Renwick 2013 
   ----------------------------------
   Version 2.0.3
*/
#pragma once

/*
	This file contains the standard library type definitions.
*/

// Compiler-specific defines
#ifdef __GNUG__
#ifndef GCC
#define GCC
#endif
#endif

#define null 0

#ifdef VS
/* Guarantees that the function will never throw an exception. */
#define noexcept throw()
#define __weak __declspec(selectany)
#endif

#pragma region GCC Specific

// We may need to define size_t to keep GCC happy
#ifdef GCC
typedef long unsigned int size_t;
#endif

/* NULLPTR DEFINITION */
#ifdef GCC
namespace std
{
	class nullptr_t
	{
	private:
		unsigned : sizeof(void*);

	public:
		template<typename T>
		operator T*() const { return (T*)-1; }

		template<typename T, typename Y>
		operator T Y::*() const { return -1; }

		void operator&() const = delete;
		operator void*() const { return (void*)-1; }

		inline bool operator==(const nullptr_t&) const { return true; }
		inline bool operator!=(const nullptr_t&) const { return false; }
	};
}

#define nullptr (std::nullptr_t())
#endif

#pragma endregion

#pragma region Integer Type Definitions

typedef unsigned char byte;
typedef signed   char sbyte;

typedef long      Int32;
typedef short     Int16;
typedef long long Int64;

typedef unsigned long      UInt32;
typedef unsigned short     UInt16;
typedef unsigned long long UInt64;


#if BITS64
	typedef Int64  Int;
	typedef UInt64 UInt;
#else
	typedef Int32  Int;
	typedef UInt32 UInt;
#endif

// Define named integer types, à la .NET
namespace std
{
	typedef byte  Byte;
	typedef sbyte SByte;

	typedef Int64  Long;
	typedef Int16  Short;
	typedef UInt64 ULong;
	typedef UInt16 UShort;

	/* The maximum Int value. */
	const Int  IntMax = -1 >> 1;
	/* The maximum UInt value. */
	const UInt UIntMax = (UInt)-1;

	/* The maximum Int32 value. */
	const Int32 Int32Max = -1 >> 1;
	/* The maximum UInt32 value. */
	const UInt32 UInt32Max = (UInt32)-1;

	/* The maximum Short value. */
	const Short  ShortMax = -1 >> 1;
	/* The maximum UShort value. */
	const UShort UShortMax = (UShort)-1;

	/* The maximum Long value. */
	const Long  LongtMax = -1 >> 1;
	/* The maximum ULong value. */
	const ULong ULongMax = (ULong)-1;
}

#pragma endregion