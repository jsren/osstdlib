#include "memory.hpp"
#include "async.hpp"

#define HEAP_MAGIC 0xA110CA7E

namespace std
{
	struct HeapRecord
	{
		UInt16 referenceCount;
		UInt32 size;
		void*  address;
		bool   active;
	};

	struct HeapHeader
	{
		Int32  magic;
		Int32  lockVariable;
		void*  data;
		UInt64 size;

		struct FreeEntry
		{
			UInt32      size;
			HeapRecord* record;
			FreeEntry*  next;
		} *freeBins[128];

		HeapRecord records[];
	};

	class HeapAllocatorImpl : public HeapAllocator
	{
	private:
		Lock* lock;
		bool canFree;
		HeapHeader* header;

	public:
		HeapAllocatorImpl() noexcept;
		HeapAllocatorImpl(HeapHeader* heap) noexcept;
		~HeapAllocatorImpl();

	public:
		bool free(void* obj) noexcept override;
		void* allocate(UInt size) noexcept override;
		void* reallocate(void* obj, UInt newSize) noexcept override;

		UInt32 getMinAlloc() const noexcept override;
		void* allocate(UInt size, UInt32 alignment);
	};


	HeapAllocatorImpl::HeapAllocatorImpl() noexcept
	{
		auto pageAlloc = PageAllocator::getSystemAllocator();
		this->header   = static_cast<HeapHeader*>(
			pageAlloc.allocate(pageAlloc.getSmallPageSize()));

		this->header->magic = HEAP_MAGIC;
		this->header->lockVariable = 0;
		this->header->data = pageAlloc.allocate(this->header->size);
		this->header->size = pageAlloc.getSmallPageSize();

		this->lock = new SpinLock(&this->header->lockVariable);
	}

	HeapAllocatorImpl::HeapAllocatorImpl(HeapHeader* heap)
	{
		this->header = heap;
		if (this->header->magic == HEAP_MAGIC)
		{
			this->lock = new SpinLock(&this->header->lockVariable);
		}
		else this->header = nullptr;
	}

	UInt32 HeapAllocatorImpl::getMinAlloc() const noexcept
	{
		return 16;
	}

	void* HeapAllocatorImpl::allocate(UInt size) noexcept
	{
		
	}
}
