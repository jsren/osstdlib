/* list.h - (c) James S Renwick 2013
   ---------------------------------
   Version 1.1.2
*/
#pragma once
#include "../std.h"
#include <stdlib/collections.h>

namespace std
{
	static const UInt maxListExtra = 120;

	template<class T>
	class List : public IList<T>
	{
	private:
		T*   data;
		UInt length;
		UInt realLength;

	public:
		List() : IList<T>(), length(0), realLength(10) {
			this->data = new T[this->realLength];
		}

		explicit List(UInt initialCapacity) 
			: IList<T>(), length(0), realLength(initialCapacity)
		{
			this->data = new T[this->realLength];
		}

		// Virtual destructor
		virtual ~List() { delete[] this->data; }

	public:
		/* Adds the given item to the list. 
		Returns true if the item was successfully added. */
		bool add(const T item) override
		{
			if (this->length == UIntMax - 1) {
				return false;
			}
			else if (this->length == this->realLength) {
				this->__resize(this->length + 1);
			}

			this->data[this->length++] = item;
			return true;
		}

		/* Removes the item at the given index from the list. 
		Returns true if the index is valid and the item removed. */
		bool removeAt(UInt index) override
		{
			if (index >= this->length) { return false; }

			for (UInt i = index + 1; i < this->length; i++) {
				this->data[i - 1] = this->data[i];
			}
			this->length--;

			// Check if we should resize
			if (this->realLength > maxListExtra) {
				this->__resize(this->length);
			}
			return true;
		}

		/* Returns the item at the given index. Behaviour undefined for invalid indices. */
		const T itemAt(UInt index) const noexcept {
			return this->data[index];
		}
		inline const T operator [](UInt index) const {
			return this->data[index];
		}

		/* Returns the index of the given item or -1 (UIntMax) if the item is not found. */
		UInt indexOf(const T item) const noexcept
		{
			for (UInt i = 0; i < this->length; i++) {
				if (this->data[i] == item) return i;
			}
			return UIntMax;
		}

		/* Returns the number of items in the list. */
		UInt count() const noexcept { return length; }

	private:

		/* =========================================
			The Enumerator for the List collection.
			=========================================*/
		class ListEnum : public Enumerator<T>
		{
		private:
			UInt index;
			const List<T>& list;
		public:
			explicit ListEnum(const List<T>& list) noexcept 
				: Enumerator<T>(), index(0), list(list) {};

			// Performs a single step of iteration. Returns false if no next item exists. 
			bool moveNext() noexcept {
				return (++this->index) <= (this->list.count());
			}
			// Gets the current item.
			const T getCurrentItem() const
			{
				     if (index == 0)           return list.itemAt(0);
				else if (index > list.count()) return list.itemAt(list.count() - 1);
				else                           return list.itemAt(index - 1);
			}
		};

	public:
		/* Creates and returns an enumertor enabling iteration over the list. */
		Enumerator<T>* getEnumerator() const override {
			return new ListEnum(*this);
		}

	private:
		void __resize(UInt length)
		{
			UInt extra = length >> 1;
			this->realLength = length + std::min(extra, maxListExtra);

			// Account for integer overflow
			if (this->realLength < length) { this->realLength = length; }

			T* tmpData = this->data;

			this->data = new T[this->realLength];

			// Copy old data if present
			if (this->data != nullptr)
			{
				for (UInt i = 0; i < this->length; i++) {
					this->data[i] = tmpData[i];
				}
				delete[] tmpData;
			}

			// The max length of a list is actually UIntMax-1
			if (this->length == UIntMax) this->length = UIntMax - 1;
		}

	};
}
