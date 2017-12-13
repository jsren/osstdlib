#define __PLATFORM_CPP platform/platform-_OSSTDLIB_PLATFORM.cpp
#define __PLATFORM_STR(s) #s
#define __PLATFORM_XSTR(s) __PLATFORM_STR(s)

#include __PLATFORM_XSTR(__PLATFORM_CPP)

#undef __PLATFORM_CPP
#undef __PLATFORM_STR
#undef __PLATFORM_XSTR
