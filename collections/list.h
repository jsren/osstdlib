/* list.h - (c) James S Renwick 2013
   ---------------------------------
   Version 1.0.6
*/
#pragma once
#include "../std.h"
#include <stdlib/collections.h>

namespace std
{
	template<class T>
	class List : public IList<T>
	{
	private:
		T*   data;
		UInt length;
		UInt realLength;

		void __resize(UInt length);

	public:
		List() : length(0), realLength(10) {
			this->data = new T[this->realLength];
		}

		explicit List(UInt initialCapacity) : length(0), realLength(initialCapacity) {
			this->data = new T[this->realLength];
		}

		// Virtual destructor
		virtual ~List() { delete[] this->data; }

	public:
		/* Adds the given item to the list. 
		Returns true if the item was successfully added. */
		bool add(const T item);

		/* Removes the item at the given index from the list. 
		Returns true if the index is valid and the item removed. */
		bool removeAt(UInt index);

		/* Returns the item at the given index. Behaviour undefined for invalid indices. */
		const T itemAt(UInt index) const noexcept;

		/* Returns the index of the given item or -1 (UIntMax) if the item is not found. */
		UInt indexOf(const T item) const noexcept;

		/* Returns the number of items in the list. */
		UInt count() const noexcept{ return length; }

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
			explicit ListEnum(const List<T>& list) noexcept : index(0), list(list) {};

			// Performs a single step of iteration. Returns false if no next item exists. 
			bool moveNext() noexcept{
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
		Enumerator<T>* getEnumerator() const {
			return new ListEnum(*this);
		}
	};
}
