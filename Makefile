CC := gcc
lib := lib/bin/libcypher_io.o
FLAGS := -Wall -Wextra -O3

####################################
#
# Base64 program
#
####################################

Base64: Base64/build/main

Base64/build/main: Base64/bin/main.o $(lib)
	@$(CC) $(FLAGS) $^ -o $@

Base64/bin/main.o: Base64/src/main.c
	@$(CC) $(FLAGS) -c $< -o $@

####################################
#
# CLI program
#
####################################

CLI: CLI/build/main

CLI/build/main: CLI/bin/main.o $(lib)
	@$(CC) $(FLAGS) $^ -o $@

CLI/bin/main.o: CLI/src/main.c
	@$(CC) $(FLAGS) -c $< -o $@