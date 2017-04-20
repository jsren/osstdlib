/* uuid.hpp - (c) James S Renwick 2013
   -----------------------------------
   Version 1.0.4
*/
#pragma once
#include "std"

namespace std
{
	// Represents an ISO/IEC 11578:1996-compliant Universally Unique IDentifier.
	struct UUID
	{
	public:
		// The size of a compliant UUID
		static const size_t size = 16;

		// Represents a UUID's variant.
		// Taken from "boost/uuid/uuid.hpp".
		enum Variant 
		{
			variant_ncs,       // NCS backward compatibility.
			variant_rfc_4122,  // Defined in RFC 4122 document.
			variant_microsoft, // Microsoft Corporation backward compatibility.
			variant_future     // Future definition.
		};

		// Represents a UUID's version.
		// Taken from "boost/uuid/uuid.hpp".
		enum Version 
		{
			version_unknown             = -1, // Unknown UUID version.
			version_time_based          =  1, // Combination of MAC address and time.
			version_dce_security        =  2, // Similar to v1 but with the upper byte of the clock sequence replaced by the identifier for a "local domain".
			version_name_based_md5      =  3, // UUID constructed from the MD5 hash of a particular name.
			version_random_number_based =  4, // UUID relying on only random numbers.
			version_name_based_sha1     =  5  // Similar to v3 but using the truncated output from a SHA-1 hash function.
		};

	public:
		// The uuid's byte data
		byte data[UUID::size];

		// Creates an empty UUID
		UUID() noexcept
		{
			for (size_t i = 0; i < UUID::size; i++) {
				this->data[i] = 0;
			}
		}

		explicit UUID(const byte* const data) noexcept
		{
			for (size_t i = 0; i < UUID::size; i++) {
				this->data[i] = data[i];
			}
		}

		/* Gets if the UUID is a 'nil' UUID. */
		bool isNilUUID() const noexcept
		{
			for (size_t i = 0; i < size; i++) {
				if (data[i] != 0U) return false; 
			}
			return true;
		}

		/* Gets the variant of the current UUID. */
		Variant getVariant() noexcept
		{
			byte variant_value = data[8];

				 if ((variant_value & 0x80) == 0x00 ) return variant_ncs;       // 0b0xxxxxxx
			else if ((variant_value & 0xC0) == 0x80 ) return variant_rfc_4122;  // 0b10xxxxxx
			else if ((variant_value & 0xE0) == 0xC0 ) return variant_microsoft; // 0b110xxxxx
			else return variant_future; // 0b111xxxx
		}

		/* Gets the version of the current UUID. */
		Version getVersion() noexcept
		{
			switch (data[6] & 0xF0)
			{
			case 0x10: return version_time_based;
			case 0x20: return version_dce_security;
			case 0x30: return version_name_based_md5;
			case 0x40: return version_random_number_based;
			case 0x50: return version_name_based_sha1;
			default:   return version_unknown;
			}
		}
	};
}
