CC      := gcc
CSTD    := -std=c2x
WARN    := -Wall -Wextra -Wpedantic
DEBUG   := -g -O0
OUT     := a.out
SRC     := main.c
SDL     := $(shell pkg-config --cflags --libs sdl2)

.PHONY: all build clean run

all: build

build:
	@$(CC) $(CSTD) $(WARN) $(DEBUG) $(SRC) -o $(OUT) $(SDL)

do: 
	@make build && cat image.ppm | ./a.out 

run: 
	@./$(OUT)

clean:
	rm -f $(OUT)


convert-p3:
	convert images.jpeg -compress none image.ppm

convert-p6:
	convert original-images.jpeg -define ppm:format=binary image.ppm

