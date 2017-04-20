/* array.hpp - (c) James S Renwick 2013
   ------------------------------------
   Authors: James S Renwick
*/
#pragma once

#include "_std.hpp"
#include "enumeration.hpp"
#include "memory.hpp"


namespace std
{
    template<typename Y>
    class ArrayEnumerator;

	// Core data type for a fixed-size generic collection.
	template<class T>
	class Array final : public Enumerable<T>
	{
        friend ArrayEnumerator<T>;
        friend ArrayEnumerator<const T>;

	public:
		// The type of the array elements
		using ElementType = T;

	private:
		// The internal data of the array
        unique_ptr<T[]> arrayData{};

		/* The number of items in the array. */
		size_t _length = 0;

	public:

		// Creates a new array with the specified fixed capacity.
		explicit Array(size_t capacity);
		// Creates a new copy of the given array.
		Array(const Array<T>& copy);


		/* Gets the item at the specified index. Returns true when index is valid. */
		bool getItem(size_t index, T& out) const noexcept;
		/* Sets the item at the specified index. Returns true when index is valid. */
		bool setItem(size_t index, const T& item) noexcept;


        /* Gets an enumerator enabling iteration over the array. */
        Enumerator<T> getEnumerator() override final;
		/* Gets an enumerator enabling iteration over the array. */
		Enumerator<const T> getEnumerator() const override final;

		/* Shallow-copies the members of the current array to a destination array. */
		void copyTo(Array<T>& destination) const;

		/* Returns the unsafe C backing-array. */
		inline const T* getCArray() const { return this->arrayData; }

        /* Gets the number of elements in the array. */
        inline size_t length() const { return this->_length;  }

		// Copy assignment
		Array<T>& operator =(const Array<T>& array);

		// Move assignment
		Array<T>& operator =(Array<T>&& array) noexcept;

	public:
		/* Gets the item at the specified index. No bounds check is performed. */
        inline T& operator[] (size_t index) noexcept { return arrayData[index]; }

        /* Gets the item at the specified index. No bounds check is performed. */
		inline const T& operator[] (size_t index) const noexcept { return arrayData[index]; }

		/* Resizes the given array, trimming items if smaller. */
		static void resize(Array<T>* arr, size_t newSize);

	};

	template <class T>
	Array<T>::Array(size_t capacity) : _length(capacity)
	{
		if (capacity != 0) this->arrayData = new T[capacity];
	}

	template <class T>
	Array<T>::Array(const Array<T>& copy) : _length(copy._length)
	{
		if (copy.arrayData != nullptr)
		{
			this->arrayData = new T[copy._length];

			for (size_t i = 0; i < copy._length; i++) {
				this->arrayData[i] = copy.arrayData[i];
			}
		}
		else this->_length = 0;
	}

	template <class T>
	inline bool Array<T>::getItem(size_t index, T& out) const noexcept
	{
		if (index < this->_length)
		{
			out = this->arrayData[index];
			return true;
		}
		else return false;
	}

	template <class T>
	inline bool Array<T>::setItem(size_t index, const T& item) noexcept
	{
		if (index < this->_length)
		{
			this->arrayData[index] = item;
			return true;
		}
		else return false;
	}

	template <class T>
	void Array<T>::copyTo(Array<T>& destination) const
	{
        size_t l = this->_length < destination._length ? this->_length : destination._length;
		for (size_t i = 0; i < l; i++)
		{
			destination.arrayData[i] = this->arrayData[i];
		}
	}

	template <class T>
	void Array<T>::resize(Array<T>* arr, size_t newSize)
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
			T* newData = new T[array._length];

			this->_length = array._length;

			delete this->arrayData;
			this->arrayData = newData;

			array.copyTo(*this);
		}
		return *this;
	}

	template <class T>
	Array<T>& Array<T>::operator =(Array<T>&& array) noexcept
	{
		this->arrayData = array.arrayData;
		this->_length    = array._length;
		array.arrayData = nullptr;
		array._length    = 0;
		return *this;
	}


	/* An explicit enumerator for array objects. */
	template<class T>
	class ArrayEnumerator final : public EnumeratorBase<T>
	{
	private:
        T* data = nullptr;
		size_t index = 0;
        size_t length = 0;

	public:
		explicit ArrayEnumerator(const Array<std::remove_const<T>>& arr) noexcept
            : data(const_cast<T*>(arr.arrayData.get())), length(arr._length) { }

		// Performs a single step of iteration. Returns false if no next item exists. 
		bool next() noexcept override final {
			return (++this->index) <= (this->length);
		}
		// Gets the current item.
		T& current() override final
		{
			if (index == 0)          return data[0];
            else if (index > length) return data[length -1];
			else                     return data[index - 1];
		}
	};

    template <class T>
    Enumerator<T> Array<T>::getEnumerator() {
        return new ArrayEnumerator<T>(*this);
    }

	template <class T>
	Enumerator<const T> Array<T>::getEnumerator() const {
		return new ArrayEnumerator<const T>(*this);
	}
}
