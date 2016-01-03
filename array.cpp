#include "array.hpp"

namespace std
{
    	template <class T>
	Array<T>::Array(const Array<T>& copy) : 
        arrayData(new T[copy.length]), length(copy.length)
	{
		for (UInt i = 0; i < copy.length; i++) {
			this->arrayData[i] = copy.arrayData[i];
		}
	}

    template <class T>
    bool Array<T>::any() const {
        return this->length != 0;
    }

	template <class T>
	inline bool Array<T>::setItem(UInt index, const T& item) noexcept
	{
		if (index < this->length) {
			this->arrayData[index] = item; return true;
		}
		else return false;
	}

	template <class T>
	UInt Array<T>::copyTo(Array<T>& destination) const
	{
        UInt l = min(this->length, destination.length);
		for (UInt i = 0; i < l; i++) {
			destination.arrayData[i] = this->arrayData[i];
		}
        return l;
	}

    template <class T>
    UInt Array<T>::copyTo(T target[], UInt targetCount) const
    {
        UInt l = min(this->length, targetCount);
        for (UInt i = 0; i < l; i++) {
            target[i] = this->arrayData[i];
        }
        return l;
    }

	template <class T>
	void Array<T>::resize(Array<T>* arr, UInt newSize)
	{
		Array<T> newArr(newSize);
		arr->copyTo(newArr);
		*arr = newArr;
	}

	template <class T>
	Array<T>& Array<T>::operator =(const Array<T>& array)
	{
		if (this->arrayData != array.arrayData)
		{
			T* newData = new T[array.length];

			this->length = array.length;

			delete this->arrayData;
			this->arrayData = newData;

			array.copyTo(*this);
		}
		return *this;
	}

	template <class T>
	Array<T>& Array<T>::operator =(Array<T>&& array) noexcept
	{
		this->arrayData = array.arrayData;
		this->length    = array.length;
		array.arrayData = nullptr;
		array.length    = 0;
		return *this;
	}


	template<class T>
	/* An explicit enumerator for array objects. */
	class ArrayEnumerator : public Enumerator<T>
	{
	private:
		mutable UInt index;
		Array<T>& arr;

	public:
		explicit ArrayEnumerator(Array<T>& arr) noexcept 
            : index(0), arr(arr) { }

        // Returns whether a next item is available.
        bool hasNext() const override {
            return this->index < this->arr.count();
        }

        // Gets the next item and advances the enumerator.
        T &nextItem() override {
            return this->arr.getItem(this->index++);
        }

        // Gets the next item and advances the enumerator.
        const T &nextItem() const override {
            return this->arr.getItem(this->index++);
        }

		~ArrayEnumerator() { }
	};

	template <class T>
	Enumerator<T>* Array<T>::getEnumerator() {
		return new ArrayEnumerator<T>(*this);
	}

    template <class T>
    const Enumerator<T>* Array<T>::getEnumerator() const {
        return new ArrayEnumerator<T>(*const_cast<Array<T>*>(this));
    }
}