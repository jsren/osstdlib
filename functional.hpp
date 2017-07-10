#pragma once

#include "type_traits.hpp"

namespace std
{
    template<typename T>
    class reference_wrapper
    {
    private:
        T* pointer;

    public:
        using type = T;

        reference_wrapper(T& reference) noexcept 
            : pointer(&reference) { }
    
        reference_wrapper(const reference_wrapper<T>& other) noexcept 
            : pointer(other.get()) { }

        reference_wrapper(T&& reference) = delete;
        
        reference_wrapper& operator=(const reference_wrapper<T>& other) noexcept {
            pointer = &other.get();
        }

        operator T&() const noexcept {
            return *pointer;
        }
        T& get() const noexcept {
            return *pointer;
        }

        template<typename... Args>
        invoke_result_t<T&, Args...> operator()(Args&&... args) const
        {
#pragma warn "TODO: Enable if callable"
            return (*pointer)(std::forward<Args>(args)...);
        }
    }; 

#ifdef __cpp_deduction_guides
    template<typename T>
    reference_wrapper(reference_wrapper<T>) -> reference_wrapper<T>;
#endif

    template<typename T>
    inline reference_wrapper<T> ref(T& reference) noexcept {
        return reference_wrapper<T>(reference);
    }
    template<typename T>
    inline reference_wrapper<T> ref(reference_wrapper<T> reference) noexcept {
        return reference_wrapper<T>(reference.get());
    }
    template<typename T>
    inline reference_wrapper<const T> cref(const T& reference) noexcept {
        return reference_wrapper<const T>(reference);
    }
    template<typename T>
    inline reference_wrapper<const T> cref(reference_wrapper<T> reference) noexcept {
        return reference_wrapper<T>(reference.get());
    }
    
    template<typename T>
    void ref(const T&&) = delete;

    template<typename T>
    void cref(const T&&) = delete;



}
