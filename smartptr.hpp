/* smartptr.hpp - (c) James S Renwick 2014-2016
   --------------------------------------------
   Version 1.0.1
*/
#pragma once
#include "std"

namespace std
{
	template<class T>
	/* A pointer type enforcing automatic object destruction when the 
	referenced object leaves the current scope. */ 
	struct unique_ptr
	{
	private:
		T* ptr;

	public:
		/* Creates a new unique_ptr from the given object pointer. */
		unique_ptr(T* obj) noexcept : ptr(obj) { }
		/* Creates a new unique_ptr from the given object reference. */
		unique_ptr(T& obj) noexcept : ptr(&obj) { }

		// Move constructor
		inline unique_ptr(unique_ptr<T>&& ptr) noexcept
		{
			this->ptr = ptr.ptr;
			ptr.ptr   = nullptr;
		}
		// No copy constructor
		unique_ptr(const unique_ptr<T>& ptr) = delete;

		// Move assignment
		inline unique_ptr& operator =(unique_ptr<T>&& ptr) noexcept
		{
			this->ptr = ptr.ptr;
			ptr.ptr   = nullptr;
			return *this;
		}
		// No copy assignment
		unique_ptr& operator =(const unique_ptr<T>& ptr) = delete;

		/* Structure dereference. */
		inline T* operator ->() const noexcept { return ptr; }
		/* Indirection. */
		inline T& operator *() const { return *ptr; }

		/* Calling this destructor will also invoke the destructor
		 of the object being pointed to. */
		~unique_ptr() {
			if (this->ptr != nullptr) delete this->ptr;
		}
	};

    template<class T>
    inline constexpr unique_ptr<T> unique_ptr_from(T &obj) noexcept {
        return unique_ptr<T>(obj);
    }

    template<class T>
    inline constexpr unique_ptr<T> unique_ptr_from(T *obj) noexcept {
        return unique_ptr<T>(obj);
    }

	template<class T>
	/* 
	A pointer type allowing for the creation of other pointers, 
	each pointing to the same object. This creation happens via the copy constructor. 
	Atomatic object destruction occurs when the final referenced to the object is destroyed. 
	*/
	struct shared_ptr
	{
	private:
		T*    ptr;
		UInt* refCount;

	public:
		/* Creates the first shared_ptr for the given object. */
		explicit shared_ptr(T* ptr) : ptr(ptr), refCount(new UInt(1)) { }

		/* Creates the first shared_ptr for the given object. */
		explicit shared_ptr(const T& obj) : ptr(&obj), refCount(new UInt(1)) { }

		/* Creates another shared_ptr pointing to the same object. */
		inline shared_ptr(const shared_ptr& copy) noexcept : ptr(copy.ptr), refCount(copy.refCount)
		{
			*(this->refCount)++;
		}

		/* Destructor. When called, will decrement the object's reference count
		and destroy the object when the count reaches zero. */
		inline ~shared_ptr()
		{
			if (--(*this->refCount) == 0)
			{ 
				delete this->ptr;
				delete this->refCount;
			}
		}

		/*  */
		shared_ptr& operator =(const shared_ptr& value)
		{
			if (this != &value)
			{
				if (--(*this->refCount) == 0)
				{
					delete this->ptr;
					delete this->refCount;
				}
				this->ptr      = value.ptr;
				this->refCount = ++(*value.refCount);
			}
			return *this;
		}

		/* Structure dereference. */
		inline T* operator ->() const noexcept { return ptr; }
		/* Indirection. */
		inline T& operator  *() const { return *ptr; }

		/* Prevent move operations. */
		shared_ptr(shared_ptr&& value)             = delete;
		shared_ptr& operator =(shared_ptr&& value) = delete;
	};


    template<class T>
    inline constexpr shared_ptr<T> shared_ptr_from(T &obj) noexcept {
        return shared_ptr<T>(obj);
    }

    template<class T>
    inline constexpr shared_ptr<T> shared_ptr_from(T *obj) noexcept {
        return shared_ptr<T>(obj);
    }
}
