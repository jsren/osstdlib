/* std.hpp - (c) James S Renwick 2017
   ----------------------------------
   Version 2.0.5
*/
#pragma once
#include "_feats.hpp"

/*
	This file contains the standard library type definitions.
*/

// Compiler-specific defines
#ifdef __GNUG__
#ifndef GCC
#define GCC
#endif
#endif


#pragma region GCC Specific

// We may need to define size_t to keep GCC happy
#ifdef GCC
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
        inline __STD_CONSTEXPR operator T*() const { return (T*)0; }

		template<typename T, typename Y>
        inline __STD_CONSTEXPR operator T Y::*() const { return 0; }

		void operator&() const = delete;
		inline __STD_CONSTEXPR operator void*() const { return (void*)0; }

		inline __STD_CONSTEXPR bool operator==(const nullptr_t&) const { return true; }
		inline __STD_CONSTEXPR bool operator!=(const nullptr_t&) const { return false; }
	};
}

#endif

#if !defined(nullptr)
#define nullptr (std::nullptr_t())
#endif

#pragma endregion

#pragma region Integer Type Definitions

typedef signed char int8_t;
typedef short       int16_t;
typedef long        int32_t;
typedef long long   int64_t;

typedef unsigned char      uint8_t;
typedef unsigned short     uint16_t;
typedef unsigned long      uint32_t;
typedef unsigned long long uint64_t;

#pragma endregion
