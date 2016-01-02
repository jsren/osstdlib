/* memory.hpp - (c) James S Renwick 2016
   -------------------------------------
   Version 1.0.2
*/
#pragma once
#include "std"

namespace std
{
    void *heap_allocate(UInt size) noexcept;

    template<class T>
    inline T *heap_allocate() noexcept {
        return reinterpret_cast<T*>(heap_allocate(sizeof(T)));
    }

    template<class T, UInt count>
    inline T *heap_allocate() noexcept {
        return reinterpret_cast<T*>(heap_allocate(sizeof(T) * count));
    }

    void *reallocate_obj(void *source, UInt oldSize, UInt newSize) noexcept;
        
	// Copies the given region of memory from the source to destination. Returns the number of bytes copied.
	UInt copy_memory(const void* source, void* destination, UInt size) noexcept;

	// Efficiently zeroes an area of memory. Returns 0 if successful.
	UInt zero_memory(void* address, UInt size) noexcept;
}
