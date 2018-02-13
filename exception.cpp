/* exception.cpp - (c) 2018 James Renwick */
#include <exception>

namespace std
{
    void abort();

    namespace _detail
    {
        static void _default_handler()
        {
            std::abort();
        }

        static terminate_handler _term = _default_handler;
    }

    void terminate() noexcept
    {
        // Reset default in case terminate called recursively from within handler
        set_terminate(_detail::_default_handler)();
    }

    terminate_handler set_terminate(terminate_handler handler) noexcept
    {
        auto prev = _detail::_term;
        _detail::_term = handler != nullptr ? handler : _detail::_default_handler;
        return prev;
    }

    terminate_handler get_terminate() noexcept
    {
        return _detail::_term;
    }
}
