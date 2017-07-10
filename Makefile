default:
	clang -std=c++14 -Wall -Wextra -Werror -pedantic -Wno-unknown-pragmas -Wno-keyword-macro -Wno-ambiguous-ellipsis -I. iterator_test.cpp -O3 -fno-exceptions -o test
	gcc-7 -std=c++14 -Wall -Wextra -Werror -pedantic -Wno-unknown-pragmas -Wno-keyword-macro -Wno-ambiguous-ellipsis -I. iterator_test.cpp -O3 -fno-exceptions -o test
