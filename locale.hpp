#include ".ostreambuf-iterator.hpp"

namespace std
{
    namespace locale
    {
        class id
        {
            friend class facet;
            unsigned long long _value;

        public:
            id() = default;
            id(const id&) = delete;
            id& operator=(const id&) = delete;
        };


        class facet
        {
            explicit facet(size_t initRefCount = 0);
            facet(const facet&) = delete;
        };
    }


    template<typename Char, typename OutputIt = ostreambuf_iterator<Char>>
    class num_put;

    template<>
    class num_put<char, ostreambuf_iterator<char>>
        : public locale::facet
    {
    public:
        using char_type = char;
        using iter_type = ostreambuf_iterator<char>;

        explicit num_put(size_t refCountStart)
            : locale::facet(refCountStart) { }
    };
}
