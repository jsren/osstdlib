/*#if defined(__GNUC__)
#if __LP64__*/
#include "impl/_platform-linux-i64.cpp"/*
#else
#include "impl/_platform-linux-i32.cpp"
#endif
#else
#pragma error "Unknown or not-implemented platform"
#endif
*/