/* version.h - (c) James S Renwick 2013
   ------------------------------------
   Version 1.0.3
*/
#pragma once
#include "std.h"

namespace std
{
	// Data type representing a four-part version.
	struct Version
	{
	private:
		byte _data[4];

	public:
		explicit Version(const byte* const data) noexcept;
		Version(byte major, byte minor, byte build, byte revision) noexcept;

	public:
		inline UInt getMajorVersion()  noexcept { return _data[0]; }
		inline UInt getMinorVersion()  noexcept { return _data[1]; }
		inline UInt getMajorRevision() noexcept { return _data[2]; }
		inline UInt getMinorRevision() noexcept { return _data[3]; }
	};

	Version::Version(const byte* const data) noexcept
	{
		for (UInt i = 0; i < sizeof(this->_data); i++) {
			this->_data[i] = data[i];
		}
	}
	Version::Version(byte major, byte minor, byte build, byte revision) noexcept
	{
		this->_data[0] = major;
		this->_data[1] = minor;
		this->_data[2] = build;
		this->_data[3] = revision;
	}
}
