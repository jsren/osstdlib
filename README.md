# osstdlib

A portable C++14 standard library implementation for embedded/custom OS projects.

This project is a work in progress.

All source (c) 2017-18 James Renwick
except where otherwise stated.

## Project Status

### Done
- array
- string_view
- initializer_list

### Mostly Done
- pointer_traits
- type_traits
- allocators
- stdexcept
- system_error
- tuple
- iterators
- unique/smart_ptr
- vector (w/SSO)
- string (w/SSO)
- limits

### Started ###
- exception
- ios
- iostream
- streambuf
- locale

### Targeted ###
- chrono
- bitset

## Building ##
1. Requires GNU Make, Python 2, and G++/Clang with C++14 support
1. Run `make static` or `make dynamic`
1. Static and shared object files are in `build/`
1. Headers are in `build/include/`

## Usage ##
1. Add generated include directory to your include directories (e.g. with -I)
1. (Both static and dynamic) link with `osstdc++.o`
1. (Dynamic only) link with `libosstdc++.so`

### Optimisation ###
Link-time optimisation builds can be enabled by setting the Make variable `LTO` to `-flto`.
Optimisation level defaults to `Os`, but can be changed via the Make variable `OPT_LVL`.

## (Intentional) Deviations from standard

### std::vector\<bool> specialisation
We have decided not to implement the `std::vector<bool>` specialisation where bool values are packed as in a bitvector. This is for four main reasons:

1. References from operator[] are often a cause of confusion and are generally dangerous
2. Increased testing surface, redundant code, and larger binary sizes
3. `std::bitset` is better when the number of booleans is known at compile-time
4. Implementing this feature is trivial for users via integer types, e.g.

```c++
bool at(const std::vector<std::uint8_t>& vector, std::size_t index)
{
    return vector.at(index / sizeof(std::uint8_t)) &
            (1 << (index % sizeof(std::uint8_t))) != 0;
}
```
