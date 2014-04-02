/* version.h - (c) James S Renwick 2013
   ------------------------------------
   Version 1.0.1
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
		explicit Version(byte* data);
		Version(UInt major, UInt minor, UInt build, UInt revision);

	public:
		inline UInt getMajorVersion()  { return _data[0]; }
		inline UInt getMinorVersion()  { return _data[1]; }
		inline UInt getMajorRevision() { return _data[2]; }
		inline UInt getMinorRevision() { return _data[3]; }
	};
}
