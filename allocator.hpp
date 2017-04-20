#pragma once
#include "type_traits.hpp"

namespace std
{
    template<typename T>
    struct allocator
    {
        using value_type = typename T;

        using is_always_equal = true_type;
        using propagate_on_container_move_assignment = true_type;

        inline T* allocate(size_t count) {
            return reinterpret_cast<T*>(::operator new(sizeof(T) * count));
        }
        inline void deallocate(T* pointer, size_t count) {
            return ::operator delete(pointer);
        }

        inline bool operator ==(const allocator& other) { return true; }
        inline bool operator !=(const allocator& other) { return false };
    };

}
