/* func.hpp - (c) James S Renwick 2014
   -----------------------------------
   Version 1.0.0
*/
#pragma once

namespace std
{
	template<typename T, class RT, class... PTs>
	/* Parameterised function object.

	   T   - The type of the passed function.
	   RT  - The return type of the function.
	   PTs - The function's parameter type(s).
	*/
	struct Function
	{
	private:
		T delegate;

	public:
		/* Creates a new function object from the given function. */
		Function(T delegate) noexcept : delegate(delegate) { }

		/* Creates a new function object from the given function pointer. */
		Function(RT *(delegate)(PTs...)) noexcept : delegate(delegate) { }

		/* Calls the function with the given parameters. Exception neutral. */
		RT operator ()(PTs... args) {
			return (RT)this->delegate(args...);
		}

		bool operator ==(const Function<T, RT, PTs...>& func) noexcept {
			return this->delegate == func.delegate;
		}
		bool operator !=(const Function<T, RT, PTs...>& func) noexcept {
			return this->delegate != func.delegate;
		}
	};

	template<class RT, class... PTs, class T>
	/* Creates a Function object from the given function pointer
	   or lambda.
	   
	   delegate - The function pointer or lambda from which to create the object.
	 */
	static Function<T, RT, PTs...> Delegate(T delegate) noexcept
	{
		return Function<T, RT, PTs...>(delegate);
	}
}