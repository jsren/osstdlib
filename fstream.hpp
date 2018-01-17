#include <__ios-decl>
#include <__platform>
#include <string>

namespace std
{
    template<typename Char, typename Traits = char_traits<Char>>
    class basic_filebuf : public basic_streambuf<Char, Traits>
    {
    public:
        using char_type = Char;
        using traits_type = Traits;
        using int_type = typename Traits::int_type;
        using pos_type = typename Traits::pos_type;
        using off_type = typename Traits::off_type;
        static_assert(is_same<pos_type, fpos<Traits::state_type>::value, "");

    private:
        static constexpr const size_t defaultBufferSize = 1024;

        bool _open{};
        uint8_t* _buffer{};
        uint8_t* _cursor{};
        uint8_t* _dataEnd{};
        ios_base::openmode _openmode{};
        __platform::__file_handle _handle{};

    public:
        basic_filebuf() = default;

        basic_filebuf(const basic_filebuf& other) = delete;

        basic_filebuf(basic_filebuf&& other) {
            swap(other);
        }

        virtual ~basic_filebuf()
        {
            if (_buffer != nullptr) { delete[] _buffer; }
            try { close(); } catch(...) { }
        }

        basic_filebuf& operator=(basic_filebuf&& other)
        {
            close();
            swap(other);
        }

        basic_filebuf& operator=(const basic_filebuf&) = delete;

        void swap(basic_filebuf& other)
        {
            basic_streambuf<Char, Traits>::swap(other);
            swap()
        }

        bool is_open() const {
            return _open;
        }

        basic_filebuf<Char, Traits>* open(const char* filepath,
            ios_base::openmode mode)
        {
            __platform::__open_options opts{};
            opts.read     = mode & in == in;
            opts.write    = mode & out == out;
            opts.truncate = mode & trunc == trunc;
            opts.append   = mode & app == app;
            opts.binary   = mode & binary == binary;
            opts.create   = opts.write && ((mode & ~ate) != (out | in));
            _openmode = mode;

            _open = __platform::__open(filepath, opts, __platform::__file_acl{}, _handle);
            return _open ? *this : nullptr;
        }

        basic_filebuf<Char, Traits>* open(const string& filepath,
            ios_base::openmode mode)
        {
            return open(filepath.c_str(), mode);
        }

        basic_filebuf<Char, Traits>* close()
        {
            if (!_open) return nullptr;

            bool success = true;
            try
            {
                if (pptr() != nullptr)
                {
                    if (overflow() == Traits::eof()) {
                        success = false;
                    }
                }
                if (!__platform::__close(_handle)) {
                    success = false;
                }
            }
            catch (...)
            {
                __platform::__close(_handle);
                rethrow_exception(current_exception());
            }
            return success ? this : nullptr;
        }

    protected:
        virtual streamsize showmanyc() override {
            return egptr() - gptr();
        }

        virtual int_type underflow() override
        {
            if (pbase() == nullptr)
            {
                // Allocate buffer if we don't already have one
                if (_buffer != nullptr)
                {
                    _buffer = new uint8_t[defaultBufferSize];
                    _cursor = _buffer;
                }
                static_assert(is_same<Char, char>::value,
                    "Non-ASCII character encodings not yet implmented.");
                setg(_buffer, _buffer + defaultBufferSize, _buffer + defaultBufferSize);
                setp(_buffer, _buffer + defaultBufferSize, _buffer + defaultBufferSize);
            }

            if (showmanyc() == 0)
            {
                size_t read = 0;
                auto request = defaultBufferSize;
                if (__platform::__read(_handle, _buffer, request, read))
                {
                    _cursor = _buffer;
                    _dataEnd = _buffer + read;
                    setg(_buffer, _buffer, _buffer + _cursor);
                    setp(_buffer, _buffer, _buffer + _cursor);
                    return Traits::to_int_type(*gptr());
                }
                else return Traits::eof();
            }
            else Traits::to_int_type(*gptr());
        }
    };

    using filebuf = basic_filebuf<char>;
    using wfilebuf = basic_filebuf<wchar_t>;
}
