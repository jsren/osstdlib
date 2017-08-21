default:
	clang -std=c++14 -Wall -Werror -Wextra -pedantic -Wno-unknown-pragmas -Wno-keyword-macro -Wno-ambiguous-ellipsis -I. string.cpp iterator_test.cpp -O3 -fno-exceptions -o test
	gcc-7 -std=c++14 -Wall -Werror -Wextra -pedantic -Wno-unknown-pragmas -I. string.cpp iterator_test.cpp -O3 -fno-exceptions -o test

.PHONY: test

test:
	git submodule update --init
	g++-7 -std=c++14 -I. -Itesting/ostest *.cpp testing/*.cpp -O3 -fno-exceptions -fno-rtti testing/ostest/ostest.o -o testing/test.exe
	clang -std=c++14 -I. -Itesting/ostest *.cpp testing/*.cpp -O3 -fno-exceptions -fno-rtti testing/ostest/ostest.o -o testing/test.exe
