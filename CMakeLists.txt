cmake_minimum_required (VERSION 2.6)
project (osstdlib CXX)

set(VERSION "0.1.1")
set(SRC_DIR ${CMAKE_CURRENT_SOURCE_DIR})


include_directories("${SRC_DIR}")

file(GLOB C_SOURCES "${SRC_DIR}/*.c")
file(GLOB CXX_SOURCES "${SRC_DIR}/*.cpp")
file(GLOB HEADERS1 "${SRC_DIR}/*.h")
file(GLOB HEADERS2 "${SRC_DIR}/*.hpp")
list(APPEND HEADERS ${HEADERS1} ${HEADERS2})


if ("${CMAKE_C_COMPILER_ID}" STREQUAL "Clang" OR "${CMAKE_C_COMPILER_ID}" STREQUAL "GNU")
    set(CMAKE_CXX_FLAGS "-Wall -Wextra -pedantic -nostdlib -std=c++1y -O3 ${CMAKE_CXX_FLAGS}")
else()
    message(WARNING "Unsupported compiler. Not setting compiler flags.")
endif()

add_library(osstdlib STATIC ${C_SOURCES} ${CXX_SOURCES} ${HEADERS})

install (TARGETS osstdlib DESTINATION bin)
install (FILES *.h DESTINATION include)
