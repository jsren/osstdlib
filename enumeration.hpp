/* enumeration.hpp - (c) James S Renwick 2014
   ------------------------------------------
   Version 1.1.1
*/
#pragma once

#include "_std.hpp"
#include "meta.hpp"
#include "smartptr.hpp"


namespace std
{
	/* An interface allowing the iteration of the items in a collection. */
	template<class T>
	class EnumeratorBase
	{
	public:
		using ElemType = T;

	public:
		// Virtual destructor
		virtual ~EnumeratorBase() noexcept = default;

	public:
		// Performs a single step of iteration. Returns false if no next item exists. 
		virtual bool next() = 0;

        // Gets the current item.
        virtual T& current() = 0;
	};



	template <class T>
	class Enumerator
	{
	public:
		using ElemType = T;
	private:
		unique_ptr<EnumeratorBase<T>> pimpl{};

	public:
		inline Enumerator(EnumeratorBase<T>* pimpl) 
			: pimpl(pimpl) { }
		inline Enumerator(unique_ptr<EnumeratorBase<T>> pimpl) 
			: pimpl(std::move(pimpl)) { }

	public:
		// Performs a single step of iteration. Returns false if no next item exists. 
		inline bool next() _noexc_copy_from(pimpl->next()) {
			return pimpl->next();
		}
		// Gets the current item.
		T& current() const _noexc_copy_from(pimpl->current()) {
			return pimpl->current();
		}
	};



	template<class T>
	class Enumerable
	{
	public:
        __DefaultCtorCopyMove(Enumerable);

		// Virtual destructor
		virtual ~Enumerable() noexcept = default;

		/* Creates and returns an enumerator enabling iteration over the collection. */
		virtual Enumerator<T> getEnumerator() = 0;

		/* Creates and returns an enumerator enabling iteration over the collection. */
		virtual Enumerator<const T> getEnumerator() const = 0;

		/* Iterates through each item, applying the given delegate. */
		template<class Delegate>
		void forEach(Delegate& delegate) const
		{
			auto enumerator = this->getEnumerator();
			while (enumerator->next()) {
				delegate(enumerator->current());
			}
		}

		/* Iterates through each item, applying the given delegate. */
		template<class Aggregator, class AT>
		AT aggregate(Aggregator& aggregator, AT initalValue) const
		{
			AT accumulator = initalValue;
			auto enumerator = this->getEnumerator();

			while (enumerator->next()) {
				accumulator = delegate(enumerator->current(), accumulator);
			}
			return accumulator;
		}
	};
}
