#include "memory.hpp"

namespace std
{

	size_t copy_memory(const void* source, void* destination, size_t size) noexcept
	{
		const uint8_t* bp_src  = static_cast<const uint8_t*>(source);
		      uint8_t* bp_dest = static_cast<uint8_t*>(destination);

		if (bp_dest > bp_src && bp_dest < bp_src + size) { return 0; }
		size_t diff = bp_dest - bp_src;

		// If very small or very misaligned, zero uint8_t-by-uint8_t
		if (size > (sizeof(size_t) << 5) || diff % 2 != 0)
		{
			for (size_t i = 0; i < size; i++)	{
				bp_dest[i] = bp_src[i];
			}
			return size; // Finished
		}
		else
		{
			size_t i_src = reinterpret_cast<size_t>(source);
			size_t i_dst = reinterpret_cast<size_t>(destination);

			// Get best alignment
			size_t alignment = 2;
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
			size_t correction = i_src % alignment;
			if (correction != 0)
			{
				correction = alignment - correction;

				i_src += correction;
				i_dst += correction;
			}

			size_t copied = 0;

			// Copy leading bytes
			for (size_t i = 0; i < correction; i++) {
				bp_dest[i] = bp_src[i];
			}
			copied += correction;

			// Performed aligned copy
			if (alignment == 2)
			{
				size_t iterations = (size - correction) / 2;

				for (size_t i = 0; i < iterations; i++) {
					reinterpret_cast<uint16_t*>(i_dst)[i] = reinterpret_cast<uint16_t*>(i_src)[i];
				}
				copied += iterations * 2;
			}
			else if (alignment == 4)
			{
				size_t iterations = (size - correction) / 4;

				for (size_t i = 0; i < iterations; i++) {
					reinterpret_cast<uint32_t*>(i_dst)[i] = reinterpret_cast<uint32_t*>(i_src)[i];
				}
				copied += iterations * 4;
			}
			else
			{
				size_t iterations = (size - correction) / 8;

				for (size_t i = 0; i < iterations; i++) {
					reinterpret_cast<uint64_t*>(i_dst)[i] = reinterpret_cast<uint64_t*>(i_src)[i];
				}
				copied += iterations * 8;
			}

			// Get trailing pointer
			bp_src  += copied;
			bp_dest += copied;

			// Copy trailing bytes
			for (size_t i = 0; i < size - copied; i++) {
				bp_dest[i] = bp_src[i];
			}
			return size;
		}
	}

	size_t zero_memory(void* address, size_t size) noexcept
	{
		uint8_t* bptr = (uint8_t*)address;

		// If very small, zero uint8_t-by-uint8_t
		if (size < (sizeof(size_t) << 4))
		{
			for (size_t i = 0; i < size; i++)	{
				bptr[i] = 0;
			}
			return 0; // Finished
		}
		else
		{
			// (Q/D)word-align address
			size_t iaddr = (size_t)address;
			size_t trim  = sizeof(size_t)-(iaddr % sizeof(size_t));
			size -= trim;

			// Get the number of integers to zero
			size_t  intCount = size / sizeof(size_t);
			size_t* iptr = (size_t*)(bptr + trim);

			// Zero int-wise
			for (size_t i = 0; i < intCount; i++)
			{
				iptr[i] = 0;
			}
			size -= intCount * sizeof(size_t);

			// Zero leading bits
			for (size_t i = 0; i < trim; i++)	{
				bptr[i] = 0;
			}

			// Get trailing pointer
			bptr = bptr + trim + intCount * sizeof(size_t);

			// Zero trailing bits
			for (size_t i = 0; i < size; i++)	{
				bptr[i] = 0;
			}

			return size; // Return bits not zeroed (should be 0!)
		}
	}

}