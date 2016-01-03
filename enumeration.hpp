/* enumeration.hpp - (c) James S Renwick 2016
   ------------------------------------------
   Version 1.2.0
*/
#pragma once

#include "std"
#include "func.hpp"

namespace std
{

#ifndef NO_FOREACH
#define foreach(_vardecl, _ptr_enumerable) {\
    auto __enum = (_ptr_enumerable)->getEnumerator();\
    while (__enum->hasNext()) {\
        _vardecl = __enum->nextItem();

#define end_foreach } delete __enum; }
#endif


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

        // Gets the next item and advances the enumerator.
        virtual const T &nextItem() const {
             return const_cast<Enumerator<T>*>(this)->nextItem();
        }
	};


	template<class T>
	class Enumerable
	{
	public:
        virtual ~Enumerable() { };

		/* Creates and returns an enumerator enabling iteration over the collection. */
        virtual Enumerator<T> *getEnumerator() = 0;
        virtual const Enumerator<T> *getEnumerator() const = 0;

        virtual bool any() const
        {
            auto enumerator = unique_ptr_from(this->getEnumerator());
            return enumerator->hasNext();
        }

        template<class F>
        bool any(Function<F, bool, T> delegate) const
        {
            auto enumerator = unique_ptr_from(this->getEnumerator());

            while (enumerator->hasNext()) {
                if (delegate(enumerator->nextItem())) return true;
            }
            return false;
        }

		/* Iterates through each item, applying the given delegate. */
		template<class F>
		void forEach(Function<F, void, T> delegate) const
		{
            auto enumerator = unique_ptr_from(this->getEnumerator());

			while (enumerator->hasNext()) {
				delegate(enumerator->nextItem());
			}
		}

		/* Iterates through each item, applying the given delegate. */
		template<class F, class AT>
		AT aggregate(AT initalValue, Function<F, AT, AT, T> delegate) const
		{
            auto enumerator  = unique_ptr_from(this->getEnumerator());
			AT   accumulator = initalValue;

			while (enumerator->hasNext()) {
				accumulator = delegate(accumulator, enumerator->nextItem());
			}
			return accumulator;
		}

        Enumerable<T> *union_(const Enumerable<T> &enumerable) const
        {
            auto enum1 = unique_ptr_from(this->getEnumerator());
            auto *list = new List<T>{};

            while (enum1->hasNext())
            {
                bool add   = false;
                auto item1 = enum1->nextItem();
                auto enum2 = unique_ptr_from(enumerable.getEnumerator());
                
                while (enum2->hasNext())
                {
                    if (item1 == enum2->nextItem()) {
                        add = true; break;
                    }
                }
                if (add && !list->contains(item1)) {
                    list->add(item1);
                }
            }
            return list;
        }

        Enumerable<T> *intersection(const Enumerable<T> &enumerable) const
        {
            auto enumerator = unique_ptr(this->getEnumerator());
            auto *list = new List<T>{};

            while (enumerator->hasNext())
            {
                auto &item = enumerator->nextItem();
                if (!list->contains(item)) list->add(item);
            }

            delete enumerator;
            enumerator = enumerable.getEnumerator();

            while (enumerator->hasNext())
            {
                auto &item = enumerator->nextItem();
                if (!list->contains(item)) list->add(item);
            }
            return list;
        }
	};
}