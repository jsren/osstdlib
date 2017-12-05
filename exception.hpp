#pragma once


namespace std
{
    class exception
    {
    private:
        const char empty[1] = { 0 };

    public:
        exception() noexcept = default;
        virtual ~exception() = default;

        exception(const exception&) noexcept = default;
        exception& operator=(const exception&) noexcept = default;

        inline virtual const char* what() const noexcept {
            return empty;
        }
    };
}

namespace __abi {
    [[noreturn]] void __throw_exception(const std::exception&) noexcept;
}
