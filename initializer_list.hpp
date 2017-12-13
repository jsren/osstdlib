#include <cstddef>
#include <__iterator>

namespace std
{
    template<typename T>
    class initializer_list
    {
    public:
        using value_type = T;
        using reference = const T&;
        using const_reference = const T&;
        using size_type = size_t;
        using iterator = const T*;
        using const_iterator = const T*;

    private:
        iterator _begin{};
        iterator _end{};

        constexpr size_type size() const noexcept {
            return _end - _begin;
        }
        constexpr iterator begin() const noexcept {
            return _begin;
        }
        constexpr iterator end() const noexcept {
            return _end;
        }
    };

    template<typename T>
    constexpr const T* begin(initializer_list<T> list) noexcept {
        return list.begin();
    }

    template<typename T>
    constexpr const T* end(initializer_list<T> list) noexcept {
        return list.end();
    }

    template<typename T>
    constexpr reverse_iterator<const T*> rbegin(initializer_list<T> list) noexcept {
        return reverse_iterator<const T*>(list.end());
    }

    template<typename T>
    constexpr reverse_iterator<const T*> end(initializer_list<T> list) noexcept {
        return reverse_iterator<const T*>(list.begin());
    }
}
