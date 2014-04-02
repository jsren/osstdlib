/* string.h - (c) James S Renwick 2013
   -----------------------------------
   Version 1.0.4
*/
#pragma once
#include <stdlib/std.h>
#include <stdlib/array.h>

/*
==========================================
CONSIDER: String<T>
          typedef String<char>    string;
		  typedef String<wchar_t> wstring;

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
		UInt length;
		UInt realLength;

		String(UInt, UInt);

	public:

		String() : data(nullptr(char*)), length(0), realLength(0) { };

		String(const char* data);
		String(const String& str);
		String(const char* data, UInt length);

		/* Gets the length, in characters, of this string. */
		inline UInt getLength() const { return this->length; }


		/*
		Gets the character at the given index. No bounds checking is
		performed.

		index - The index of the character to retrieve.
		*/
		inline char getCharAt(UInt index) const { return this->data[index]; }


		/*
		Sets the character at the given index. Returns false if the index
		does not pass bounds checks.

		character - The new value.
		index     - The index of the character to set.
		*/
		bool setCharAt(char character, UInt index);


		/*
		Creates a new String which is a substring of the current string.
		Should the given length go beyond the string length, the substring
		will be taken from the given index until the end of the string.

		startIndex - The index from which the substring should be taken.
		length     - The length, in characters, of the substring.
		*/
		String* substring(UInt startIndex, UInt length);


		/*
		Removes the specified number of characters from the string, returning
		the number of characters actually removed. 

		startIndex - The index, inclusive, from where removal should begin.
		length     - The number of characters to remove.
		*/
		UInt remove(UInt startIndex, UInt length);


		/*
		Returns the index within the string of the given substring,
		or -1 (UIntMax) if not found.

		substr - The substring for which to search.
		*/
		UInt indexOf(const String& substr) const;
		/*
		Returns the index within the string of the given substring,
		or -1 (UIntMax) if not found.

		substr     - The substring for which to search.
		startIndex - The index (inclusive) at which to begin the search.
		*/
		UInt indexOf(const String& substr, UInt startIndex) const;


		/*
		Tests whether the string starts with the given substring.

		prefix - The prefix for which to test.
		*/
		bool startsWith(const String& prefix) const;
		/*
		Tests whether the string starts with the given character array.

		prefix - The prefix for which to test.
		length - The length of the character array.
		*/
		bool startsWith(const char* prefix, UInt length) const;


		/*
		Tests whether the string ends with the given substring.

		suffix - The suffix for which to test.
		*/
		bool endsWith(const String& suffix) const;
		/*
		Tests whether the string ends with the given character array.

		suffix - The suffix for which to test.
		length - The length of the character array.
		*/
		bool endsWith(const char* suffix, UInt length) const;


		/*
		Reallocates the string data to the string's length, 
		potentially freeing unused space.
		*/
		void freeToSize();



		// === OPERATORS === 
		void operator +=(char c1);
		void operator +=(const char* s1);
		void operator +=(const String& s1);

		bool operator ==(const String* s1) const;

		inline char operator [](UInt index) {
			return getCharAt(index);
		};
		inline char operator [](UInt index) const {
			return getCharAt(index);
		};

		/*
		Creates a new string formed from the concatenation of
		this string and the given character. Returns null if the resulting
		string is too large.
		*/
		String* operator +(char c) const;
		/*
		Creates a new string formed from the concatenation of
		this string and the given string. Returns null if the resulting
		string is too large.
		*/
		String* operator +(const String& s2) const;
		/*
		Creates a new string formed from the concatenation of
		this string and the given null-terminated character array. 
		Returns null if the resulting string is too large.
		*/
		String* operator +(const char* s2) const;


		// === STATIC MEMBERS === 

		/* The empty string. */
		static const String Empty;


		/* 
		Gets the length, in bytes, of a null-terminated character array.
		This length does not include the terminator.
		
		data - the byte array for which to get the length
		*/
		static UInt getNullLength(const char* data);


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
		inline static void _getUnsafe(char* data, UInt length, String& output)
		{
			output.data       = data;
			output.length     = length;
			output.realLength = length;
		}
	};

	// Empty string definition
	const String String::Empty = String();
}