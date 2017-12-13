#include <__string-decl>
#include <ios>

namespace std
{
    template<typename Char, typename Traits = char_traits<Char>>
    class basic_streambuf
    {
    public:
        using char_type = Char;
        using traits_type = Traits;
        using int_type = typename Traits::int_type;
        using pos_type = typename Traits::pos_type;
        using off_type = typename Traits::off_type;

    private:
        Char* _get_start{};
        Char* _get_next{};
        Char* _get_end{};

        Char* _put_start{};
        Char* _put_next{};
        Char* _put_end{};

    protected:
        basic_streambuf() {}
        basic_streambuf(const basic_streambuf& other) = default;

    public:
        virtual ~basic_streambuf() = default;

        basic_streambuf& operator=(const basic_streambuf& other) = default;

        void swap(basic_streambuf& other)
        {
            swap(_get_start, other._get_start);
            swap(_get_next, other._get_next);
            swap(_get_end, other._get_end);
            swap(_put_start, other._put_start);
            swap(_put_next, other._put_next);
            swap(_put_end, other._put_end);
        }

        basic_streambuf* pubsetbuf(char_type* buffer, streamsize size) {
            return setbuf(buffer, size);
        }
        pos_type pubseekoff(off_type offset, ios_base::seekdir direction,
            ios_base::openmode which)
        {
            return seekoff(offset, direction, which);
        }
        pos_type pubseekpos(pos_type position, ios_base::openmode which) {
            return seekpos(position, which);
        }
        int pubsync() {
            return sync();
        }

        streamsize in_avail()
        {
            auto res = _get_end - _get_next;
            return res == 0 ? showmanyc() : res;
        }

        int_type snextc()
        {
            auto res = sbumpc();
            return res == Traits::eof() ? res : sgetc();
        }

        int_type sbumpc()
        {
            if (_get_end != _get_next) {
                return Traits::to_int_type(*_get_next++);
            }
            else return uflow();
        }

        int_type sgetc()
        {
            if (_get_end != _get_next) {
                return Traits::to_int_type(*_get_next);
            }
            else return underflow();
        }

        streamsize sgetn(char_type* buffer, streamsize size) {
            return xsgetn(buffer, size);
        }

        int_type sputc(char_type value)
        {
            if (_put_end != _put_next) {
                return Traits::to_int_type(*_put_next++ = value);
            }
            else return overflow(value);
        }

        streamsize sputn(const char_type* string, streamsize count) {
            return xsputn(string, count);
        }

        int_type sputbackc(char_type value)
        {
            if (_get_next == _get_start || !Traits::eq(value, _get_next[-1])) {
                return bpackfail(Traits::to_int_type(value));
            }
            else return Traits::to_int_type(*_get_next--);
        }

        int_type sungetc(char_type value)
        {
            if (_get_next != _get_start) {
                return Traits::to_int_type(*_get_next--);
            }
            else pbackfail(Traits::to_int_type(value));
        }

    protected:
        char_type* eback() const {
            return _get_start;
        }
        char_type* gptr() const {
            return _get_next;
        }
        char_type* egptr() const {
            return _get_end;
        }
        char_type* pbase() const {
            return _put_start;
        }
        char_type* pptr() const {
            return _put_next;
        }
        char_type* epptr() const {
            return _put_end;
        }

        void gbump(int count) {
            _get_next += count;
        }
        void pbump(int count) {
            _put_next += count;
        }
        void setg(char_type* get_start, char_type* get_next, char_type* get_end) {
            _get_start = get_start; _get_next = get_next; _get_end = get_end;
        }
        void setp(char_type* put_start, char_type* put_end) {
            _put_start = put_start; _put_next = put_start; _put_end = put_end;
        }

        virtual basic_streambuf* setbuf(char_type* buffer, streamsize size) {
            return this;
        }
        virtual pos_type seekoff(off_type offset, ios_base::seekdir direction,
            ios_base::openmode which)
        {
            return static_cast<pos_type>(off_type(-1));
        }
        virtual pos_type seekpos(pos_type position, ios_base::openmode which) {
            return static_cast<pos_type>(off_type(-1));
        }
        virtual int sync() {
            return 0;
        }
        virtual streamsize showmanyc() {
            return 0;
        }
        virtual int_type underflow() {
            return Traits::eof();
        }
        virtual int_type uflow()
        {
            auto value = underflow();
            if (value != Traits::eof()) _get_next++;
            return value;
        }
        virtual int_type overflow() {
            return Traits::eof();
        }
        virtual streamsize xsgetn()(char_type* buffer, streamsize size)
        {
            for (streamsize i = 0; i < size; i++)
            {
                auto value = sbumpc();
                if (value == Traits::eof()) return i;
                else buffer[i] = value;
            }
            return size;
        }
        virtual streamsize xsputn()(const char_type* string, streamsize size)
        {
            for (streamsize i = 0; i < size; i++)
            {
                auto value = sputc(string[i]);
                if (value == Traits::eof()) return i;
            }
            return size;
        }
        virtual int_type pbackfail(int_type value = Traits::eof()) {
            return Traits::eof();
        }
    };

    using streambuf = basic_streambuf<char>;
    using wstreambuf = basic_streambuf<wchar_t>;
}
