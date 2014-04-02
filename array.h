/* array.h - (c) James S Renwick 2013
   ----------------------------------
   Version 1.0.1
*/
#pragma once
#include "std.h"

namespace std
{
	template <class T>
	// Core data type for a fixed-size generic collection.
	class Array
	{
	private:
		// The internal length of the array
		UInt length;
		// The internal data of the array
		T*   arrayData;

	public:
		// Creates a new array with the specified fixed capacity.
		explicit Array(UInt capacity);
		// Destroys the current array.
		~Array();

		/* Gets the number of items in the array. */
		UInt getLength() const;

		/* Gets the item at the specified index. */
		T getItem(UInt index) const;
		/* Sets the item at the specified index. */
		void setItem(UInt index, T item);

		/* Shallow-copies the members of the current array to a destination array. */
		void copyTo(Array<T>& destination) const;

	public:
		/* Gets the item at the specified index. */
		inline T operator[] (UInt index) const { return this->getItem(index); }

		/* Resizes the current array, trimming items if smaller. */
		static Array<T>* resize(const Array<T>& arr, UInt newSize);
	};

	template <class T>
	inline Array<T>::Array(UInt capacity)
	{
		this->length    = capacity;
		this->arrayData = new T[capacity];
	}

	template <class T>
	inline Array<T>::~Array()
	{
		delete[] this->arrayData;
	}

	template <class T>
	inline UInt Array<T>::getLength() const
	{
		return this->length;
	}

	template <class T>
	inline T Array<T>::getItem(UInt index) const
	{
		return this->arrayData[index];
	}

	template <class T>
	inline void Array<T>::setItem(UInt index, T item)
	{
		this->arrayData[index] = item;
	}

	template <class T>
	void Array<T>::copyTo(Array<T>& destination) const
	{
		UInt l = length > destination->length ? length : destination->length;
		for (UInt i = 0; i < l; i++)
		{
			destination->arrayData[i] = this->arrayData[i];
		}
	}

	template <class T>
	Array<T>* Array<T>::resize(const Array<T>& arr, UInt newSize)
	{
		Array<T>* newArr = new Array<T>(newSize);
		arr->copyTo(newArr);

		return newArr;
	}
}
