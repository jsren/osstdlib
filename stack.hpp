/* stack.hpp - (c) James S Renwick 2014
   ------------------------------------
   Version 1.1.0
*/
#pragma once
#include "std"
#include "list.hpp"

namespace std
{
	template <class T>
	class Stack : public List<T>
	{
	public:
		inline bool push(const T item) noexcept {
			return this->add(item);
		}

		inline T pop() noexcept
		{
			T item = this->itemAt(this->length - 1);
			this->removeAt(this->length - 1);

			return item;
		}

		inline T peek() noexcept
		{
			return this->itemAt(this->length - 1);
		}
	};
}