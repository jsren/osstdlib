#pragma once
#include "std.hpp"

// Take noexcept specifier from expression
#define _noexc_copy_from(expr) noexcept(noexcept(expr))

// Enable default copy/move semantics in class declaration
#define __DefaultCtorCopyMove(T) \
    T() = default; \
	T(const T&) = default; \
	T(T&&) = default; \
	T& operator =(const T&) = default; \
	T& operator =(T&&) = default;

// Enable default copy/move semantics in class declaration
#define __DefaultCopyMove(T) \
	T(const T&) = default; \
	T(T&&) = default; \
	T& operator =(const T&) = default; \
	T& operator =(T&&) = default;

// Enable default copy semantics in class declaration
#define __DefaultCopy(T) \
	T(const T&) = default; \
	T& operator(const T&) = default; 

#define __DefaultVirtualDtor(T) \
    virtual ~T() noexcept = default;
