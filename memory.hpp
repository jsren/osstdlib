/* memory.hpp - (c) James S Renwick 2016
   -------------------------------------
   Version 1.0.2
*/
#pragma once
#include "std"

namespace std
{
    void *heap_allocate(UInt size) noexcept;

    void *heap_resize(void *source, UInt oldSize, UInt newSize) noexcept;
    
	// Copies the given region of memory from the source to destination. Returns the number of bytes copied.
	UInt copy_memory(const void* source, void* destination, UInt size) noexcept;

	// Efficiently zeroes an area of memory. Returns 0 if successful.
	UInt zero_memory(void* address, UInt size) noexcept;


    class Allocator
    {
    public:
        virtual ~Allocator() { }

    public:
        virtual bool free(void* obj) noexcept = 0;
        virtual void* allocate(UInt size) noexcept = 0;
        virtual void* reallocate(void* obj, UInt newSize) noexcept = 0;

        virtual UInt getFreeSpace() noexcept = 0;
        virtual byte getGranularity() noexcept = 0;
    };

}
