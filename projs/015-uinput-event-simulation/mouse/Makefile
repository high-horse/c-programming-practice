.PHONY: build

build:
	gcc -std=c2x  -static -Wall -Wextra src/* $(shell pkg-config --cflags --libs libevdev) -o bin/a.out 

run:
	bin/a.out

test:
	evtest
