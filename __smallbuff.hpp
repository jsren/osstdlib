#include <type_traits>

namespace std
{
    namespace __detail
    {
        template<typename Size, typename Pointer, typename Mover, typename Dtor, typename Dealloc, typename Alloc, size_t Extra>
        struct sbo_config
        {
            Mover move;
            Dtor destroy;
            Dealloc deallocate;
            Alloc allocate;

            static constexpr const size_t extra = Extra;

            using size_type = Size;
            using pointer = Pointer;
            using allocator_type = Alloc;

            sbo_config(Mover&& mover, Dtor&& dtor, Dealloc&& dealloc, Alloc&& alloc)
                : move(reinterpret_cast<Mover&&>(mover)),
                  destroy(reinterpret_cast<Dtor&&>(dtor)),
                  deallocate(reinterpret_cast<Dealloc&&>(dealloc)),
                  allocate(reinterpret_cast<Alloc&&>(allocate)) { }
        };

        template<typename T, typename Traits>
        struct sbo_type
        {
        protected:
            Traits traits;
            using size_type = typename Traits::size_type;
            using pointer = typename Traits::pointer;
            using allocator_type = typename Traits::allocator_type;

            struct long_data
            {
                size_type _capacity;
                size_type _length;
                pointer _data;
            };

            static constexpr const size_type sbo_max_capacity =
                (sizeof(long_data) - sizeof(uint8_t) + Traits::extra)
                / sizeof(T);

            static constexpr const size_type sbo_capacity =
                sbo_max_capacity <= 255 ? sbo_max_capacity : 255;

            struct short_data
            {
                uint8_t _length;
                aligned_storage_t<sbo_capacity, alignof(T)>
            };

            allocator_type _alloc{};

            union {
                long_data _long;
                short_data _short;
            } _repr{};

            constexpr bool is_sbo() const noexcept {
                return (_repr._long._capacity & 0b1) == 0b1;
            }

            constexpr size_type eval_capacity(size_type size) noexcept
            {
                size = ((size & 0b1) == 0) ? size : size + 1;
                if (size > max_size() || size == 0) {
                    __abi::__throw_exception(length_error("size"));
                }
                return size;
            }

            void _set_size(size_type size) noexcept
            {
                if (is_sbo()) _repr._short._length = 0b1 | (size << 1);
                else _repr._long._length = size;
            }

            void _destroy(pointer data, size_type size, size_type capacity)
            {
                for (size_type i = 0; i < size; i++) {
                    traits.destroy(_alloc, &data[i]);
                }
                traits.deallocate(_alloc, data, capacity);
            }

            void _destroy()
            {
                if (is_sso()) {
                    for (size_type i = 0; i < size() + 1; i++) {
                        traits.destroy(_alloc, &data()[i]);
                    }
                }
                else _destroy(data(), size() + 1, _capacity());
            }

            sbo_type(Traits&& traits)
                : traits(reinterpret_cast<Traits&&>(traits)) { }
            
        };
    }
}
