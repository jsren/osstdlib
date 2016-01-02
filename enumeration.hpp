/* enumeration.hpp - (c) James S Renwick 2016
   ------------------------------------------
   Version 1.2.0
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
        // Returns whether a next item is available.
        virtual bool hasNext() const = 0;
        // Gets the next item and advances the enumerator.
        virtual T& nextItem() = 0;
	};

	template<class T>
	class Enumerable
	{
	public:
        virtual ~Enumerable() { };

		/* Creates and returns an enumerator enabling iteration over the collection. */
		virtual Enumerator<T> *getEnumerator() const = 0;

        virtual bool any() const
        {
            unique_ptr<Enumerator<T>> enumerator = this->getEnumerator();
            return enumerator->hasNext();
        }

        template<class F>
        bool any(Function<F, bool, T> delegate) const
        {
            unique_ptr<Enumerator<T>> enumerator = this.getEnumerator();

            T &item;
            while (enumerator->hasNext()) 
            {
                item = enumerator->nextItem();
                if (delegate(item)) return true;
            }
            return false;
        }

		/* Iterates through each item, applying the given delegate. */
		template<class F>
		void forEach(Function<F, void, T> delegate) const
		{
            unique_ptr<Enumerator<T>> enumerator = this->getEnumerator();

            T item;
			while (enumerator->next(item)) {
				delegate(item);
			}
		}

		/* Iterates through each item, applying the given delegate. */
		template<class F, class AT>
		AT aggregate(AT initalValue, Function<F, AT, AT, T> delegate) const
		{
            unique_ptr<Enumerator<T>> enumerator = this->getEnumerator();

			AT accumulator = initalValue;

            T item;
			while (enumerator->next(item)) {
				accumulator = delegate(accumulator, item);
			}
			return accumulator;
		}

        Enumerable<T> *union_(const Enumerable<T> &enumerable) const
        {
            unique_ptr<Enumerator<T>> enum1 = this->getEnumerator();
            IList<T> *list = new List<T>{};

            T item1;
            while (enum1->next(item))
            {
                bool add   = false;
                unique_ptr<Enumerator<T>> enum2 = enumerable.getEnumerator();
                
                T item2;
                while (enum2->next(item))
                {
                    if (item1 == item2) {
                        add = true; break;
                    }
                }
                if (add && !list->contains(item1)) {
                    list->add(item);
                }
            }
            return list;
        }

        Enumerable<T> *intersection(const Enumerable<T> &enumerable) const
        {
            unique_ptr<Enumerator<T>> enumerator = this->getEnumerator();
            IList<T> *list = new List<T>{};

            T item; 
            while (enumerator->next(item))
            {
                if (!list->contains(item)) list->add(item);
            }

            delete enumerator;
            enumerator = enumerable.getEnumerator();

            while (enumerator->next(item))
            {
                if (!list->contains(item)) list->add(item);
            }
            return list;
        }
	};
}