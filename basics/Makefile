.PHONY: build run

# Default target
run: build
	./main

build:
	gcc -std=c2x main.c -o main

hex:
	hexdump -C txt/bin/output.bin

ptr:
	gcc -std=c2x 008_06_pointer_arithmetic_star_star_main.c -o ptr.out && ./ptr.out
