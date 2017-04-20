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
    class ICollection : public Enumerable<T>
    {
    public:
        __DefaultVirtualDtor(IList);

    public:
        /* Returns the number of items in the list. */
        virtual size_t count() const = 0;

        /* Gets whether the list contains the given item. */
        virtual bool contains(const T& item) const
        {
            auto enumerator = this->getEnumerator();

            while (enumerator.next()) {
                if (enumerator.current() == item) return true;
            }
            return false;
        }

        /* Returns the item at the given index. Behaviour undefined for invalid indices. */
        virtual T& itemAt(size_t index) const
        {
            unique_ptr<Enumerator<T>> enumerator = this->getEnumerator();

            while (enumerator->moveNext()) {
                if (index-- == 0) { return enumerator->getCurrentItem(); }
            }
            // This is just here to silence compiler warnings.
            return enumerator->getCurrentItem();
        }
    };


    constexpr const size_t npos = static_cast<size_t>(-1);

    /* Returns the index of the given item or -1 (size_tMax) if the item is not found. */
    template<typename T>
    size_t indexOf(Enumerator<T> enum_, const T& item) const
    {
        bool found; size_t index = 0;
        while (enum_.next() && (found = (enum_.current() != item))) {
            index++;
        }
        return found ? index : std::npos;
    }
    template<typename Enumerable>
    inline size_t indexOf(Enumerable enumerable, size_t index) const {
        return indexOf(enumerable.getEnumerator(), index);
    }
    template<typename T>
    inline size_t indexOf(const Enumerable<T>& enumerable, const T& item) const {
        return indexOf(enumerable.getEnumerator(), item);
    }

    /* Returns the item at the given index. Behaviour undefined for invalid indices. */
    template<typename T>
    T& itemAt(Enumerator<T>& enum_, size_t index) const
    {
        while (enum_->moveNext()) {
            if (index-- == 0) { return enum_->current(); }
        }
        // This is just here to silence compiler warnings.
        return enum_->current();
    }
    template<typename Enumerable>
    inline auto itemAt(Enumerable enumerable, size_t index) const {
        return itemAt(enumerable.getEnumerator(), index);
    }

	template<class T>
	/* Abstract base class for all list objects. */
	class IList : public ICollection<T>
	{
	public:
        __DefaultVirtualDtor(IList);

	public:
		/* Adds the given item to the list. 
		Returns true if the item was successfully added. */
        virtual bool add(T item) override {
            return this->insert(this->length, item);
        }

		/* Removes the item at the given index from the list. 
		Returns true if the index is valid and the item removed. */
		virtual bool removeAt(size_t index) = 0;

		/* Inserts the item into the list at the given index.
		Returns true if the index is valid and the item added. */
		virtual bool insert(size_t index, T item) = 0;


		/* Removes the first occurance of the item from the list. Returns true if the item
		is found and removed. */
		virtual bool remove(const T& item)
		{
			size_t index = this->indexOf(item);
			if (index == size_tMax) {
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

			size_t i = 0;
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
			size_t count = this->count();
			for (size_t i = 0, j = count - 1; i < count / 2; i++, j--)
			{
				if (i == j) break;

				T tmp = this->itemAt(i);
				this->itemAt(i) = this->itemAt(j);
				this->itemAt(j) = tmp;
			}
		}

	};

}
