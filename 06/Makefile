CC=g++
FLAGS=-std=c++20 -Werror -Wall -Wextra

all: tests

tests: formatting.o tests.cpp
	$(CC) $(FLAGS) formatting.o tests.cpp -o tests -lgtest -lgtest_main -lpthread

test: tests
	./tests

formatting.o: formatting.cpp formatting.hpp
	$(CC) $(FLAGS) -c formatting.cpp -o formatting.o

clean:
	rm -f tests tests.exe *.o

.PHONY: all test clean