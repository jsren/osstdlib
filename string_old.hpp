/* string.hpp - (c) James S Renwick 2013
   -------------------------------------
   Version 1.1.0

   TODO: SSO!
*/
#pragma once
#include "std"

/*
==========================================
CONSIDER: String<T>
          typedef String<char>     string;
		  typedef String<char16_t> lstring;

	Note that this would force function definition to occur
	within the header file.
==========================================
*/

namespace std
{
	/*
	Core data type for the handling of mutable text objects.	
	Backed by a dynamically-sized char array. 
	This implementation is not threadsafe.
	*/
	class String
	{
	private:
		char* data;
		size_t length;
		size_t realLength;
		
		String(size_t, size_t);

		void __resize(size_t);

	public:
		String();
		String(const char* data);
		String(const String& str);
		String(const char* data, size_t length);

		// Move assignment
		String& operator =(String&& string) noexcept;

		/* Gets the length, in characters, of this string. */
		inline size_t getLength() const noexcept { return this->length; }

		/*
		Gets the character at the given index. No bounds checking is
		performed.

		index - The index of the character to retrieve.
		*/
		inline char getCharAt(size_t index) const noexcept{ return this->data[index]; }


		/*
		Sets the character at the given index. Returns false if the index
		does not pass bounds checks.

		character - The new value.
		index     - The index of the character to set.
		*/
		bool setCharAt(char character, size_t index) noexcept;

		/*
		Copies up to 'length' chars of string data to the given buffer.
		Returns the number of characters copied.

		buffer - The buffer into which to copy the data.
		length - The maximum number of characters to copy.
		*/
		size_t copyTo(char* buffer, size_t length) noexcept;

		/*
		Creates a new String which is a substring of the current string.
		Should the given length go beyond the string length, the substring
		will be taken from the given index until the end of the string. If
		the start index is beyond the string, an empty string will be 
		returned.

		startIndex - The index from which the substring should be taken.
		length     - The length, in characters, of the substring.
		*/
		String substring(size_t startIndex, size_t length) const;


		/*
		Removes the specified number of characters from the string, returning
		the number of characters actually removed. 

		startIndex - The index, inclusive, from where removal should begin.
		length     - The number of characters to remove.
		*/
		size_t remove(size_t startIndex, size_t length);


		/*
		Returns the index within the string of the given substring,
		or -1 (size_tMax) if not found.

		substr - The substring for which to search.
		*/
		size_t indexOf(const String& substr) const noexcept;
		/*
		Returns the index within the string of the given substring,
		or -1 (size_tMax) if not found.

		substr     - The substring for which to search.
		startIndex - The index (inclusive) at which to begin the search.
		*/
		size_t indexOf(const String& substr, size_t startIndex) const noexcept;


		/*
		Tests whether the string starts with the given substring.

		prefix - The prefix for which to test.
		*/
		bool startsWith(const String& prefix) const noexcept;
		/*
		Tests whether the string starts with the given character array.

		prefix - The prefix for which to test.
		length - The length of the character array.
		*/
		bool startsWith(const char* prefix, size_t length) const noexcept;


		/*
		Tests whether the string ends with the given substring.

		suffix - The suffix for which to test.
		*/
		bool endsWith(const String& suffix) const noexcept;
		/*
		Tests whether the string ends with the given character array.

		suffix - The suffix for which to test.
		length - The length of the character array.
		*/
		bool endsWith(const char* suffix, size_t length) const noexcept;


		/*
		Reallocates the string data to the string's length, 
		potentially freeing unused space.
		*/
		void freeToSize();



		// === OPERATORS === 
		String& operator +=(char c1);
		String& operator +=(const char* s1);
		String& operator +=(const String& s1);

		bool operator ==(const String& s1) const;

		inline char operator [](size_t index) noexcept {
			return getCharAt(index);
		};
		inline char operator [](size_t index) const noexcept {
			return getCharAt(index);
		};

		/*
		Copies the given null-termintated character array
		to the current string. Returns the current string or nullptr 
		if the resulting string is too large.
		*/
		String& operator =(const char* s1);
		/*
		Copies the given string to the current string. Returns the current string.
		*/
		String& operator =(const String& s1);

		/*
		Creates a new string formed from the concatenation of
		this string and the given character.
		*/
		String operator +(char c) const;
		/*
		Creates a new string formed from the concatenation of
		this string and the given string.
		*/
		String operator +(const String& s2) const;
		/*
		Creates a new string formed from the concatenation of
		this string and the given null-terminated character array.
		*/
		String operator +(const char* s2) const;


		// === STATIC MEMBERS === 

		/* The empty string. */
		static const String Empty;


		/* 
		Gets the length, in bytes, of a null-terminated character array.
		This length does not include the terminator.
		
		data - the byte array for which to get the length
		*/
		static size_t getNullLength(const char* data) noexcept;


		/*
		Replaces "output" with an 'unsafe' string - one whose value is not
		solely controlled by the string object.

		Note that any String functions which alter the underlying data will also
		affect the given character array.

		data   - The array from which the string will be generated.
		output - The String object to construct.
		*/
		inline static void _getUnsafe(char* data, String& output)
		{
			output.data       = data;
			output.length     = getNullLength(data);
			output.realLength = output.length;
		}
		/*
		Replaces "output" with an 'unsafe' string - one whose value is not
		solely controlled by the string object.

		Note that any String functions which alter the underlying data will also
		affect the given character array.

		data   - The array from which the string will be generated.
		length - The length, in bytes, of the string.
		output - The String object to construct.
		*/
		inline static void _getUnsafe(char* data, size_t length, String& output)
		{
			output.data       = data;
			output.length     = length;
			output.realLength = length;
		}
	};
}
