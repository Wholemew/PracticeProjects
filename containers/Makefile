CC = g++

CFLAGS = -std=c++17 -Wall -Werror -Wextra -g -lgtest -lpthread -fprofile-arcs -ftest-coverage --coverage -O3 

all: test

test:
	$(CC) tests/*.cpp $(CFLAGS) -o tests.out
	valgrind --leak-check=full --log-file="val.out" ./tests.out
	mkdir -p report
	lcov --capture --directory . --output-file coverage.info
	genhtml coverage.info --output-directory report

style:
	clang-format -i */*.cpp */*.h
	clang-format -n */*.cpp */*.h

clean:
	rm tests.out
	rm val.out
	rm -rf report
	rm *.gcda
	rm *.gcno
	rm *.info