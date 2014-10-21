/* enumeration.hpp - (c) James S Renwick 2014
   ------------------------------------------
   Version 1.1.1
*/
#pragma once

#include "std"
#include "func.hpp"

namespace std
{
	template<class T>
	/* An interface allowing the iteration of the items in a collection.
	Note that no exception need be thrown if the collection is modified. */
	class Enumerator
	{
	public:
		// Virtual destructor
		virtual ~Enumerator() noexcept { };

	public:
		// Performs a single step of iteration. Returns false if no next item exists. 
		virtual bool moveNext() = 0;
		// Gets the current item.
		virtual T& getCurrentItem() const = 0;
	};

	template<class T>
	class Enumerable
	{
	public:
		// Virtual destructor
		virtual ~Enumerable() = 0;

		/* Creates and returns an enumerator enabling iteration over the collection. */
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
}