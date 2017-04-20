/* list.hpp - (c) James S Renwick 2013
   -----------------------------------
   Authors: James S Renwick
*/
#pragma once
#include "_std.hpp"
#include "smartptr.hpp"
#include "collections.hpp"


namespace std
{
	static const size_t __maxListExtra = 120;
    static const size_t __list_initialLength = 10;

	template<class T>
	class List : public IList<T>
	{
	private:
		size_t length = 0;
		size_t reservedLength = 0;
        unique_ptr<T[]> data{};

	public:
        __DefaultVirtualDtor(List);

		explicit List() : reservedLength(__list_initialLength),
            data(new T[reservedLength]) { }

		explicit List(size_t initialCapacity) 
			: reservedLength(initialCapacity)
		{
			if (initialCapacity != 0) {
				this->data = new T[this->reservedLength];
			}
		}


	public:
        template<typename Y>
        inline bool emplaceAdd(size_t index, Y&& item) {
            return this->emplaceInsert(index, std::forward<Y>(item));
        }

		/* Removes the item at the given index from the list. 
		Returns true if the index is valid and the item removed. */
		bool removeAt(size_t index) override
		{
			if (index >= this->length) return false;

			for (size_t i = index + 1; i < this->length; i++) {
				this->data[i - 1] = this->data[i];
			}
			this->length--;

			// Check if we should resize
			if (this->reservedLength > __maxListExtra) {
				this->__resize(this->length);
			}
			return true;
		}

		/* Inserts the item into the list at the given index.
		Returns true if the index is valid and the item added. */
		bool insert(size_t index, const T& item)
		{
			if (index > this->length) return false;

			// Check if we should resize
			if (this->length == size_tMax - 1) {
				return false;
			}
			else if (this->length == this->reservedLength) {
				this->__resize(this->length + 1);
			}

			// Move existing elements to make space for insert
			for (size_t i = this->length; i > index; i--) {
				this->data[i] = this->data[i - 1];
			}
			this->data[index] = item;
			this->length++;
			
			return true;
		}

		/* Returns the item at the given index. Behaviour undefined for invalid indices. */
		T& itemAt(size_t index) const noexcept {
			return this->data[index];
		}
		inline T& operator [](size_t index) const {
			return this->data[index];
		}

		/* Returns the index of the given item or -1 (size_tMax) if the item is not found. */
		size_t indexOf(const T& item) const noexcept
		{
			for (size_t i = 0; i < this->length; i++) {
				if (this->data[i] == item) return i;
			}
			return size_tMax;
		}

		/* Returns the number of items in the list. */
		size_t count() const noexcept { return length; }

	private:

		/* =========================================
			The Enumerator for the List collection.
			=========================================*/
		class ListEnum final : public EnumeratorBase<T>
		{
		private:
			size_t index = 0;
			const List<T>& list;
		public:
			explicit ListEnum(const List<T>& list) noexcept 
				: list(list) { };

			// Performs a single step of iteration. Returns false if no next item exists. 
			bool next() noexcept override final {
				return (++this->index) <= (this->list.count());
			}
			// Gets the current item.
			T& current() override final
			{
				     if (index == 0)           return list.itemAt(0);
				else if (index > list.count()) return list.itemAt(list.count() - 1);
				else                           return list.itemAt(index - 1);
			}
            // Gets the current item.
            const T& current() const override final
            {
                if (index == 0)           return list.itemAt(0);
                else if (index > list.count()) return list.itemAt(list.count() - 1);
                else                           return list.itemAt(index - 1);
            }
		};

	public:
		/* Creates and returns an enumertor enabling iteration over the list. */
		Enumerator<T> getEnumerator() override final {
			return new ListEnum(*this);
		}
        Enumerator<const T> getEnumerator() const override final {
            return new ListEnum(*this);
        }

	private:
		void __resize(size_t length)
		{
			size_t extra = length >> 1;
			this->reservedLength = length + (extra > __maxListExtra ? __maxListExtra : extra);

			// Account for integer overflow
			if (this->reservedLength < length) { this->reservedLength = length; }

			T* tmpData = this->data;

			this->data = new T[this->reservedLength];

			// Copy old data if present
			if (this->data != nullptr)
			{
				for (size_t i = 0; i < this->length; i++) {
					this->data[i] = tmpData[i];
				}
				delete[] tmpData;
			}

			// The max length of a list is actually size_tMax-1
			if (this->length == size_tMax) this->length = size_tMax - 1;
		}

	};
}
