/* list.hpp - (c) James S Renwick 2013
   -----------------------------------
   Version 1.2.0
*/
#pragma once
#include "std"
#include "collections.hpp"

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
		bool add(const T& item) override
		{
			return this->insert(this->length, item);
		}

		/* Removes the item at the given index from the list. 
		Returns true if the index is valid and the item removed. */
		bool removeAt(UInt index) override
		{
			if (index >= this->length) return false;

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

		/* Inserts the item into the list at the given index.
		Returns true if the index is valid and the item added. */
		bool insert(UInt index, const T& item)
		{
			if (index > this->length) return false;

			// Check if we should resize
			if (this->length == UIntMax - 1) {
				return false;
			}
			else if (this->length == this->realLength) {
				this->__resize(this->length + 1);
			}

			// Move existing elements to make space for insert
			for (UInt i = this->length; i > index; i--) {
				this->data[i] = this->data[i - 1];
			}
			this->data[index] = item;
			this->length++;
			
			return true;
		}

		/* Returns the item at the given index. Behaviour undefined for invalid indices. */
		T& itemAt(UInt index) const noexcept {
			return this->data[index];
		}
		inline T& operator [](UInt index) const {
			return this->data[index];
		}

		/* Returns the index of the given item or -1 (UIntMax) if the item is not found. */
		UInt indexOf(const T& item) const noexcept
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

            // Returns whether a next item is available.
            bool hasNext() const override {
                return this->index < this->list.count();
            }

			// Gets the next item and advances the enumerator.
			T &nextItem() override {
                return this->list.itemAt(this->index++);
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
			this->realLength = length + (extra > maxListExtra ? maxListExtra : extra);

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
