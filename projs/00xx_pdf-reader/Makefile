.PHONY: build run

# Default target
run: build
	./main

build:
	gcc -std=c2x main.c -o main

hex:
	hexdump -C txt/bin/output.bin

compress:
	qpdf --qdf --object-streams=disable sample.pdf decompressed-sample.pdf

build-com:
	gcc main.c -o main  $(pkg-config --cflags --libs poppler-glib)

run-com: 
	./main /home/camel/Desktop/office-documents/decompressed-sample.pdf