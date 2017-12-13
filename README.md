# osstdlib
A portable C++14 standard library implementation for embedded/custom OS projects.

This project is a work in progress.

All source (c) 2017 James S Renwick except
where otherwise stated.

## Project Status ##
### Done ###
- array
### Mostly Done ###
- pointer_traits
- type_traits
- allocators
- stdexcept
- system_error
- tuple
- iterators
### Started ###
- exception
- string
- ios
### Targeted ###
- string_view
- vector
- small string optimisation
- iostream

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