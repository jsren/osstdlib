STD ?= c++1z
OPT_LVL ?= Os

TARGET_STATIC := osstdc++.o
TARGET_DYNAMIC := osstdc++

FLAGS := -nostdlib -fno-exceptions -fno-rtti -$(OPT_LVL) -std=$(STD) -ffunction-sections -fdata-sections -flto

.PHONY: main-static main-dynamic default test clean

$(TARGET_STATIC):
	$(CXX) $(FLAGS) -r _abi.cpp _platform.cpp charconv.cpp cstring.cpp functional.cpp stdexcept.cpp string.cpp -o $(TARGET_STATIC)

lib$(TARGET_DYNAMIC).so:
	$(CXX) $(FLAGS) -fPIC -shared _platform.cpp charconv.cpp cstring.cpp functional.cpp stdexcept.cpp string.cpp -o lib$(TARGET_DYNAMIC).so
	$(CXX) $(FLAGS) -r _abi.cpp -o $(TARGET_STATIC)

main-static: $(TARGET_STATIC)
	$(CXX) $(FLAGS) -Wl,--gc-sections -s $(TARGET_STATIC) -I. testing/main.cpp -o testing/main

main-dynamic: lib$(TARGET_DYNAMIC).so
	$(CXX) $(FLAGS) -Wl,--gc-sections -s $(TARGET_STATIC) -I. -L. -l$(TARGET_DYNAMIC) testing/main.cpp -o testing/main

clean:
	rm -f *.so
	rm -f *.o
	rm -f testing/main

default:
	clang -std=c++14 -Wall -Werror -Wextra -pedantic -Wno-unknown-pragmas -Wno-keyword-macro -Wno-ambiguous-ellipsis -I. string.cpp iterator_test.cpp -O3 -fno-exceptions -o test
	gcc-7 -std=c++14 -Wall -Werror -Wextra -pedantic -Wno-unknown-pragmas -I. string.cpp iterator_test.cpp -O3 -fno-exceptions -o test

test:
	git submodule update --init
	g++-7 -std=c++1z -I. -Itesting/ostest *.cpp testing/*.cpp -O3 -fno-exceptions -fno-rtti testing/ostest/ostest.o -o testing/test.exe
#clang -std=c++1z -I. -Itesting/ostest *.cpp testing/*.cpp -O3 -fno-exceptions -fno-rtti testing/ostest/ostest.o -o testing/test.exe
