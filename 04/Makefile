CC=g++
FLAGS=-std=c++20 -Werror -Wall -Wextra

all: tests

tests: big_numbers.o tests.cpp
	$(CC) $(FLAGS) big_numbers.o tests.cpp -o tests -lgtest -lgtest_main -lpthread

test: tests
	./tests

big_numbers.o: big_numbers.cpp big_numbers.hpp
	$(CC) $(FLAGS) -c big_numbers.cpp -o big_numbers.o

clean:
	rm -f tests tests.exe *.o

.PHONY: all test clean