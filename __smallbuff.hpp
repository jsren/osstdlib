#pragma once
#include <type_traits>

namespace std
{
    namespace __detail
    {
        template<typename Size, typename Pointer, typename ConstPointer, typename Mover, typename Dtor, typename Dealloc, typename Alloc, size_t Extra, size_t MaxSize>
        struct sbo_config
        {
            Mover move;
            Dtor destroy;
            Dealloc deallocate;
            Alloc allocate;

            static constexpr const size_t extra = Extra;
            static constexpr const size_t max_size = MaxSize;

            using size_type = Size;
            using pointer = Pointer;
            using const_pointer = ConstPointer;
            using allocator_type = Alloc;

            sbo_config(Mover&& mover, Dtor&& dtor, Dealloc&& dealloc, Alloc&& alloc)
                : move(reinterpret_cast<Mover&&>(mover)),
                  destroy(reinterpret_cast<Dtor&&>(dtor)),
                  deallocate(reinterpret_cast<Dealloc&&>(dealloc)),
                  allocate(reinterpret_cast<Alloc&&>(allocate)) { }
        };

        template<typename Traits>
        struct sbo_type
        {
        protected:
            using T = typename Traits::value_type;
            Traits traits;
            using size_type = typename Traits::size_type;
            using pointer = typename Traits::pointer;
            using const_pointer = typename Traits::const_pointer;
            using allocator_type = typename Traits::allocator_type;

            static constexpr const size_t max_size = Traits::max_size;
            static_assert(Traits::realloc_factor > 0, "");

            struct long_data
            {
                size_type _capacity;
                size_type _count;
                pointer _data;
            };

            static constexpr const size_type sbo_max_capacity =
                (sizeof(long_data) - sizeof(uint8_t) + Traits::extra)
                / sizeof(T);

            static constexpr const size_type sbo_capacity =
                sbo_max_capacity <= 127 ? sbo_max_capacity : 127;

            struct short_data
            {
                uint8_t _count;
                aligned_storage_t<sbo_capacity, alignof(T)> _data;
            };

            allocator_type _alloc{};

            union {
                long_data _long;
                short_data _short;
            } _repr{};

            constexpr pointer _data() {
                if (is_sbo()) return pointer(reinterpret_cast<T*>(&_repr._short._data));
                else return _repr._long._data;
            }
            constexpr const_pointer _data() const noexcept {
                if (is_sbo()) return const_pointer(reinterpret_cast<const T*>(&_repr._short._data));
                else return _repr._long._data;
            }
            constexpr size_type _size() const noexcept {
                if (is_sbo()) return _repr._short._count >> 1;
                else return _repr._long._count;
            }
            constexpr size_type _capacity() const noexcept {
                return is_sbo() ? sbo_capacity : _repr._long._capacity;
            }

            constexpr bool is_sbo() const noexcept {
                return (_repr._long._capacity & 0b1) == 0b1;
            }

            constexpr size_type eval_capacity(size_type size) noexcept
            {
                // Make re-allocation at least 1.5 times current capacity
                size_type target = _capacity() + static_cast<size_type>(
                    _capacity() / Traits::realloc_factor);
                if (size < target) size = target;

                size = ((size & 0b1) == 0) ? size : size + 1;
                if (size > traits.max_size || size == 0) {
                    __abi::__throw_exception(length_error("size"));
                }
                return size;
            }

            void _set_size(size_type size) noexcept
            {
                if (is_sbo()) _repr._short._count = 0b1 | (size << 1);
                else _repr._long._count = size;
            }

            void _destroy()
            {
                _destroy(_data(), _size());
            }

            void _destroy(pointer data, size_type size)
            {
                for (size_type i = 0; i < size; i++) {
                    traits.destroy(_alloc, data + i);
                }
            }

            pointer _reallocate(size_type newCapacity)
            {
                pointer data;
                pointer prevData = _data();
                size_type prevSize = _size();
                size_type prevCapacity = _capacity();
                bool prevHeap = !is_sbo() && prevData != nullptr;
                size_type newSize = (prevSize > newCapacity) ?
                    newCapacity : prevSize;

                // Inline buffer
                // TODO: This is probably the wrong thing to do
                if (newCapacity <= sbo_capacity)
                {
                    _repr._short._count = 0b1 | (newSize << 1); // Set SBO flag
                    data = reinterpret_cast<pointer>(&_repr._short._data);

                    if (prevSize != 0)
                    {
                        if (!prevHeap) // Destroy extra items if shrunk
                        {
                            for (size_type i = newSize; i < prevSize; i++) {
                                prevData[i].~T();
                            }
                        }
                        else // Copy items from heap
                        {
                            for (size_type i = 0; i < prevSize; i++) {
                                prevData[i].~T();
                            }
                        }
                    }
                }
                // Heap buffer
                else
                {
                    newCapacity = eval_capacity(newCapacity);
                    data = traits.allocate(_alloc, newCapacity);

                    // Move and destroy previous data
                    traits.move(data, prevData, prevSize < newSize ? prevSize : newSize);
                    if (!prevHeap && prevCapacity != 0)
                    {
                        for (size_type i = 0; i < prevSize; i++) {
                            prevData[i].~T();
                        }
                    }
                    this->_repr._long._count = newSize;
                    this->_repr._long._capacity = newCapacity;
                    this->_repr._long._data = data;
                }

                if (prevHeap) {
                    _destroy(prevData, prevSize);
                    traits.deallocate(_alloc, prevData, prevCapacity);
                }
                return data;
            }

            sbo_type(Traits&& traits) : traits(reinterpret_cast<Traits&&>(traits))
            {
                _repr._short._count = 1; // Default to SBO
            }

            sbo_type(Traits&& traits, const allocator_type& alloc)
                : traits(reinterpret_cast<Traits&&>(traits)), _alloc(alloc)
            {
                _repr._short._count = 1; // Default to SBO
            }

            ~sbo_type()
            {
                _destroy();
                if (!is_sbo()) traits.deallocate(_alloc, _data(), _capacity());
            }
        };
    }
}
