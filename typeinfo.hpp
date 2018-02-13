#pragma once
#include <cstddef>

namespace std
{
    class type_info
    {
    private:
        const char* __type_name;

    public:
        type_info(const type_info&) = delete;
        type_info& operator =(const type_info&) = delete;

    protected:
        explicit type_info(const char* name);

    public:
        virtual ~type_info();

        bool operator ==(const type_info&) const noexcept;
        bool operator !=(const type_info&) const noexcept;
        bool before(const type_info&) const noexcept;
        const char* name() const noexcept;
        size_t hash_code() const;
    };
}
