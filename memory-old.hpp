/* memory.hpp - (c) James S Renwick 2013
   -------------------------------------
   Version 1.0.0
*/
#pragma once
#include "std"

namespace std
{
	// Copies the given region of memory from the source to destination. Returns the number of bytes copied.
	size_t copy_memory(const void* source, void* destination, size_t size) noexcept;

	// Efficiently zeroes an area of memory. Returns 0 if successful.
	size_t zero_memory(void* address, size_t size) noexcept;
}
