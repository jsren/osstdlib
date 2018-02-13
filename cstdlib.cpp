/* cstdlib.cpp - (c) 2018 James Renwick */
extern "C" void __cxa_finalize(void (*dtor)(void*));
extern "C" void _exit(int);

namespace std
{
    void abort()
    {
        ::_exit(1);
    }

    void exit(int status)
    {
        ::__cxa_finalize(nullptr);
        ::_exit(status);
    }
}
