CC=g++
FLAGS=-std=c++20 -Werror -Wall -Wextra

all: tests

tests: matrix.o tests.cpp
	$(CC) $(FLAGS) matrix.o tests.cpp -o tests -lgtest -lgtest_main -lpthread

test: tests
	./tests

token_parser.o: matrix.cpp matrix.hpp
	$(CC) $(FLAGS) -c matrix.cpp -o matrix.o

clean:
	rm -f tests tests.exe *.o

.PHONY: all test clean