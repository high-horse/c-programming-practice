CC = gcc

CFLAGS = -Wall -Wextra -Werror

build:
	$(CC) -o bin/pie-chart src/main.c -lraylib $(CFLAGS) 
	
run:
	./bin/pie-chart
	
clean:
	rm -f bin/pie-chart
	
all: build run


.PHONY=build run clean
