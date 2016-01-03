/* array.hpp - (c) James S Renwick 2016
   ------------------------------------
   Version 1.1.5
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
		T* arrayData;
        mutable UInt length;

	public:
		// Creates a new array with the specified fixed capacity.
		explicit inline Array(UInt capacity) : 
            arrayData(new T[capacity]), length(capacity) { };

		// Creates a new copy of the given array.
		Array(const Array<T>& copy);

		// Destroys the current array.
		inline ~Array() {
            delete[] this->arrayData;
        };

        /* The number of items in the array. */
        inline UInt count() const noexcept {
            return this->length;
        }

        /* Gets the item at the specified index. */
        inline T& getItem(UInt index) noexcept {
            return this->arrayData[index];
        }
		/* Gets the item at the specified index. */
        inline const T& getItem(UInt index) const noexcept {
            return this->arrayData[index];
        }
		/* Sets the item at the specified index. */
		bool setItem(UInt index, const T& item) noexcept;

		/* Creates and returns an enumerator enabling iteration over the array. */
		Enumerator<T>* getEnumerator() override;
        const Enumerator<T>* getEnumerator() const override;

        bool any() const override;

		/* Shallow-copies the members of the current array to a destination array. */
		UInt copyTo(Array<T>& destination) const;
        /* Shallow-copies the members of the current array to a destination array. */
        UInt copyTo(T target[], UInt targetCount) const;

		/* Returns the unsafe C backing-array. */
		inline const T* getCArray() const { return this->arrayData; }

		// Copy assignment
		Array<T>& operator =(const Array<T>& array);

		// Move assignment
		Array<T>& operator =(Array<T>&& array) noexcept;

	public:
		/* Gets the item at the specified index. No bounds check is performed. */
		inline T& operator[] (UInt index) noexcept { return arrayData[index]; }
        inline const T& operator[] (UInt index) const noexcept { return arrayData[index]; }

    public:
		/* Resizes the given array, trimming items if smaller. */
		static void resize(Array<T>* arr, UInt newSize);
	};
}

#include "array.cpp"
