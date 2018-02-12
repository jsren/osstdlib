STD ?= c++1z
OPT_LVL ?= O3

TARGET_STATIC := osstdc++.o
TARGET_DYNAMIC := osstdc++
BUILD_DIR := build

LTO ?=

export OSSTDLIB_PLATFORM ?= linux-i64

FLAGS := -I$(BUILD_DIR)/include -nostdlib -fno-exceptions -g -fno-stack-protector -fno-rtti -Wno-pragmas -$(OPT_LVL) -std=$(STD) $(LTO) -ffunction-sections -fdata-sections -D_OSSTDLIB_PLATFORM=$(OSSTDLIB_PLATFORM) -Wno-unknown-warning-option

.PHONY: prepare main-static main-dynamic default test clean

prepare:
	python ./build/prepare.py

$(BUILD_DIR)/$(TARGET_STATIC): prepare
	$(CXX) $(FLAGS) -r __abi.cpp __platform.cpp ostream.cpp charconv.cpp cstring.cpp cstdlib.cpp functional.cpp stdexcept.cpp string.cpp tuple.cpp -o $(BUILD_DIR)/$(TARGET_STATIC)

$(BUILD_DIR)/lib$(TARGET_DYNAMIC).so: prepare
	$(CXX) $(FLAGS) -fPIC -shared __platform.cpp ostream.cpp charconv.cpp cstring.cpp cstdlib.cpp functional.cpp stdexcept.cpp string.cpp tuple.cpp -o $(BUILD_DIR)/lib$(TARGET_DYNAMIC).so
	$(CXX) $(FLAGS) -r __abi.cpp -o $(BUILD_DIR)/$(TARGET_STATIC)

static: $(BUILD_DIR)/$(TARGET_STATIC)
dynamic: $(BUILD_DIR)/lib$(TARGET_DYNAMIC).so

main-static: static
	$(CXX) $(FLAGS) -Wl,--gc-sections -s $(BUILD_DIR)/$(TARGET_STATIC) testing/main.cpp -o testing/main

main-dynamic: dynamic
	$(CXX) $(FLAGS) -Wl,--gc-sections -s $(BUILD_DIR)/$(TARGET_STATIC) -L$(BUILD_DIR) -l$(TARGET_DYNAMIC) testing/main.cpp -o testing/main

clean:
	make -C testing/ostest clean
	rm -f *.so
	rm -f *.o
	rm -f testing/main
	rm -f build/*.o
	rm -f build/*.so
	rm -f build/include/*

default:
	clang -std=c++14 -Wall -Werror -Wextra -pedantic -Wno-unknown-pragmas -Wno-keyword-macro -Wno-ambiguous-ellipsis -I. string.cpp iterator_test.cpp -O3 -fno-exceptions -o test
	gcc-7 -std=c++14 -Wall -Werror -Wextra -pedantic -Wno-unknown-pragmas -I. string.cpp iterator_test.cpp -O3 -fno-exceptions -o test

test: static
#	git submodule update --init
	make -C testing/ostest PROFILE=bare CXX=$(CXX)
	$(CXX) $(FLAGS) -DOSTEST_NO_ALLOC -g -Itesting/ostest testing/*.cpp $(BUILD_DIR)/$(TARGET_STATIC) testing/ostest/ostest.o -o testing/test.exe
