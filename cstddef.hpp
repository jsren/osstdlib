#pragma once
#include <__platform>

namespace std
{
    using size_t = __platform::size_t;
    using max_align_t = __platform::max_align_t;
    using ptrdiff_t = __platform::ptrdiff_t;


    enum class byte : unsigned char { };

	class nullptr_t
	{
	private:
		unsigned : sizeof(void*);

	public:
		template<typename T>
        inline constexpr operator T*() const { return (T*)0; }

		template<typename T, typename Y>
        inline constexpr operator T Y::*() const { return 0; }

		void operator&() const = delete;
		inline constexpr operator void*() const { return (void*)0; }

		inline constexpr bool operator==(const nullptr_t&) const { return true; }
		inline constexpr bool operator!=(const nullptr_t&) const { return false; }
	};
}
