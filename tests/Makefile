CC := gcc
lib := ../lib/bin/libcypher_io.o
FLAGS := -Wall -Wextra -O3

libcypher_io: libcypher_io/build/test

libcypher_io/build/test: libcypher_io/bin/test.o $(lib)
	@$(CC) $(FLAGS) $^ -o $@

libcypher_io/bin/test.o: libcypher_io/src/test.c
	@$(CC) $(FLAGS) -c $< -o $@