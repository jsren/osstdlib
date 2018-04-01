STD ?= c++14
OPT_LVL ?= O0

TARGET_STATIC := osstdc++.o
TARGET_DYNAMIC := osstdc++
BUILD_DIR := build

LINKER_SCRIPT := impl/gnu/linker.ld

LTO ?=

export OSSTDLIB_PLATFORM ?= linux-i64

FLAGS := -I$(BUILD_DIR)/include -Wall -Wextra -pedantic -nostdlib -fno-exceptions -g -fno-stack-protector -fno-rtti -Wno-pragmas -$(OPT_LVL) -std=$(STD) $(LTO) -ffunction-sections -fdata-sections -D_OSSTDLIB_PLATFORM=$(OSSTDLIB_PLATFORM) -Wno-unknown-warning-option -Wno-invalid-noreturn -Wno-main

.PHONY: prepare main-static main-dynamic default test clean

prepare:
	python ./build/prepare.py

$(BUILD_DIR)/$(TARGET_STATIC): prepare
	$(CXX) $(FLAGS) -r __abi.cpp impl/$(OSSTDLIB_PLATFORM)/*.cpp impl/$(OSSTDLIB_PLATFORM)/*.s exception.cpp ostream.cpp charconv.cpp cstring.cpp cstdlib.cpp functional.cpp stdexcept.cpp string.cpp tuple.cpp -o $(BUILD_DIR)/$(TARGET_STATIC)

$(BUILD_DIR)/lib$(TARGET_DYNAMIC).so: prepare
	$(CXX) $(FLAGS) -fPIC -shared impl/$(OSSTDLIB_PLATFORM)/*.cpp impl/$(OSSTDLIB_PLATFORM)/*.s exception.cpp ostream.cpp charconv.cpp cstring.cpp cstdlib.cpp functional.cpp stdexcept.cpp string.cpp tuple.cpp -o $(BUILD_DIR)/lib$(TARGET_DYNAMIC).so
	$(CXX) $(FLAGS) -r __abi.cpp -o $(BUILD_DIR)/$(TARGET_STATIC)

static: $(BUILD_DIR)/$(TARGET_STATIC)
dynamic: $(BUILD_DIR)/lib$(TARGET_DYNAMIC).so

main-static: static
	$(CXX) $(FLAGS) -Wl,--script=$(LINKER_SCRIPT) -Wl,--gc-sections -s $(BUILD_DIR)/$(TARGET_STATIC) testing/main.cpp -o testing/main

main-dynamic: dynamic
	$(CXX) $(FLAGS) -Wl,--script=$(LINKER_SCRIPT) -Wl,--gc-sections -s $(BUILD_DIR)/$(TARGET_STATIC) -L$(BUILD_DIR) -l$(TARGET_DYNAMIC) testing/main.cpp -o testing/main

clean:
	make -C testing/ostest clean
	rm -f *.so
	rm -f *.o
	rm -f testing/main
	rm -f build/*.o
	rm -f build/*.so
	rm -f build/include/*

test: static
	make -C testing/ostest PROFILE=bare CXX=$(CXX)
	$(CXX) $(FLAGS) -Wl,--script=$(LINKER_SCRIPT) -Wl,--gc-sections -DOSTEST_NO_ALLOC -Itesting/ostest testing/*.cpp $(BUILD_DIR)/$(TARGET_STATIC) testing/ostest/ostest.o -o testing/test.exe
