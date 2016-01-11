/* memory.hpp - (c) James S Renwick 2016
   -------------------------------------
   Version 1.1.0
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

        virtual UInt32 getMinAlloc() const noexcept = 0;
    };


	// This should be provided by the operating system
	// For user mode, consider system calls to the kernel allocator
	class PageAllocator : public Allocator
	{
	public:
		PageAllocator() noexcept;
		~PageAllocator() noexcept;

	public:
		bool free(void* obj) noexcept override;
		void *allocate(UInt size) noexcept override;
		void *reallocate(void* obj, UInt newSize) noexcept override;

		UInt32 getMinAlloc() const noexcept override;

		UInt32 getSmallPageSize() const noexcept;
		UInt32 getLargePageSize() const noexcept;

		bool lockPage(void* address) const noexcept;
		bool unlockPage(void* address) const noexcept;

		void *allocate(UInt size, UInt32 flags);

	public:
		static PageAllocator& getSystemAllocator();
	};

	class HeapAllocatorImpl;

	class HeapAllocator : public Allocator
	{
	public:
		virtual ~HeapAllocator() noexcept { };

	public:
		virtual bool free(void* obj) noexcept override = 0;
		virtual void *allocate(UInt size) noexcept override = 0;
		virtual void *reallocate(void* obj, UInt newSize) noexcept override = 0;
		
		virtual UInt32 getMinAlloc() const noexcept override = 0;
		virtual void *allocate(UInt size, UInt32 alignment) = 0;

	public:
		static HeapAllocator* newHeap();
		static HeapAllocator* allocatorFromHeap(void* heap);
	};

}
