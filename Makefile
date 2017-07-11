default:
	clang -std=c++14 -Wall -Werror -Wextra -pedantic -Wno-unknown-pragmas -Wno-keyword-macro -Wno-ambiguous-ellipsis -I. string.cpp iterator_test.cpp -O3 -fno-exceptions -o test
	gcc-7 -std=c++14 -Wall -Werror -Wextra -pedantic -Wno-unknown-pragmas -I. string.cpp iterator_test.cpp -O3 -fno-exceptions -o test
