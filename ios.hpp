/* ios.hpp - (c) 2017 James Renwick */
#pragma once
#include <system_error>
#include <atomic>
#include <memory>

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
	
	
	typedef make_signed_t<size_t> streamsize;
    typedef make_signed_t<size_t> streamoff;

    class ios_base
    {
    private:
        static atomic<int> xalloc_next;
        unique_ptr<long[]> iarray;
        unique_ptr<void*[]> parray;

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
        fmtflags flags() const;
        fmtflags flags(fmtflags fmtfl);
        fmtflags setf(fmtflags fmtfl);
        fmtflags setf(fmtflags fmtfl, fmtflags mask);
        void unsetf(fmtflags mask);
        streamsize precision() const;
        streamsize precision(streamsize prec);
        streamsize width() const;
        streamsize width(streamsize wide);

        // storage:
        static int xalloc();
        long& iword(int index);
        void*& pword(int index);

        // destructor
        virtual ~ios_base() { }

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
        ios_base();

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
