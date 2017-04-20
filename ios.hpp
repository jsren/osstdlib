#pragma once
#include "system_error.hpp"
#include "atomic.hpp"
#include "memory.hpp"

namespace __std
{
    namespace __fmtflags {
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
    namespace __iostate {
        enum enum_ {
            goodbit = 0,
            badbit  = 0x1,
            eofbit  = 0x2,
            failbit = 0x4
        };
    }
    namespace __openmode {
        enum enum_ {
            app    = 0x1,
            ate    = 0x2,
            binary = 0x4,
            in     = 0x8,
            out    = 0x10,
            trunc  = 0x20
        };
    }
    namespace __seekdir {
        enum enum_ {
            beg = 0x1,
            cur = 0x2,
            end = 0x4
        };
    }
}

namespace std
{
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
        typedef __std::__fmtflags::enum_ fmtflags;
        static constexpr fmtflags boolalpha = __std::__fmtflags::boolalpha;
        static constexpr fmtflags dec = __std::__fmtflags::dec;
        static constexpr fmtflags fixed = __std::__fmtflags::fixed;
        static constexpr fmtflags hex = __std::__fmtflags::hex;
        static constexpr fmtflags internal = __std::__fmtflags::internal;
        static constexpr fmtflags left = __std::__fmtflags::left;
        static constexpr fmtflags oct = __std::__fmtflags::oct;
        static constexpr fmtflags right = __std::__fmtflags::right;
        static constexpr fmtflags scientific = __std::__fmtflags::scientific;
        static constexpr fmtflags showbase = __std::__fmtflags::showbase;
        static constexpr fmtflags showpoint = __std::__fmtflags::showpoint;
        static constexpr fmtflags showpos = __std::__fmtflags::showpos;
        static constexpr fmtflags skipws = __std::__fmtflags::skipws;
        static constexpr fmtflags unitbuf = __std::__fmtflags::unitbuf;
        static constexpr fmtflags uppercase = __std::__fmtflags::uppercase;
        static constexpr fmtflags adjustfield = static_cast<fmtflags>(left | right | internal);
        static constexpr fmtflags basefield = static_cast<fmtflags>(dec | oct | hex);
        static constexpr fmtflags floatfield = static_cast<fmtflags>(scientific | fixed);

        typedef __std::__iostate::enum_ iostate;
        /* Flag indicating no error. */
        static constexpr iostate goodbit = __std::__iostate::goodbit;
        /* Flag indicating an irrecoverable stream error. */
        static constexpr iostate badbit = __std::__iostate::badbit;
        /* Flag indicating a formatting or extraction error. */
        static constexpr iostate eofbit = __std::__iostate::eofbit;
        /* Flag indicating that the end of file has been reached. */
        static constexpr iostate failbit = __std::__iostate::failbit;

        // openmode
        typedef __std::__openmode::enum_ openmode;
        /* Flag indicating a seek to end of stream before each write. */
        static constexpr openmode app = __std::__openmode::app;
        /* Flag indicating a seek to end of stream after opening. */
        static constexpr openmode ate = __std::__openmode::ate;
        /* Flag indicating opening in binary mode. */
        static constexpr openmode binary = __std::__openmode::binary;
        /* Flag indicating opening for reading. */
        static constexpr openmode in = __std::__openmode::in;
        /* Flag indicating opening for writing. */
        static constexpr openmode out = __std::__openmode::out;
        /* Flag indicating that the contents of the stream should be discarded upon open. */
        static constexpr openmode trunc = __std::__openmode::trunc;

        // seekdir
        typedef __std::__seekdir::enum_ seekdir;
        /* Flag indicating seeking relative to the begining of the stream. */
        static constexpr seekdir beg = __std::__seekdir::beg;
        /* Flag indicating seeking relative to the current position. */
        static constexpr seekdir cur = __std::__seekdir::cur;
        /* Flag indicating seeking relative to the end of the stream. */
        static constexpr seekdir end = __std::__seekdir::end;

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

namespace __abi
{
    extern int __ecode_from_enum(std::io_errc) noexcept;
}

namespace std
{

    inline error_code make_error_code(io_errc e) {
        return error_code(__abi::__ecode_from_enum(e), iostream_category());
    }

    /*inline error_condition make_error_condition(io_errc e) {
        return error_condition(static_cast<int>(static_cast<underlying_type_t<io_errc>>(e)),
            iostream_category());
    }*/
}
