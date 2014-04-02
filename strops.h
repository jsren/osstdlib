#pragma once
#include "string.h"

namespace std
{
	Int32 parseInt32 (const String& s);
	Int64 parseInt64 (const String& s);
	Int32 parseUInt32(const String& s);
	Int64 parseUInt64(const String& s);

	float  parseFloat (const String& s);
	double parseDouble(const String& s);

	String* toString(const Int32 i);
	String* toString(const Int64 i);
	String* toString(const UInt32 i);
	String* toString(const UInt64 i);
	
	String* toString(const float f);
	String* toString(const double d);

	/*=================================================================*/
}