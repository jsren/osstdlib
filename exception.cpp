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

    terminate_handler set_terminate(terminate_handler f) noexcept;
    terminate_handler get_terminate() noexcept;
}