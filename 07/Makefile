CC=g++
FLAGS=-std=c++20 -Werror -Wall -Wextra

all: tests

tests: tests.cpp AVL-Tree.hpp
	$(CC) $(FLAGS) tests.cpp -o tests -lgtest -lgtest_main -lpthread

test: tests
	./tests

clean:
	rm -f tests tests.exe *.o

.PHONY: all test clean