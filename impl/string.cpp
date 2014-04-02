/* string.cpp - (c) James S Renwick 2013
   -----------------------------------
   Version 1.0.4
*/
#include "../string.h"

namespace std
{
	// Holds the maximum amount by which the string
	// will be over-allocated
	const UInt maxStrExtra = 120;

	// === CONSTRUCTORS ===
	String::String(UInt length, UInt realLength) : length(length), realLength(realLength)
	{
		this->data = new char[realLength];
	}
	String::String(const char* buffer)
	{
		this->length = String::getNullLength(buffer);

		UInt extra = length >> 1;
		this->realLength = length + (extra < maxStrExtra ? extra : maxStrExtra);

		// Account for integer overflow
		if (this->realLength < length) { this->realLength = length; }

		this->data = new char[this->realLength];

		for (UInt i = 0; i < this->length; i++) {
			this->data[i] = buffer[i];
		}

		// The max length of a string is actually UIntMax-1
		if (this->length == UIntMax) this->length = UIntMax - 1;
	}
	String::String(const String& str) : length(str.length), realLength(str.realLength)
	{
		this->data = new char[str.realLength];

		for (int i = 0; i < str.length; i++) {
			this->data[i] = str.data[i];
		}

		// The max length of a string is actually UIntMax-1
		if (this->length == UIntMax) this->length = UIntMax - 1;
	}
	String::String(const char* data, UInt length) : length(length)
	{
		UInt extra = length >> 1;
		this->realLength = length + (extra < maxStrExtra ? extra : maxStrExtra);

		// Account for integer overflow
		if (this->realLength < length) { this->realLength = length; }

		this->data = new char[this->realLength];

		for (int i = 0; i < length; i++) {
			this->data[i] = data[i];
		}

		// The max length of a string is actually UIntMax-1
		if (this->length == UIntMax) this->length = UIntMax - 1;
	}

	// === METHODS ===
	bool String::setCharAt(char c, UInt index)
	{
		if (index < this->length)
		{
			this->data[index] = c;
			return true;
		}
		else return false;
	}

	String* String::substring(UInt startIndex, UInt length)
	{
		if (startIndex > this->length) {
			return nullptr(String*);
		}

		UInt sublen = this->length - startIndex;
		if (length < sublen) { sublen = length; }

		return new String(this->data + startIndex, sublen);
	}

	UInt String::remove(UInt startIndex, UInt length)
	{
		UInt sublen   = this->length - startIndex;
		UInt endIndex = startIndex + length;

		// Check for valid input
		if (length == 0 || startIndex > this->length ||
			endIndex < (startIndex > length ? startIndex : length))
		{
			return 0;
		}
		else if (endIndex > this->length)
		{
			length = this->length - startIndex;
			this->length = startIndex;
		}
		else this->length -= length;

		// If not just removing from the end
		if (length != sublen)
		{
			if (length < sublen) { sublen = length; }

			char* remStart = this->data + startIndex;
			
			for (int i = 0, n = endIndex; n < this->length; i++, n++) {
				remStart[i] = this->data[n];
			}
		}
		return length;
	}

	UInt String::indexOf(const String& substr) const
	{
		for (UInt i = 0; i < this->length; i++)
		{
			bool found = true;
			for (UInt n = 0; n < substr.length; n++)
			{
				if (this->data[i] != substr.data[n])
				{
					found = false;
					break;
				}
			}
			if (found) { return i; }
		}
		return UIntMax; // -1
	}
	UInt String::indexOf(const String& substr, UInt startIndex) const
	{
		const char* data   = (this->data + startIndex);
		const UInt  length = this->length - startIndex;

		for (UInt i = 0; i < length; i++)
		{
			bool found = true;
			for (UInt n = 0; n < substr.length; n++)
			{
				if (data[i] != substr.data[n])
				{
					found = false;
					break;
				}
			}
			if (found) { return i + startIndex; }
		}
		return UIntMax; // -1
	}

	bool String::startsWith(const String& prefix) const
	{
		if (prefix.length > this->length) return false;

		for (UInt n = 0; n < prefix.length; n++) {
			if (this->data[n] != prefix.data[n]) return false;
		}
		return true;
	}
	bool String::startsWith(const char* prefix, UInt length) const
	{
		if (length > this->length) return false;

		for (UInt n = 0; n < length; n++) {
			if (this->data[n] != prefix[n]) return false;
		}
		return true;
	}

	bool String::endsWith(const String& suffix) const
	{
		if (suffix.length > this->length) return false;

		for (UInt n = suffix.length; n != 0; n--) {
			if (this->data[n-1] != suffix.data[n-1]) return false;
		}
	}
	bool String::endsWith(const char* suffix, UInt length) const
	{
		if (length > this->length) return false;

		for (UInt n = length; n != 0; n--) {
			if (this->data[n-1] != suffix[n-1]) return false;
		}
	}

	void String::freeToSize()
	{
		char* buffer = new char[this->length];

		for (UInt i = 0; i < this->length; i++) {
			buffer[i] = this->data[i];
		}
		// Delete previous and swap
		delete[] this->data;
		this->data = buffer;
	}

	UInt String::getNullLength(const char* data)
	{
		for (UInt i; i < UIntMax; i++) {
			if (data[i] == '\0') return i;
		}
	}

	// === OPERATORS ===

	void String::operator +=(char c1)
	{
		if (this->realLength - this->length >= 1) {
			this->data[this->length] = c1;
		}
		this->length++;

		// The max length of a string is actually UIntMax-1
		if (this->length == UIntMax) this->length = UIntMax - 1;
	}
	void String::operator +=(const char* s1)
	{
		UInt length = String::getNullLength(s1);

		char* append = this->data + this->length;

		// If resize required, re-alloc
		if (this->realLength - this->length < length)
		{
			// Get new data array length
			this->realLength = this->length + length;
			UInt excess = this->realLength >> 1;
			this->realLength += (excess < maxStrExtra ? excess : maxStrExtra);

			// Account for integer overflow
			if (this->realLength < length) { this->realLength = length; }

			char* buffer = new char[this->realLength];
			
			for (UInt i = 0; i < this->length; i++) {
				buffer[i] = this->data[i];
			}
			delete[] this->data;
			this->data = buffer;
		}
		// Copy other string
		for (UInt i = 0; i < length; i++) {
			append[i] = s1[i];
		}
		this->length += length;

		// The max length of a string is actually UIntMax-1
		if (this->length == UIntMax) this->length = UIntMax - 1;
	}
	void String::operator +=(const String& s1)
	{
		char* append = this->data + this->length;

		// If resize required, re-alloc
		if (this->realLength - this->length < s1.length)
		{
			// Get new data array length
			this->realLength = this->length + s1.length;
			UInt excess = this->realLength >> 1;
			this->realLength += (excess < maxStrExtra ? excess : maxStrExtra);

			// Account for integer overflow
			if (this->realLength < length) { this->realLength = length; }

			char* buffer = new char[this->realLength];
			
			for (UInt i = 0; i < this->length; i++) {
				buffer[i] = this->data[i];
			}
			delete[] this->data;
			this->data = buffer;
		}
		// Copy other string
		for (UInt i = 0; i < s1.length; i++) {
			append[i] = s1[i];
		}
		this->length += s1.length;

		// The max length of a string is actually UIntMax-1
		if (this->length == UIntMax) this->length = UIntMax - 1;
	}

	bool String::operator ==(const String* s1) const
	{
		if (s1->getLength() != this->length) return false;

		for (UInt i = 0; i < this->length; i++) {
			if (this->getCharAt(i) != s1->getCharAt(i)) return false;
		}
		return true;
	}

	String* String::operator +(char c) const
	{
		String* output;

		if (this->length == UIntMax - 1) { 
			return nullptr(String*); 
		}
		else if (this->realLength != UIntMax -1) {
			output = new String(this->realLength + 1, this->length + 1);
		}
		else output = new String(this->realLength, this->length);

		UInt i = 0;
		for (; i < this->length; i++) {
			output->data[i] = this->data[i];
		}
		output->data[i] = c;

		return output;
	}
	String* String::operator +(const String& s1) const
	{
		UInt newLen     = this->length + s1.length;
		UInt extra      = newLen >> 1;
		UInt newRealLen = newLen + (extra < maxStrExtra ? extra : maxStrExtra);

		// Check that the new length is not too large
		if (newLen < (this->length > s1.length ? this->length : s1.length)) {
			return nullptr(String*);
		}
		// Account for integer overflow
		else if (newRealLen < newLen) { newRealLen = newLen; }


		String* output = new String(newLen, newRealLen);

		for (UInt i = 0; i < this->length; i++) {
			output->data[i] = this->data[i];
		}
		
		char* append = output->data + this->length;

		for (UInt i = 0; i < s1.length; i++) {
			append[i] = s1.data[i];
		}
		return output;
	}
	String* String::operator +(const char* s1) const
	{
		UInt s1Length = String::getNullLength(s1);

		UInt newLen     = this->length + s1Length;
		UInt extra      = newLen >> 1;
		UInt newRealLen = newLen + (extra < maxStrExtra ? extra : maxStrExtra);

		// Check that the new length is not too large
		if (newLen < (this->length > s1Length ? this->length : s1Length)) {
			return nullptr(String*);
		}
		// Account for integer overflow
		else if (newRealLen < newLen) { newRealLen = newLen; }


		String* output = new String(newLen, newRealLen);

		for (UInt i = 0; i < this->length; i++) {
			output->data[i] = this->data[i];
		}
		
		char* append = output->data + this->length;

		for (UInt i = 0; i < s1Length; i++) {
			append[i] = s1[i];
		}
		return output;
	}
}
