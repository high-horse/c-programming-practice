.PHONY: build run

# Default target
run: build
	./main

build:
	gcc -std=c2x main.c -o main

hex:
	hexdump -C txt/bin/output.bin