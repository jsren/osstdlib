///* string.cpp - (c) James S Renwick 2013
//   -------------------------------------
//   Version 1.1.0
//*/
//#include "math.hpp"
//#include "string.hpp"
//
//namespace std
//{
//	// Holds the maximum amount by which the string
//	// will be over-allocated
//	static const size_t maxStrExtra = 120;
//
//	// Empty string definition
//	const String String::Empty = String(String());
//
//	void String::__resize(size_t length) 
//	{
//		size_t extra = length >> 1;
//		this->realLength = length + std::min(extra, maxStrExtra);
//
//		// Account for integer overflow
//		if (this->realLength < length) { this->realLength = length; }
//
//		char* tmpData = this->data;
//
//		this->data = new char[this->realLength];
//
//		// Copy old data if present
//		if (tmpData != nullptr)
//		{
//			for (size_t i = 0; i < this->length; i++) {
//				this->data[i] = tmpData[i];
//			}
//			delete[] tmpData;
//		}
//
//		// The max length of a string is actually size_tMax-1
//		if (this->length == size_tMax) this->length = size_tMax - 1;
//	}
//
//	// === CONSTRUCTORS ===
//	String::String() : data(nullptr), length(0), realLength(0)
//	{
//	}
//	String::String(size_t length, size_t realLength) : length(length), realLength(realLength)
//	{
//		this->data = new char[realLength];
//	}
//	String::String(const char* buffer) : data(nullptr)
//	{
//		this->length = String::getNullLength(buffer);
//		this->__resize(this->length);
//
//		for (size_t i = 0; i < this->length; i++) {
//			this->data[i] = buffer[i];
//		}
//	}
//	String::String(const char* buffer, size_t length)  : data(nullptr), length(length)
//	{
//		this->__resize(length);
//
//		for (size_t i = 0; i < this->length; i++) {
//			this->data[i] = buffer[i];
//		}
//	}
//	String& String::operator=(String && string) noexcept
//	{
//		char* tmp = this->data;
//		this->data = string.data;
//		string.data = tmp;
//
//		this->length = string.length;
//		this->realLength = string.realLength;
//
//		string.length = string.realLength = 0;
//
//		return *this;
//	}
//	String::String(const String& str)  : length(str.length), realLength(str.realLength)
//	{
//		this->data = new char[str.realLength];
//
//		for (size_t i = 0; i < str.length; i++) {
//			this->data[i] = str.data[i];
//		}
//
//		// The max length of a string is actually size_tMax-1
//		if (this->length == size_tMax) this->length = size_tMax - 1;
//	}
//
//	// === METHODS ===
//	bool String::setCharAt(char c, size_t index) noexcept
//	{
//		if (index < this->length)
//		{
//			this->data[index] = c;
//			return true;
//		}
//		else return false;
//	}
//
//	size_t String::copyTo(char* buffer, size_t length) noexcept
//	{
//		length = min(this->length, length);
//
//		size_t i = 0;
//		for (; i < length; i++) {
//			buffer[i] = this->data[i];
//		}
//		return i;
//	}
//
//	String String::substring(size_t startIndex, size_t length) const
//	{
//		if (startIndex > this->length) {
//			return String();
//		}
//
//		size_t sublen = this->length - startIndex;
//		if (length > sublen) { length = sublen; }
//
//		return String(this->data + startIndex, length);
//	}
//
//	size_t String::remove(size_t startIndex, size_t length) 
//	{
//		size_t sublen   = this->length - startIndex;
//		size_t endIndex = startIndex + length;
//
//		// Check for valid input
//		if (length == 0 || startIndex > this->length ||
//			endIndex < std::max(startIndex, length))
//		{
//			return 0;
//		}
//		else if (endIndex > this->length)
//		{
//			length = this->length - startIndex;
//			this->length = startIndex;
//		}
//		else this->length -= length;
//
//		// If not just removing from the end
//		if (length != sublen)
//		{
//			if (length < sublen) { sublen = length; }
//
//			char* remStart = this->data + startIndex;
//			
//			for (size_t i = 0, n = endIndex; n < this->length; i++, n++) {
//				remStart[i] = this->data[n];
//			}
//		}
//		// Too much over-alloc so resize
//		if (this->realLength - length >= maxStrExtra) {
//			this->__resize(this->length);
//		}
//		return length;
//	}
//
//	size_t String::indexOf(const String& substr) const noexcept
//	{
//		for (size_t i = 0; i < this->length; i++)
//		{
//			bool found = true;
//			for (size_t n = 0; n < substr.length; n++)
//			{
//				if (this->data[i+n] != substr.data[n])
//				{
//					found = false;
//					break;
//				}
//			}
//			if (found) { return i; }
//		}
//		return size_tMax; // -1
//	}
//	size_t String::indexOf(const String& substr, size_t startIndex) const noexcept
//	{
//		const char* data   = (this->data + startIndex);
//		const size_t  length = this->length - startIndex;
//
//		for (size_t i = 0; i < length; i++)
//		{
//			bool found = true;
//			for (size_t n = 0; n < substr.length; n++)
//			{
//				if (data[i] != substr.data[n])
//				{
//					found = false;
//					break;
//				}
//			}
//			if (found) { return i + startIndex; }
//		}
//		return size_tMax; // -1
//	}
//
//	bool String::startsWith(const String& prefix) const noexcept
//	{
//		if (prefix.length > this->length) return false;
//
//		for (size_t n = 0; n < prefix.length; n++) {
//			if (this->data[n] != prefix.data[n]) return false;
//		}
//		return true;
//	}
//	bool String::startsWith(const char* prefix, size_t length) const noexcept
//	{
//		if (length > this->length) return false;
//
//		for (size_t n = 0; n < length; n++) {
//			if (this->data[n] != prefix[n]) return false;
//		}
//		return true;
//	}
//
//	bool String::endsWith(const String& suffix) const noexcept
//	{
//		if (suffix.length > this->length) return false;
//
//		char* end1 = this->data + this->length - 1;
//		char* end2 = suffix.data + suffix.length - 1;
//
//		for (size_t n = 0; n != suffix.length; ++n) {
//			if (*(end1 - n) != *(end2 - n)) return false;
//		}
//		return true;
//	}
//	bool String::endsWith(const char* suffix, size_t length) const noexcept
//	{
//		if (length > this->length) return false;
//
//		for (size_t n = length; n != 0; n--) {
//			if (this->data[n-1] != suffix[n-1]) return false;
//		}
//		return true;
//	}
//
//	void String::freeToSize()
//	{
//		char* buffer = new char[this->length];
//
//		for (size_t i = 0; i < this->length; i++) {
//			buffer[i] = this->data[i];
//		}
//		// Delete previous and swap
//		delete[] this->data;
//		this->data = buffer;
//	}
//
//	size_t String::getNullLength(const char* data) noexcept
//	{
//		for (size_t i = 0; i < size_tMax; i++) {
//			if (data[i] == '\0') return i;
//		}
//		return size_tMax;
//	}
//
//	// === OPERATORS ===
//
//	String& String::operator +=(char c1)
//	{
//		// Check for max string length
//		if (this->length == size_tMax - 1) return *this;
//		// Resize as necessary
//		if (this->realLength == this->length) {
//			this->__resize(this->length + 1);
//		}
//		this->data[this->length++] = c1;
//
//		// Return this
//		return *this;
//	}
//	String& String::operator +=(const char* s1)
//	{
//		size_t appendLen = String::getNullLength(s1);
//		size_t newLen    = std::boundedAdd(this->length, appendLen);
//
//		// Resize as required
//		if (newLen > this->realLength) this->__resize(newLen);
//
//		// Copy other string
//		char* append = this->data + this->length;
//		
//		for (size_t i = 0, n = this->length; 
//			i < appendLen && n < newLen; i++, n++)
//		{
//			append[i] = s1[i];
//		}
//		this->length = newLen;
//
//		// Return this
//		return *this;
//	}
//	String& String::operator +=(const String& s1)
//	{
//		size_t newLen = std::boundedAdd(this->length, s1.length);
//
//		// Resize as required
//		if (newLen > this->realLength) this->__resize(newLen); 
//
//		// Copy other string
//		char* append = this->data + this->length;
//
//		for (size_t i = 0, n = this->length; 
//			i < s1.length && n < newLen; i++, n++)
//		{
//			append[i] = s1[i];
//		}
//		this->length = newLen;
//
//		// Return this
//		return *this;
//	}
//
//	bool String::operator ==(const String& s1) const
//	{
//		if (s1.getLength() != this->length) return false;
//
//		for (size_t i = 0; i < this->length; i++) {
//			if (this->getCharAt(i) != s1.getCharAt(i)) return false;
//		}
//		return true;
//	}
//
//	String String::operator +(char c) const
//	{
//		// If reached max length, just return copy of original
//		if (this->length > size_tMax - 2) return *this;
//
//		String output;
//
//		// Create new string
//		if (this->realLength < size_tMax - 1) {
//			output = String(this->length + 1, this->realLength + 1);
//		}
//		else output = String(this->length, this->realLength);
//
//		// Copy data
//		size_t i = 0;
//		for (; i < this->length; i++) {
//			output.data[i] = this->data[i];
//		}
//		output.data[i] = c;
//
//		return output;
//	}
//	String String::operator +(const String& s1) const
//	{
//		size_t newLen     = std::boundedAdd(this->length, s1.length);
//		size_t extra      = newLen >> 1;
//		size_t newRealLen = std::boundedAdd(newLen, std::min(extra, maxStrExtra));
//
//		// Copy data to new string
//		String output = String(newLen, newRealLen);
//
//		size_t n = 0;
//		for (size_t i = 0; i < this->length && n < newLen; i++, n++) {
//			output.data[i] = this->data[i];
//		}
//		
//		char* append = output.data + this->length;
//
//		for (size_t i = 0; i < s1.length && n < newLen; i++, n++) {
//			append[i] = s1.data[i];
//		}
//		return output;
//	}
//	String String::operator +(const char* s1) const
//	{
//		size_t s1Length = String::getNullLength(s1);
//
//		size_t newLen     = std::boundedAdd(this->length, s1Length);
//		size_t extra      = newLen >> 1;
//		size_t newRealLen = std::boundedAdd(newLen, std::min(extra, maxStrExtra));
//
//		// Copy data to new string
//		String output = String(newLen, newRealLen);
//
//		size_t n = 0;
//		for (size_t i = 0; i < this->length && n < newLen; i++, n++) {
//			output.data[i] = this->data[i];
//		}
//		
//		char* append = output.data + this->length;
//
//		for (size_t i = 0; i < s1Length && n < newLen; i++, n++) {
//			append[i] = s1[i];
//		}
//		return output;
//	}
//
//	String& String::operator =(const char* s1)
//	{
//		size_t newlen  = String::getNullLength(s1);
//		this->length = std::min(newlen, size_tMax - 1);
//
//		if (this->length > this->realLength) {
//			this->__resize(this->length);
//		}
//		for (size_t i = 0; i < this->length; i++) {
//			this->data[i] = s1[i];
//		}
//		return *this;
//	}
//	String& String::operator =(const String& s1)
//	{
//		if ((this->length = s1.length) > this->realLength) {
//			this->__resize(this->length);
//		}
//		for (size_t i = 0; i < this->length; i++) {
//			this->data[i] = s1.data[i];
//		}
//		return *this;
//	}
//}
