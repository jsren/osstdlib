/* exception.hpp - (c) James S Renwick 2014
   ----------------------------------------
   Version 1.0.0
*/
#pragma once
#include "string.hpp"

namespace std
{
    class Exception
    {
    private:
        String message;

    public:
        virtual ~Exception() noexcept { }

        explicit Exception() noexcept 
            : message(String::Empty) { }

        inline Exception(String message) noexcept
            : message(message) { }

        inline Exception(Exception &ex) noexcept
            : message(ex.message) { }

    public:
        inline const String &getMesssage() noexcept {
            return this->message;
        }
    };

    void throwException(Exception &e);
}