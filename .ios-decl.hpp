#pragma once
#include <cwchar>
#include <cstddef>
#include <type_traits>

extern "C++" {
namespace std
{
    typedef make_signed_t<size_t> streamsize;
    typedef make_signed_t<size_t> streamoff;

    template<typename State>
    class fpos
    {
        streamoff _offset{};
        State _state{};

    public:
        fpos() = default;
        fpos(int offset) : _offset(offset) { }
        fpos(streamoff offset) : _offset(offset) { }

        fpos(const fpos&) = default;
        fpos(fpos&&) = default;

        fpos& operator=(const fpos&) = default;
        fpos& operator=(fpos&&) = default;

        fpos& operator+=(streamoff rhs) {
            _state = State(); _offset += rhs;
        }
        fpos& operator-=(streamoff rhs) {
            _state = State(); _offset -= rhs;
        }

        operator streamoff() const {
            return _offset;
        }

        State state() const {
            return _state;
        }
        void state(State state) {
            _state = state;
        }
    };

    template<typename T1, typename T2>
    bool operator==(const fpos<T1>& lhs, const fpos<T2>& rhs) {
        return lhs._offset == rhs._offset;
    }
    template<typename T1, typename T2>
    bool operator!=(const fpos<T1>& lhs, const fpos<T2>& rhs) {
        return lhs._offset != rhs._offset;
    }
    template<typename T>
    fpos<T> operator+(const fpos<T>& lhs, streamoff rhs) {
        return fpos<T>(static_cast<streamoff>(lhs) + rhs);
    }
    template<typename T>
    fpos<T> operator+(streamoff lhs, const fpos<T>& rhs) {
        return fpos<T>(lhs + static_cast<streamoff>(rhs));
    }
    template<typename T>
    fpos<T> operator-(const fpos<T>& lhs, streamoff rhs) {
        return fpos<T>(static_cast<streamoff>(lhs) - rhs);
    }
    template<typename T>
    fpos<T> operator-(streamoff lhs, const fpos<T>& rhs) {
        return fpos<T>(lhs - static_cast<streamoff>(rhs));
    }


    using streampos = fpos<mbstate_t>;
    using wstreampos = fpos<mbstate_t>;
    using u16streampos = fpos<mbstate_t>;
    using u32streampos = fpos<mbstate_t>;
}
}