COMP = gcc

SRC = tetris.c Engine/library.c Gui/tetrisGUI.c
HEAD = tetris.c Engine/library.h Gui/tetrisGUI.h
TEST = Tests/libTests.c Engine/library.c -lcheck -lsubunit -lrt -lpthread -lm

FLAGS = -Wall -Werror -Wextra -lncurses -g

all: install

Tetris/:
	mkdir Tetris

dist:
	tar -czf tetris.tar.gz Engine/ Gui/ Tests/ tetris.c tetris.h Makefile

dvi:
	latex Documentation.tex
	rm Documentation.aux Documentation.log Documentation.toc

install: Tetris/
	$(COMP) $(SRC) $(FLAGS) -o Tetris/Tetris.exe

uninstall: Tetris/
	rm -rf Tetris/

test: Tetris/
	$(COMP) $(FLAGS) $(TEST) -o Tetris/unit_test.out
	Tetris/unit_test.out

gcov_report: Tetris/
	$(COMP) -fprofile-arcs -ftest-coverage --coverage $(TEST) -o Tetris/test.out
	Tetris/test.out
	mkdir -p report
	lcov --capture --directory . --output-file coverage.info
	genhtml coverage.info --output-directory report
	rm *.gcda
	rm *.gcno
	rm Score.dat
	rm coverage.info

clean: Tetris/
	rm -rf Tetris

style:
	clang-format -i $(SRC) Tests/* $(HEAD)
	clang-format -n $(SRC) Tests/* $(HEAD)
