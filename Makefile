STD ?= c++14
OPT_LVL ?= O0

TARGET_STATIC := osstdc++.o
TARGET_DYNAMIC := osstdc++
BUILD_DIR := build

LTO ?=

export OSSTDLIB_PLATFORM ?= linux-i64
export OSSTDLIB_COMPILER ?= gnu

ABI_DIR ?= osabi
PLATFORM_DIR := $(ABI_DIR)/$(OSSTDLIB_PLATFORM)
LINKER_SCRIPT := $(ABI_DIR)/$(OSSTDLIB_COMPILER)/linker.ld

FLAGS := -I$(BUILD_DIR)/include -nostdlib -fno-exceptions -g -fno-stack-protector -fno-rtti -Wno-pragmas -$(OPT_LVL) -std=$(STD) $(LTO) -ffunction-sections -fdata-sections -D_OSSTDLIB_PLATFORM=$(OSSTDLIB_PLATFORM) -Wno-main
SOURCES := exception.cpp ostream.cpp charconv.cpp cstring.cpp cstdlib.cpp functional.cpp stdexcept.cpp string.cpp tuple.cpp

.PHONY: prepare main-static main-dynamic default test clean

prepare:
	python ./build/prepare.py
	cp $(LINKER_SCRIPT) ./build/

$(BUILD_DIR)/$(TARGET_STATIC): prepare
	$(CXX) $(FLAGS) -r $(ABI_DIR)/__abi.cpp $(PLATFORM_DIR)/*.cpp $(PLATFORM_DIR)/*.s $(SOURCES) -o $(BUILD_DIR)/$(TARGET_STATIC)

$(BUILD_DIR)/lib$(TARGET_DYNAMIC).so: prepare
	$(CXX) $(FLAGS) -fPIC -shared $(PLATFORM_DIR)/*.cpp $(PLATFORM_DIR)/*.s $(SOURCES) -o $(BUILD_DIR)/lib$(TARGET_DYNAMIC).so
	$(CXX) $(FLAGS) -r $(ABI_DIR)/__abi.cpp -o $(BUILD_DIR)/$(TARGET_STATIC)

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
	$(CXX) $(FLAGS) -Wall -Wextra -pedantic -Wno-unused-parameter -Wl,--script=$(LINKER_SCRIPT) -Wl,--gc-sections -DOSTEST_NO_ALLOC -Itesting/ostest testing/*.cpp $(BUILD_DIR)/$(TARGET_STATIC) testing/ostest/ostest.o -o testing/test.exe
