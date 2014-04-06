/* list.cpp - (c) James S Renwick 2013
   -----------------------------------
   Version 1.0.4
*/
#include <stdlib/math.h>
#include <stdlib/collections/list.h>

namespace std
{
	// Holds the maximum amount by which the list
	// will be over-allocated
	const UInt maxListExtra = 120;

	template<class T>
	void List<T>::__resize(UInt length)
	{
		UInt extra = length >> 1;
		this->realLength = length + std::min(extra, maxListExtra);

		// Account for integer overflow
		if (this->realLength < length) { this->realLength = length; }

		T* tmpData = this->data;

		this->data = new T[this->realLength];

		// Copy old data if present
		if (this->data != nullptr)
		{
			for (UInt i = 0; i < this->length; i++) {
				this->data[i] = tmpData[i];
			}
			delete[] tmpData;
		}

		// The max length of a list is actually UIntMax-1
		if (this->length == UIntMax) this->length = UIntMax - 1;
	}

	template<class T>
	bool List<T>::add(const T item)
	{
		if (this->length == UIntMax - 1) {
			return false;
		}
		else if (this->length == this->realLength) {
			this->__resize(this->length + 1);
		}
		
		this->data[this->length++] = item;
		return true;
	}

	template<class T>
	bool List<T>::removeAt(UInt index)
	{
		if (index >= this->length) { return false; }

		for (UInt i = index + 1; i < this->length; i++) {
			this->data[i - 1] = this->data[i];
		}
		this->length--;

		// Check if we should resize
		if (this->realLength > maxListExtra) {
			this->__resize(this->length);
		}
		return true;
	}

	template<class T>
	const T List<T>::itemAt(UInt index) const noexcept
	{
		return this->data[index];
	}

	template<class T>
	UInt List<T>::indexOf(const T item) const noexcept
	{
		for (UInt i = 0; i < this->length; i++) {
			if (this->data[i] == item) return i;
		}
		return UIntMax;
	}
}
