/* array.hpp - (c) James S Renwick 2013
   ------------------------------------
   Version 1.1.3
*/
#pragma once
#include "std"
#include "enumeration.hpp"

namespace std
{
	template <class T>
	// Core data type for a fixed-size generic collection.
	class Array : public Enumerable<T>
	{
	private:
		// The internal data of the array
		T*   arrayData;

	public:
		/* The number of items in the array. */
		mutable UInt length;

		// Creates a new array with the specified fixed capacity.
		explicit Array(UInt capacity);
		// Creates a new copy of the given array.
		Array(const Array<T>& copy);
		// Destroys the current array.
		~Array();

		/* Gets the item at the specified index. */
		T& getItem(UInt index) const noexcept;
		/* Sets the item at the specified index. */
		bool setItem(UInt index, const T& item) noexcept;

		/* Creates and returns an enumerator enabling iteration over the array. */
		Enumerator<T>* getEnumerator() const;

		/* Shallow-copies the members of the current array to a destination array. */
		void copyTo(Array<T>& destination) const;

		/* Returns the unsafe C backing-array. */
		inline const T* getCArray() const { return this->arrayData; }

		// Copy assignment
		Array<T>& operator =(const Array<T>& array);

		// Move assignment
		Array<T>& operator =(Array<T>&& array) noexcept;

	public:
		/* Gets the item at the specified index. No bounds check is performed. */
		inline T& operator[] (UInt index) const noexcept { return arrayData[index]; }

		/* Resizes the given array, trimming items if smaller. */
		static void resize(Array<T>* arr, UInt newSize);
	};

	template <class T>
	inline Array<T>::Array(UInt capacity) : arrayData(new T[capacity]), length(capacity) { }

	template <class T>
	Array<T>::Array(const Array<T>& copy) : arrayData(new T[copy.length]), length(copy.length)
	{
		for (UInt i = 0; i < copy.length; i++) {
			this->arrayData[i] = copy.arrayData[i];
		}
	}

	template <class T>
	inline Array<T>::~Array()
	{
		delete[] this->arrayData;
	}

	template <class T>
	inline T& Array<T>::getItem(UInt index) const noexcept
	{
		return this->arrayData[index];
	}

	template <class T>
	inline bool Array<T>::setItem(UInt index, const T& item) noexcept
	{
		if (index < this->length)
		{
			this->arrayData[index] = item;
			return true;
		}
		else return false;
	}

	template <class T>
	void Array<T>::copyTo(Array<T>& destination) const
	{
		UInt l = length > destination.length ? length : destination.length;
		for (UInt i = 0; i < l; i++)
		{
			destination.arrayData[i] = this->arrayData[i];
		}
	}

	template <class T>
	void Array<T>::resize(Array<T>* arr, UInt newSize)
	{
		Array<T> newArr(newSize);
		arr->copyTo(newArr);

		*arr = newArr;
	}

	template <class T>
	Array<T>& Array<T>::operator =(const Array<T>& array)
	{
		if (this->arrayData != array.arrayData)
		{
			T* newData = new T[array.length];

			this->length = array.length;

			delete this->arrayData;
			this->arrayData = newData;

			array.copyTo(*this);
		}
		return *this;
	}

	template <class T>
	Array<T>& Array<T>::operator =(Array<T>&& array)
	{
		this->arrayData = array.arrayData;
		this->length    = array.length;
		array.ptr       = nullptr;
		array.length    = 0;
		return *this;
	}


	template<class T>
	/* An explicit enumerator for array objects. */
	class ArrayEnumerator : public Enumerator<T>
	{
	private:
		UInt index;
		const Array<T>& arr;

	public:
		explicit ArrayEnumerator(const Array<T>& arr) noexcept : arr(arr), index(0) { }

		// Performs a single step of iteration. Returns false if no next item exists. 
		bool moveNext() noexcept {
			return (++this->index) <= (this->arr.length);
		}
		// Gets the current item.
		T& getCurrentItem() const
		{
			if (index == 0)              return arr.getItem(0);
			else if (index > arr.length) return arr.getItem(arr.length - 1);
			else                         return arr.getItem(index - 1);
		}

		~ArrayEnumerator() { }
	};

	template <class T>
	Enumerator<T>* Array<T>::getEnumerator() const
	{
		return new ArrayEnumerator<T>(*this);
	}
}
