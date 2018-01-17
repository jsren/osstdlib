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
    bool __uncaught_exception() noexcept;
    [[noreturn]] void __throw_exception(const std::exception&) noexcept;
}

namespace std
{
    inline bool uncaught_exception() noexcept {
        return __abi::__uncaught_exception();
    }
}
