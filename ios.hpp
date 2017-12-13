/* ios.hpp - (c) 2017 James Renwick */
#pragma once
#include <system_error>
#include <atomic>
#include <memory>
#include <__ios-decl>

namespace std
{
	namespace __detail
	{
		namespace fmtflags {
			enum enum_ {
				boolalpha  = 0x01,
				dec        = 0x02,
				fixed      = 0x04,
				hex        = 0x08,
				internal   = 0x10,
				left       = 0x20,
				oct        = 0x40,
				right      = 0x80,
				scientific = 0x100,
				showbase   = 0x200,
				showpoint  = 0x400,
				showpos    = 0x800,
				skipws     = 0x1000,
				unitbuf    = 0x2000,
				uppercase  = 0x4000
			};
		}
		namespace iostate {
			enum enum_ {
				goodbit = 0,
				badbit  = 0x1,
				eofbit  = 0x2,
				failbit = 0x4
			};
		}
		namespace openmode {
			enum enum_ {
				app    = 0x1,
				ate    = 0x2,
				binary = 0x4,
				in     = 0x8,
				out    = 0x10,
				trunc  = 0x20
			};
		}
		namespace seekdir {
			enum enum_ {
				beg = 0x1,
				cur = 0x2,
				end = 0x4
			};
		}
	}


    class ios_base
    {
    private:
        static atomic<int> xalloc_next;
        unique_ptr<long[]> iarray;
        unique_ptr<void*[]> parray;
        __detail::fmtflags::enum_ _flags{};
        streamsize _precision{};
        streamsize _width{};

    public:
        class failure;

        // fmtflags
        typedef __detail::fmtflags::enum_ fmtflags;
        static constexpr fmtflags boolalpha = fmtflags::boolalpha;
        static constexpr fmtflags dec = fmtflags::dec;
        static constexpr fmtflags fixed = fmtflags::fixed;
        static constexpr fmtflags hex = fmtflags::hex;
        static constexpr fmtflags internal = fmtflags::internal;
        static constexpr fmtflags left = fmtflags::left;
        static constexpr fmtflags oct = fmtflags::oct;
        static constexpr fmtflags right = fmtflags::right;
        static constexpr fmtflags scientific = fmtflags::scientific;
        static constexpr fmtflags showbase = fmtflags::showbase;
        static constexpr fmtflags showpoint = fmtflags::showpoint;
        static constexpr fmtflags showpos = fmtflags::showpos;
        static constexpr fmtflags skipws = fmtflags::skipws;
        static constexpr fmtflags unitbuf = fmtflags::unitbuf;
        static constexpr fmtflags uppercase = fmtflags::uppercase;
        static constexpr fmtflags adjustfield = static_cast<fmtflags>(left | right | internal);
        static constexpr fmtflags basefield = static_cast<fmtflags>(dec | oct | hex);
        static constexpr fmtflags floatfield = static_cast<fmtflags>(scientific | fixed);

        typedef __detail::iostate::enum_ iostate;
        /* Flag indicating no error. */
        static constexpr iostate goodbit = iostate::goodbit;
        /* Flag indicating an irrecoverable stream error. */
        static constexpr iostate badbit = iostate::badbit;
        /* Flag indicating a formatting or extraction error. */
        static constexpr iostate eofbit = iostate::eofbit;
        /* Flag indicating that the end of file has been reached. */
        static constexpr iostate failbit = iostate::failbit;

        // openmode
        typedef __detail::openmode::enum_ openmode;
        /* Flag indicating a seek to end of stream before each write. */
        static constexpr openmode app = openmode::app;
        /* Flag indicating a seek to end of stream after opening. */
        static constexpr openmode ate = openmode::ate;
        /* Flag indicating opening in binary mode. */
        static constexpr openmode binary = openmode::binary;
        /* Flag indicating opening for reading. */
        static constexpr openmode in = openmode::in;
        /* Flag indicating opening for writing. */
        static constexpr openmode out = openmode::out;
        /* Flag indicating that the contents of the stream should be discarded upon open. */
        static constexpr openmode trunc = openmode::trunc;

        // seekdir
        typedef __detail::seekdir::enum_ seekdir;
        /* Flag indicating seeking relative to the begining of the stream. */
        static constexpr seekdir beg = seekdir::beg;
        /* Flag indicating seeking relative to the current position. */
        static constexpr seekdir cur = seekdir::cur;
        /* Flag indicating seeking relative to the end of the stream. */
        static constexpr seekdir end = seekdir::end;

        class Init;

        // fmtflags state:
        fmtflags flags() const {
            return _flags;
        }
        fmtflags flags(fmtflags flags)
        {
            auto old = _flags;
            _flags = flags;
            return old;
        }
        fmtflags setf(fmtflags fmtfl);
        fmtflags setf(fmtflags fmtfl, fmtflags mask);
        void unsetf(fmtflags mask);
        inline streamsize precision() const {
            return _precision;
        }
        streamsize precision(streamsize precision)
        {
            auto old = _precision;
            _precision = precision;
            return old;
        }
        inline streamsize width() const {
            return _width;
        }
        streamsize width(streamsize width)
        {
            auto old = _width;
            _width = width;
            return old;
        }

        // storage:
        static int xalloc();
        long& iword(int index);
        void*& pword(int index);

        // destructor
        virtual ~ios_base() = default;

        // callbacks;
        enum event {
            erase_event,
            imbue_event,
            copyfmt_event
        };
        typedef void(*event_callback)(event, ios_base&, int index);
        void register_callback(event_callback fn, int index);

        ios_base(const ios_base&) = delete;
        ios_base& operator=(const ios_base&) = delete;
        static bool sync_with_stdio(bool sync = true);

    protected:
        iostate _iostate{};

        ios_base() = default;

        inline void move(ios_base& other)
        {
            swap(iarray, other.iarray);
            swap(parray, other.parray);
            swap(_flags, other._flags);
            swap(_precision, other._precision);
            swap(_width, other._width);
            swap(_iostate, other._iostate);
        }
    };


    // manipulators:
    ios_base& boolalpha(ios_base& str);
    ios_base& noboolalpha(ios_base& str);
    ios_base& showbase(ios_base& str);
    ios_base& noshowbase(ios_base& str);
    ios_base& showpoint(ios_base& str);
    ios_base& noshowpoint(ios_base& str);
    ios_base& showpos(ios_base& str);
    ios_base& noshowpos(ios_base& str);
    ios_base& skipws(ios_base& str);
    ios_base& noskipws(ios_base& str);
    ios_base& uppercase(ios_base& str);
    ios_base& nouppercase(ios_base& str);
    ios_base& unitbuf(ios_base& str);
    ios_base& nounitbuf(ios_base& str);
    ios_base& internal(ios_base& str);
    ios_base& left(ios_base& str);
    ios_base& right(ios_base& str);
    ios_base& dec(ios_base& str);
    ios_base& hex(ios_base& str);
    ios_base& oct(ios_base& str);
    ios_base& fixed(ios_base& str);
    ios_base& scientific(ios_base& str);
    ios_base& hexfloat(ios_base& str);
    ios_base& defaultfloat(ios_base& str);

    enum class io_errc {
        stream = 1
    };

    template <>
    struct is_error_code_enum<io_errc> : public true_type { };

    const error_category& iostream_category();


    template<typename Char, typename Traits = char_traits<Char>>
    class basic_ostream;

    template<typename Char, typename Traits = char_traits<Char>>
    class basic_ios : public ios_base
    {
    public:
        using char_type = Char;
        using traits_type = Traits;
        using int_type = Traits::int_type;
        using pos_type = Traits::pos_type;
        using off_type = Traits::off_type;

    private:
        Char _fill{' '};
        basic_streambuf<Char, Traits>* _streambuf{};
        basic_ostream<Char, Traits>* _tie{};

    protected:
        basic_ios() = default;

    public:
        virtual ~basic_ios() = default;

        explicit basic_ios(basic_streambuf<Char, Traits>* streambuf)
            : _streambuf(streambuf) { }

        basic_ios(const basic_ios&) = delete;

        Char fill() const {
            return _fill;
        }
        Char fill(Char newValue)
        {
            auto old = _fill;
            _fill = newValue;
            return old;
        }

        basic_ostream<Char, Traits>* tie() const {
            return _tie;
        }
        basic_ostream<Char, Traits>* tie(basic_ostream<Char, Traits>* newValue)
        {
            auto old = _tie;
            _tie = newValue;
            return old;
        }

        iostate rdstate() const {
            return ios_base::_iostate;
        }

        bool good() const {
            return rdstate() == 0;
        }
        bool eof() const {
            return rdstate() & iostate::eofbit == iostate::eofbit;
        }
        bool fail() const {
            return rdstate() & iostate::failbit == iostate::failbit
                || rdstate() & iostate::badbit == iostate::badbit;
        }
        bool bad() const {
            return rdstate() & iostate::badbit == iostate::badbit;
        }
        bool operator!() const {
            return fail();
        }
        operator bool() const {
            return !fail();
        }
        void setstate(iostate state) {
            clear(rdstate() | state);
        }
        void clear(iostate state = ios_base::goodbit) {
            _iostate = state | (rdbuf() == nullptr ? badbit : 0);
        }

        char narrow(char_type value, char default_) const;
        char_type widen(char value) const;

    protected:
        void init(basic_streambuf<Char, Traits>* streambuf)
        {
            _streambuf = streambuf;
            _tie = nullptr;
            clear();
            flags(skipws | dec);
            width(0);
            precision(6);
            _fill = widen(' ');
        }

        void move(basic_ios&& other)
        {
            ios_base::move(other);
            swap(_fill, other._fill);
            _tie = other._tie;
            other._tie = 0;
        }

        void swap(basic_ios& other) noexcept
        {
            ios_base::move(other);
            swap(_fill, other._fill);
            swap(_tie, other._tie);
        }

        void set_rdbuf(basic_streambuf<Char, Traits>* buffer) {
            _streambuf = buffer;
        }
    };

    using ios = basic_ios<char>;
    using wios = basic_ios<wchar_t>;
}

namespace __platform
{
    extern int __ecode_from_enum(std::io_errc) noexcept;
}

namespace std
{

    inline error_code make_error_code(io_errc e) {
        return error_code(__platform::__ecode_from_enum(e), iostream_category());
    }

    /*inline error_condition make_error_condition(io_errc e) {
        return error_condition(static_cast<int>(static_cast<underlying_type_t<io_errc>>(e)),
            iostream_category());
    }*/
}
