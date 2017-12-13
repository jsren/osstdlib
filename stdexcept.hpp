#pragma once
#include <exception>
#include <memory>
#include <cstring>
#include <__string-decl>

namespace std
{
    namespace __detail
    {
        class exception_base : public exception
        {
        private:
            std::shared_ptr<char> message;

        public:
            explicit exception_base(const std::string& what_arg);

            inline explicit exception_base(const char* what_arg)
                : message(new char[std::strlen(what_arg)])
            {
                std::strcpy(message.get(), what_arg);
            }

            exception_base(const exception_base&) = default;

            inline virtual const char* what() const noexcept override {
                return message.get();
            }
        };
    }


    class runtime_error : public __detail::exception_base
    {
    public:
        inline explicit runtime_error(const std::string& what_arg)
            : __detail::exception_base(what_arg) { }
        inline explicit runtime_error(const char* what_arg)
            : __detail::exception_base(what_arg) { }
        runtime_error(const runtime_error&) = default;
    };

    class logic_error : public __detail::exception_base
    {
    public:
        inline explicit logic_error(const std::string& what_arg)
            : __detail::exception_base(what_arg) { }
        inline explicit logic_error(const char* what_arg)
            : __detail::exception_base(what_arg) { }
        logic_error(const logic_error&) = default;
    };

    class out_of_range : public __detail::exception_base
    {
    public:
        inline explicit out_of_range(const std::string& what_arg)
            : __detail::exception_base(what_arg) { }
        inline explicit out_of_range(const char* what_arg)
            : __detail::exception_base(what_arg) { }
        out_of_range(const out_of_range&) = default;
    };
}
