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
	$(CC) $(CSTD) $(WARN) $(DEBUG) $(SRC) -o $(OUT) $(SDL)

run: build
	./$(OUT)

clean:
	rm -f $(OUT)
