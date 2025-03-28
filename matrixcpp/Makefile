CC = g++

CFLAGS = -Wall -Werror -Wextra -std=c++17 -g -O0
SRC = s21_matrix_oop.cpp

all: s21_matrix_oop.a test

s21_matrix_oop.a: $(SRC)
	$(CC) $(CFLAGS) -c $(SRC)
	ar rcs $@ *.o
	ranlib $@
	rm *.o

clean:
	rm s21_matrix_oop.a
	rm -rf build

test: s21_matrix_oop.cpp
	cmake -S . -B build
	cmake --build build
	cd build && ctest

style:
	clang-format -i *.cpp *.h
	clang-format -n *.cpp *.h
