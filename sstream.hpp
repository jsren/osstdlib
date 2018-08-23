#include <streambuf>
#include <string>

extern "C++" {
namespace std
{
    template<typename Char, typename Traits = char_traits<Char>,
        typename Alloc = allocator<char>>
    class basic_stringbuf : public basic_streambuf<Char, Traits>
    {
    public:
        using char_type = Char;
        using traits_type = Traits;
        using int_type = typename Traits::int_type;
        using pos_type = typename Traits::pos_type;
        using off_type = typename Traits::off_type;
        using allocator_type = Alloc;

        using basic_streambuf<Char, Traits>::eback;
        using basic_streambuf<Char, Traits>::egptr;

    private:
        basic_string<Char, Traits, Alloc> string{};
        ios_base::openmode mode{};

    public:
        explicit basic_stringbuf(ios_base::openmode mode = static_cast<ios_base::openmode>(
            ios_base::in | ios_base::out)) : mode(mode) { };

        explicit basic_stringbuf(const basic_string<Char, Traits, Alloc>& string,
            ios_base::openmode mode = static_cast<ios_base::openmode>(ios_base::in | ios_base::out))
            : string(string), mode(mode) { }

        basic_stringbuf(basic_stringbuf&& other) = default;
        basic_stringbuf(const basic_stringbuf& other) = delete;

        basic_stringbuf& operator =(basic_stringbuf&& other) = default;
        basic_stringbuf& operator =(const basic_stringbuf& other) = delete;

        void swap(basic_stringbuf& other);

        basic_string<Char, Traits, Alloc> str() const {
            return string.substr(eback(), egptr() - eback());
        }
        void str(const basic_string<Char, Traits, Alloc>& string)
        {
            this->string = string;
            if ((mode & ios_base::in) == ios_base::in)
            {
                setg(this->string.data(), this->string.data(),
                    this->string.data() + this->string.size());
            }
            if ((mode & ios_base::out) == ios_base::out)
            {
                auto end = this->string.data() + (
                    (mode & ios_base::app) == ios_base::app ||
                    (mode & ios_base::ate) == ios_base::ate
                    ? this->string.size() : 0);

                setp(this->string.data(), this->string.data(),
                    this->string.data() + this->string.size());
            }
        }
    };

    using stringbuf = basic_stringbuf<char>;
    using wstringbuf = basic_stringbuf<wchar_t>;
}
}
