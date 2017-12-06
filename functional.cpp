#include "functional.hpp"

namespace std
{
    namespace __detail
    {
        template<>
        extern uint32_t hash_buffer_fnv<uint32_t>(const uint8_t* buffer, size_t size)
        {
            
        }
    }
}
