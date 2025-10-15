CC=g++
FLAGS=-std=c++20 -Werror -Wall -Wextra

all: tests

tests: token_parser.o tests.cpp
	$(CC) $(FLAGS) token_parser.o tests.cpp -o tests -lgtest -lgtest_main -lpthread

test: tests
	./tests

token_parser.o: token_parser.cpp token_parser.hpp
	$(CC) $(FLAGS) -c token_parser.cpp -o token_parser.o

clean:
	rm -f tests tests.exe *.o

.PHONY: all test clean