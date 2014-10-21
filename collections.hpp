/* collections.hpp - (c) James S Renwick 2013
   ------------------------------------------
   Version 1.2.2
*/

#pragma once
#include "array.hpp"
#include "func.hpp"
#include "smartptr.hpp"
#include "enumeration.hpp"

namespace std
{
	template<class T>
	/* Abstract base class for all list objects. */
	class IList : public Enumerable<T>
	{
	public:
		// Virtual destructor
		virtual ~IList() {};

	public:
		/* Adds the given item to the list. 
		Returns true if the item was successfully added. */
		virtual bool add(const T& item) = 0;

		/* Removes the item at the given index from the list. 
		Returns true if the index is valid and the item removed. */
		virtual bool removeAt(UInt index) = 0;

		/* Inserts the item into the list at the given index.
		Returns true if the index is valid and the item added. */
		virtual bool insert(UInt index, const T& item) = 0;

		/* Returns the number of items in the list. */
		virtual UInt count() const = 0;

		/* Creates and returns an enumertor enabling iteration over the list. */
		//virtual Enumerator<T>* getEnumerator();

		/* Gets whether the list contains the given item. */
		virtual bool contains(const T& item) const
		{
			unique_ptr<Enumerator<T>> enumerator = this->getEnumerator();

			while (enumerator->moveNext())
			{
				if (enumerator->getCurrentItem() == item) return true;
			}
			return false;
		}

		/* Returns the item at the given index. Behaviour undefined for invalid indices. */
		virtual T& itemAt(UInt index) const
		{
			unique_ptr<Enumerator<T>> enumerator = this->getEnumerator();

			while (enumerator->moveNext()) {
				if (index-- == 0) { return enumerator->getCurrentItem(); }
			}
			// This is just here to silence compiler warnings.
			return enumerator->getCurrentItem();
		}

		/* Returns the index of the given item or -1 (UIntMax) if the item is not found. */
		virtual UInt indexOf(const T& item) const
		{
			unique_ptr<Enumerator<T>> enumerator = this->getEnumerator();

			UInt index = 0;
			while (enumerator->moveNext() && index != UIntMax)
			{
				if (enumerator->getCurrentItem() == item) { break; }
				else index++;
			}
			return index;
		}

		/* Gets or sets the item at the specified index. */
		inline T& operator [](UInt index) const {
			return this->itemAt(index);
		}

		/* Removes the first occurance of the item from the list. Returns true if the item
		is found and removed. */
		virtual bool remove(const T& item)
		{
			UInt index = this->indexOf(item);
			if (index == UIntMax) {
				return false;
			}
			else return this->removeAt(index);
		}

		/* Performs a shallow copy, adding each item in this list to the destination list. */
		virtual void copyTo(IList<T>& destination) const
		{
			unique_ptr<Enumerator<T>> enumerator = this->getEnumerator();

			while (enumerator->moveNext()) {
				destination.add(enumerator->getCurrentItem());
			}
		}

		/* Performs a shallow copy, adding each item to a new array and returning the result. */
		virtual Array<T> toArray() const
		{
			unique_ptr<Enumerator<T>> enumerator = this->getEnumerator();

			Array<T> output(this->count());

			UInt i = 0;
			while (enumerator->moveNext()) {
				output.setItem(i++, enumerator->getCurrentItem());
			}
			return output;
		}

		template<typename F>
		/* Performs a shallow copy of those items selected by the given delegate,
		adding each item to the destination list. */
		void filteredCopy(IList<T>& newList, Function<F, bool, T> delegate) const
		{
			unique_ptr<Enumerator<T>> enumerator = this->getEnumerator();

			while (enumerator->moveNext())
			{
				T& item = enumerator->getCurrentItem();
				if (delegate(item)) { newList.add(item); }
			}
		}

		/* Reverses the elements of the current list in place. */
		void reverse()
		{
			UInt count = this->count();
			for (UInt i = 0, j = count - 1; i < count / 2; i++, j--)
			{
				if (i == j) break;

				T tmp = this->itemAt(i);
				this->itemAt(i) = this->itemAt(j);
				this->itemAt(j) = tmp;
			}
		}

	};

}
