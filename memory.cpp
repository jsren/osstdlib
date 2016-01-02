#include "memory.hpp"

namespace std
{
#ifdef DEFAULT_ALLOC
    void *heap_allocate(UInt size) noexcept 
    {
        return new byte[size];
    }
#endif

#ifdef DEFAULT_REALLOC
    void *reallocate_obj(void *source, UInt oldSize, UInt newSize) noexcept
    {
        if (newSize == oldSize) return source;
        byte *out = new byte[newSize];

        copy_memory(source, out, newSize > oldSize ? newSize : oldSize);
        delete source;

        return out;
    }
#endif

	UInt copy_memory(const void* source, void* destination, UInt size) noexcept
	{
		const byte* bp_src  = static_cast<const byte*>(source);
		      byte* bp_dest = static_cast<byte*>(destination);

		if (bp_dest > bp_src && bp_dest < bp_src + size) { return 0; }
		UInt diff = bp_dest - bp_src;

		// If very small or very misaligned, zero byte-by-byte
		if (size > (sizeof(UInt) << 5) || diff % 2 != 0)
		{
			for (UInt i = 0; i < size; i++)	{
				bp_dest[i] = bp_src[i];
			}
			return size; // Finished
		}
		else
		{
			UInt i_src = reinterpret_cast<UInt>(source);
			UInt i_dst = reinterpret_cast<UInt>(destination);

			// Get best alignment
			UInt alignment = 2;
			if (diff % 4 == 0)
			{
#if BITS64
				if (diff % 8 == 0) {
					alignment = 8;
				}
				else 
#endif
				alignment = 4;
			}
			

			// (Q/D)word-align addresses
			UInt correction = i_src % alignment;
			if (correction != 0)
			{
				correction = alignment - correction;

				i_src += correction;
				i_dst += correction;
			}

			UInt copied = 0;

			// Copy leading bytes
			for (UInt i = 0; i < correction; i++) {
				bp_dest[i] = bp_src[i];
			}
			copied += correction;

			// Performed aligned copy
			if (alignment == 2)
			{
				UInt iterations = (size - correction) / 2;

				for (UInt i = 0; i < iterations; i++) {
					reinterpret_cast<UInt16*>(i_dst)[i] = reinterpret_cast<UInt16*>(i_src)[i];
				}
				copied += iterations * 2;
			}
			else if (alignment == 4)
			{
				UInt iterations = (size - correction) / 4;

				for (UInt i = 0; i < iterations; i++) {
					reinterpret_cast<UInt32*>(i_dst)[i] = reinterpret_cast<UInt32*>(i_src)[i];
				}
				copied += iterations * 4;
			}
			else
			{
				UInt iterations = (size - correction) / 8;

				for (UInt i = 0; i < iterations; i++) {
					reinterpret_cast<UInt64*>(i_dst)[i] = reinterpret_cast<UInt64*>(i_src)[i];
				}
				copied += iterations * 8;
			}

			// Get trailing pointer
			bp_src  += copied;
			bp_dest += copied;

			// Copy trailing bytes
			for (UInt i = 0; i < size - copied; i++) {
				bp_dest[i] = bp_src[i];
			}
			return size;
		}
	}

	UInt zero_memory(void* address, UInt size) noexcept
	{
		byte* bptr = (byte*)address;

		// If very small, zero byte-by-byte
		if (size < (sizeof(UInt) << 4))
		{
			for (UInt i = 0; i < size; i++)	{
				bptr[i] = 0;
			}
			return 0; // Finished
		}
		else
		{
			// (Q/D)word-align address
			UInt iaddr = (UInt)address;
			UInt trim  = sizeof(UInt)-(iaddr % sizeof(UInt));
			size -= trim;

			// Get the number of integers to zero
			UInt  intCount = size / sizeof(UInt);
			UInt* iptr = (UInt*)(bptr + trim);

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
}
