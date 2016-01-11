#pragma once
#include "std.hpp"

namespace std
{
	inline volatile void memory_barrier()
	{
#ifdef _MSC_VER
		__asm mfence;
#else
#ifdef _GNUC_
		return __sync_synchronize();
#else
#error "Unsupported compiler or platform"
#endif
#endif
	}

	inline volatile Int32 atomic_exchange(void* pointer, Int32 value)
	{
#ifdef _MSC_VER
		__asm mov  eax,   value
		__asm mov  ebx,   pointer
		__asm xchg [ebx], eax
		__asm mov  value, eax
		return value;
#else
#ifdef _GNUC_
		return __sync_lock_test_and_set(ptr, value);
#else
#error "Unsupported compiler or platform"
#endif
#endif
	}

	inline volatile Int32 atomic_cas(void* pointer)
	{
		Int32 value = 1;
#ifdef _MSC_VER
		__asm
		{
			; Big speedup when used in loop
			; by testing first without lock
			mov      ebx, pointer
			mov      eax, [ebx]
			cmp      eax, 0
			jne      End

			mov      ecx,   1
			cmpxchg [ebx],  ecx ; EAX=0
			mov      value, ecx
		End:
		}
		return value;
#else
#ifdef _GNUC_
		return __sync_val_compare_and_swap(ptr, value);
#else
#error "Unsupported compiler or platform"
#endif
#endif
	}

	inline volatile Int32 synchronised_add(void* pointer, Int32 value)
	{
#ifdef _MSC_VER
		__asm      mov   eax,   value
		__asm      mov   ebx,   pointer
		__asm lock xadd [ebx],  eax;
		__asm      mov   value, eax
		return value;
#else
#ifdef _GNUC_
		return __sync_fetch_and_add(ptr, value);
#else
#error "Unsupported compiler or platform"
#endif
#endif
	}

	class Lock
	{
	public:
		virtual ~Lock() { this->free(); };

	public:
		virtual bool acquire() = 0;
		virtual bool free() noexcept = 0;
		virtual bool hasLock() const noexcept = 0;
	};

	class SpinLock : public Lock
	{
	private:
		bool cleanup;
		bool locked;
		Int32* variable;

	public:
		inline explicit SpinLock()
			: cleanup(true), locked(false), variable(new Int32{0}) { }

		inline explicit SpinLock(Int32* variable) 
			: cleanup(false), locked(false), variable(variable) { }

		inline SpinLock(const SpinLock& lock)
			: cleanup(false), locked(false), variable(lock.variable) { }

	public:
		inline bool acquire() override {
			while (atomic_cas(variable)) { } return true;
		}
		inline bool free() noexcept override {
			*variable = 0;
		}
		inline bool hasLock() const noexcept override {
			return this->locked;
		}
	};

}
