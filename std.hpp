/* std.hpp - (c) James S Renwick 2013 - 2016
   -----------------------------------------
   Version 2.2.0
*/
#pragma once

/*
	This file contains the standard library type definitions.
*/

// Compiler-specific defines

#ifdef _MSC_VER
#define __weak __declspec(selectany)
#endif

#pragma region GCC Specific

// We may need to define size_t to keep GCC happy
#ifdef __GNUG__
typedef long unsigned int size_t;

/* NULLPTR DEFINITION */
namespace std
{
	class nullptr_t
	{
	private:
		unsigned : sizeof(void*);

	public:
		template<typename T>
		operator T*() const { return (T*)0; }

		template<typename T, typename Y>
		operator T Y::*() const { return 0; }

		void operator&() const = delete;
		operator void*() const { return (void*)0; }

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
	constexpr Int  IntMax = (UInt)-1 >> 1;
	/* The maximum UInt value. */
    constexpr UInt UIntMax = (UInt)-1;

	/* The maximum Int32 value. */
    constexpr Int32 Int32Max = (UInt32)-1 >> 1;
	/* The maximum UInt32 value. */
    constexpr UInt32 UInt32Max = (UInt32)-1;

	/* The maximum Short value. */
    constexpr Short  ShortMax = (UShort)-1 >> 1;
	/* The maximum UShort value. */
    constexpr UShort UShortMax = (UShort)-1;

	/* The maximum Long value. */
    constexpr Long  LongtMax = (ULong)-1 >> 1;
	/* The maximum ULong value. */
    constexpr ULong ULongMax = (ULong)-1;

    template<class T>
    inline constexpr T min(T v1, T v2) noexcept {
        return v1 <= v2 ? v1 : v2;
    }
    template<class T>
    inline constexpr T max(T v1, T v2) noexcept {
        return v1 >= v2 ? v1 : v2;
    }
}

#pragma endregion
