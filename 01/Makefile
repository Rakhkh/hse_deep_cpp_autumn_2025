CC=g++
FLAGS=-std=c++20 -Werror -Wall -Wextra

all: tests

tests: allocator.o tests.cpp
	$(CC) $(FLAGS) allocator.o tests.cpp -o tests -lgtest -lgtest_main -lpthread

test: tests
	./tests

allocator.o: allocator.cpp
	$(CC) $(FLAGS) -c allocator.cpp -o allocator.o

clean:
	rm -r -f tests *.o

.PHONY: all test clean