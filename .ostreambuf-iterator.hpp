#pragma once
#include "ostream.hpp"
#include "streambuf.hpp"

namespace std
{
    struct output_iterator_tag;


    template<typename Char, typename Traits = char_traits<Char>>
    class ostreambuf_iterator
    {
    public:
        using char_type = Char;
        using traits_type = Traits;
        using streambuf_type = basic_streambuf<Char, Traits>;
        using ostream_type = basic_ostream<Char, Traits>;

        using value_type = void;
        using difference_type = void;
        using pointer = void;
        using reference = void;
        using iterator_category = output_iterator_tag;

    private:
        streambuf_type* _buffer{};
        bool _failed{};

    public:
        ostreambuf_iterator(streambuf_type* buffer) noexcept
            : _buffer(buffer) { }

        ostreambuf_iterator(ostream_type& stream) noexcept
            : ostreambuf_iterator(stream.rdbuf()) { }

        bool failed() const noexcept {
            return _failed;
        }

        ostreambuf_iterator& operator =(Char value)
        {
            if (!_failed) {
                _failed = _buffer->sputc(value) == Traits::eof();
            }
            return *this;
        }

        ostreambuf_iterator& operator *() {
            return *this;
        }

        ostreambuf_iterator& operator++() {
            return *this;
        }
        ostreambuf_iterator& operator++(int) {
            return *this;
        }
    };

}
