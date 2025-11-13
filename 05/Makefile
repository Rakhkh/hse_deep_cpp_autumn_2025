CC=g++
FLAGS=-std=c++20 -Werror -Wall -Wextra

all: tests

tests: serializer.o tests.cpp
	$(CC) $(FLAGS) serializer.o tests.cpp -o tests -lgtest -lgtest_main -lpthread

test: tests
	./tests

serializer.o: serializer.cpp serializer.hpp
	$(CC) $(FLAGS) -c serializer.cpp -o serializer.o

clean:
	rm -f tests tests.exe *.o

.PHONY: all test clean