STD ?= c++1z
OPT_LVL ?= Os

TARGET_STATIC := osstdc++.o
TARGET_DYNAMIC := osstdc++
BUILD_DIR := build

LTO ?=

export OSSTDLIB_PLATFORM ?= linux-i64

FLAGS := -Ibuild/include -nostdlib -fno-exceptions -fno-rtti -$(OPT_LVL) -std=$(STD) $(LTO) -ffunction-sections -fdata-sections -D_OSSTDLIB_PLATFORM=$(OSSTDLIB_PLATFORM)

.PHONY: prepare main-static main-dynamic default test clean

prepare:
	python ./build/prepare.py

$(BUILD_DIR)/$(TARGET_STATIC): prepare
	$(CXX) $(FLAGS) -r __abi.cpp __platform.cpp charconv.cpp cstring.cpp functional.cpp stdexcept.cpp string.cpp -o $(BUILD_DIR)/$(TARGET_STATIC)

$(BUILD_DIR)/lib$(TARGET_DYNAMIC).so: prepare
	$(CXX) $(FLAGS) -fPIC -shared __platform.cpp charconv.cpp cstring.cpp functional.cpp stdexcept.cpp string.cpp -o $(BUILD_DIR)/lib$(TARGET_DYNAMIC).so
	$(CXX) $(FLAGS) -r __abi.cpp -o $(BUILD_DIR)/$(TARGET_STATIC)

static: $(BUILD_DIR)/$(TARGET_STATIC)
dynamic: $(BUILD_DIR)/lib$(TARGET_DYNAMIC).so

main-static: static
	$(CXX) $(FLAGS) -Wl,--gc-sections -s $(BUILD_DIR)/$(TARGET_STATIC) testing/main.cpp -o testing/main

main-dynamic: dynamic
	$(CXX) $(FLAGS) -Wl,--gc-sections -s $(BUILD_DIR)/$(TARGET_STATIC) -L$(BUILD_DIR) -l$(TARGET_DYNAMIC) testing/main.cpp -o testing/main

clean:
	rm -f *.so
	rm -f *.o
	rm -f testing/main
	rm -f build/*.o
	rm -f build/*.so
	rm -f build/include/*

default:
	clang -std=c++14 -Wall -Werror -Wextra -pedantic -Wno-unknown-pragmas -Wno-keyword-macro -Wno-ambiguous-ellipsis -I. string.cpp iterator_test.cpp -O3 -fno-exceptions -o test
	gcc-7 -std=c++14 -Wall -Werror -Wextra -pedantic -Wno-unknown-pragmas -I. string.cpp iterator_test.cpp -O3 -fno-exceptions -o test

test:
	git submodule update --init
	g++-7 -std=c++1z -I. -Itesting/ostest *.cpp testing/*.cpp -O3 -fno-exceptions -fno-rtti testing/ostest/ostest.o -o testing/test.exe
#clang -std=c++1z -I. -Itesting/ostest *.cpp testing/*.cpp -O3 -fno-exceptions -fno-rtti testing/ostest/ostest.o -o testing/test.exe
