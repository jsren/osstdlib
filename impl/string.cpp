/* string.cpp - (c) James S Renwick 2013
   -----------------------------------
   Version 1.0.6
*/
#include <stdlib/math.h>
#include <stdlib/string.h>

namespace std
{
	// Holds the maximum amount by which the string
	// will be over-allocated
	const UInt maxStrExtra = 120;

	void String::__resize(UInt length) noexcept
	{
		UInt extra = length >> 1;
		this->realLength = length + std::min(extra, maxStrExtra);

		// Account for integer overflow
		if (this->realLength < length) { this->realLength = length; }

		char* tmpData = this->data;

		this->data = new char[this->realLength];

		// Copy old data if present
		if (tmpData != nullptr)
		{
			for (UInt i = 0; i < this->length; i++) {
				this->data[i] = tmpData[i];
			}
			delete[] tmpData;
		}

		// The max length of a string is actually UIntMax-1
		if (this->length == UIntMax) this->length = UIntMax - 1;
	}

	// === CONSTRUCTORS ===
	String::String(UInt length, UInt realLength) noexcept : length(length), realLength(realLength)
	{
		this->data = new char[realLength];
	}
	String::String(const char* buffer) : data(nullptr)
	{
		this->length = String::getNullLength(buffer);
		this->__resize(this->length);

		for (UInt i = 0; i < this->length; i++) {
			this->data[i] = buffer[i];
		}
	}
	String::String(const char* buffer, UInt length) noexcept : data(nullptr), length(length)
	{
		this->__resize(length);

		for (UInt i = 0; i < this->length; i++) {
			this->data[i] = buffer[i];
		}
	}
	String::String(const String& str) noexcept : length(str.length), realLength(str.realLength)
	{
		this->data = new char[str.realLength];

		for (UInt i = 0; i < str.length; i++) {
			this->data[i] = str.data[i];
		}

		// The max length of a string is actually UIntMax-1
		if (this->length == UIntMax) this->length = UIntMax - 1;
	}

	// === METHODS ===
	bool String::setCharAt(char c, UInt index) noexcept
	{
		if (index < this->length)
		{
			this->data[index] = c;
			return true;
		}
		else return false;
	}

	String* String::substring(UInt startIndex, UInt length) noexcept
	{
		if (startIndex > this->length) {
			return new String(String::Empty);
		}

		UInt sublen = this->length - startIndex;
		if (length > sublen) { length = sublen; }

		return new String(this->data + startIndex, length);
	}

	UInt String::remove(UInt startIndex, UInt length) noexcept
	{
		UInt sublen   = this->length - startIndex;
		UInt endIndex = startIndex + length;

		// Check for valid input
		if (length == 0 || startIndex > this->length ||
			endIndex < std::max(startIndex, length))
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
			
			for (UInt i = 0, n = endIndex; n < this->length; i++, n++) {
				remStart[i] = this->data[n];
			}
		}
		// Too much over-alloc so resize
		if (this->realLength - length >= maxStrExtra) {
			this->__resize(this->length);
		}
		return length;
	}

	UInt String::indexOf(const String& substr) const noexcept
	{
		for (UInt i = 0; i < this->length; i++)
		{
			bool found = true;
			for (UInt n = 0; n < substr.length; n++)
			{
				if (this->data[i+n] != substr.data[n])
				{
					found = false;
					break;
				}
			}
			if (found) { return i; }
		}
		return UIntMax; // -1
	}
	UInt String::indexOf(const String& substr, UInt startIndex) const noexcept
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

	bool String::startsWith(const String& prefix) const noexcept
	{
		if (prefix.length > this->length) return false;

		for (UInt n = 0; n < prefix.length; n++) {
			if (this->data[n] != prefix.data[n]) return false;
		}
		return true;
	}
	bool String::startsWith(const char* prefix, UInt length) const noexcept
	{
		if (length > this->length) return false;

		for (UInt n = 0; n < length; n++) {
			if (this->data[n] != prefix[n]) return false;
		}
		return true;
	}

	bool String::endsWith(const String& suffix) const noexcept
	{
		if (suffix.length > this->length) return false;

		char* end1 = this->data + this->length - 1;
		char* end2 = suffix.data + suffix.length - 1;

		for (UInt n = 0; n != suffix.length; ++n) {
			if (*(end1 - n) != *(end2 - n)) return false;
		}
		return true;
	}
	bool String::endsWith(const char* suffix, UInt length) const noexcept
	{
		if (length > this->length) return false;

		for (UInt n = length; n != 0; n--) {
			if (this->data[n-1] != suffix[n-1]) return false;
		}
		return true;
	}

	void String::freeToSize() noexcept
	{
		char* buffer = new char[this->length];

		for (UInt i = 0; i < this->length; i++) {
			buffer[i] = this->data[i];
		}
		// Delete previous and swap
		delete[] this->data;
		this->data = buffer;
	}

	UInt String::getNullLength(const char* data) noexcept
	{
		for (UInt i = 0; i < UIntMax; i++) {
			if (data[i] == '\0') return i;
		}
		return UIntMax;
	}

	// === OPERATORS ===

	String* String::operator +=(char c1) noexcept
	{
		// Check for max string length
		if (this->length == UIntMax - 1) {
			return nullptr;
		}
		// Resize as necessary
		if (this->realLength - this->length == 0) {
			this->__resize(this->length + 1);
		}
		this->data[this->length++] = c1;

		// Return this
		return this;
	}
	String* String::operator +=(const char* s1) noexcept
	{
		UInt appendLen = String::getNullLength(s1);
		UInt newLen    = this->length + appendLen;

		// Resize as required
		if (appendLen > this->realLength - this->length)
		{
			// Catch overflow
			if (newLen < std::max(this->length, appendLen)) {
				return nullptr;
			}
			else this->__resize(newLen);
		}

		// Copy other string
		char* append = this->data + this->length;
		
		for (UInt i = 0; i < appendLen; i++) {
			append[i] = s1[i];
		}
		this->length = newLen;

		// Return this
		return this;
	}
	String* String::operator +=(const String& s1) noexcept
	{
		UInt newLen = this->length - s1.length;

		// Resize as required
		if (s1.length > this->realLength - this->length)
		{
			// Catch overflow
			if (newLen < std::max(this->length, s1.length)) {
				return nullptr;
			}
			else this->__resize(newLen);
		}

		// Copy other string
		char* append = this->data + this->length;

		for (UInt i = 0; i < s1.length; i++) {
			append[i] = s1[i];
		}
		this->length = newLen;

		// Return this
		return this;
	}

	bool String::operator ==(const String* s1) const noexcept
	{
		if (s1->getLength() != this->length) return false;

		for (UInt i = 0; i < this->length; i++) {
			if (this->getCharAt(i) != s1->getCharAt(i)) return false;
		}
		return true;
	}

	String* String::operator +(char c) const noexcept
	{
		String* output;

		if (this->length == UIntMax - 1) { 
			return nullptr; 
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
	String* String::operator +(const String& s1) const noexcept
	{
		UInt newLen     = this->length + s1.length;
		UInt extra      = newLen >> 1;
		UInt newRealLen = newLen + std::min(extra, maxStrExtra);

		// Check that the new length is not too large
		if (newLen < std::max(this->length, s1.length)) {
			return nullptr;
		}
		// Account for integer overflow
		else if (newRealLen < newLen) { newRealLen = newLen; }

		// Copy data to new string
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
	String* String::operator +(const char* s1) const noexcept
	{
		UInt s1Length = String::getNullLength(s1);

		UInt newLen     = this->length + s1Length;
		UInt extra      = newLen >> 1;
		UInt newRealLen = newLen + std::min(extra, maxStrExtra);

		// Check that the new length is not too large
		if (newLen < std::max(this->length, s1Length)) {
			return nullptr;
		}
		// Account for integer overflow
		else if (newRealLen < newLen) { newRealLen = newLen; }

		// Copy data to new string
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
