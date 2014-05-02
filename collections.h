/* collections.h - (c) James S Renwick 2013
   ----------------------------------------
   Version 1.0.3
*/

#pragma once
#include <stdlib/array.h>
#include <stdlib/func.h>
#include <stdlib/smartptr.h>

namespace std
{

	template<class T>
	/* An interface allowing the iteration of the items in a collection.
	 Note that no exception need be thrown if the collection is modified. */
	class Enumerator
	{
	public:
		// Virtual destructor
		virtual ~Enumerator() noexcept{};

	public:
		// Performs a single step of iteration. Returns false if no next item exists. 
		virtual bool moveNext() = 0;
		// Gets the current item.
		virtual const T getCurrentItem() const = 0;
	};

	template<class T>
	class Enumerable
	{
	public:
		/* Creates and returns an enumerator enabling iteration over the list. */
		virtual Enumerator<T>* getEnumerator() const = 0;

		/* Iterates through each item, applying the given delegate. */
		template<class F>
		void forEach(Function<F, void, T> delegate) const
		{
			unique_ptr<Enumerator<T>> enumerator = this->getEnumerator();

			while (enumerator->moveNext()) {
				delegate(enumerator->getCurrentItem());
			}
		}

		/* Iterates through each item, applying the given delegate. */
		template<class F, class AT>
		AT aggregate(AT initalValue, Function<F, AT, AT, T> delegate) const
		{
			unique_ptr<Enumerator<T>> enumerator = this->getEnumerator();

			AT accumulator = initalValue;

			while (enumerator->moveNext()) {
				accumulator = delegate(enumerator->getCurrentItem(), accumulator);
			}
			return accumulator;
		}
	};

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
			return (++this->index) <= (this->arr.getLength());
		}
		// Gets the current item.
		const T getCurrentItem() const
		{
			     if (index == 0)              return arr.getItem(0);
			else if (index > arr.getLength()) return arr.getItem(arr.getLength() - 1);
			else                              return arr.getItem(index - 1);
		}

		~ArrayEnumerator() { }
	};

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
		virtual bool add(const T item) = 0;

		/* Removes the item at the given index from the list. 
		Returns true if the index is valid and the item removed. */
		virtual bool removeAt(UInt index) = 0;

		/* Returns the number of items in the list. */
		virtual UInt count() const = 0;

		/* Creates and returns an enumertor enabling iteration over the list. */
		//virtual Enumerator<T>* getEnumerator();

		/* Gets whether the list contains the given item. */
		virtual bool contains(const T item) const
		{
			unique_ptr<Enumerator<T>> enumerator = this->getEnumerator();

			while (enumerator->moveNext())
			{
				if (enumerator->getCurrentItem() == item) return true;
			}
			return false;
		}

		/* Returns the item at the given index. Behaviour undefined for invalid indices. */
		virtual const T itemAt(UInt index) const
		{
			unique_ptr<Enumerator<T>> enumerator = this->getEnumerator();

			while (enumerator->moveNext()) {
				if (index-- == 0) { return enumerator->getCurrentItem(); }
			}
		}

		/* Returns the index of the given item or -1 (UIntMax) if the item is not found. */
		virtual UInt indexOf(const T item) const
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

		/* Gets the item at the specified index. */
		inline const T operator [](UInt index) const {
			return this->itemAt(index);
		}

		/* Removes the first occurance of the item from the list. Returns true if the item
		is found and removed. */
		virtual bool remove(const T item)
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

		template<typename F>
		/* Performs a shallow copy of those items selected by the given delegate,
		adding each item to the destination list. */
		void filteredCopy(IList<T>& newList, Function<F, bool, T> delegate) const
		{
			unique_ptr<Enumerator<T>> enumerator = this->getEnumerator();

			while (enumerator->moveNext())
			{
				T item = enumerator->getCurrentItem();
				if (delegate(item)) { newList.add(item); }
			}
		}

	};

}

#include <stdlib/collections/list.h>
