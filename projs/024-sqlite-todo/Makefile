CC = gcc
CFLAGS = -Wall -Wextra 
LIBS = -lsqlite3
OUT = bin/todo.out
FILES = src/main.c src/libs/*

all: build run

build:
	@echo "building ..."
	@$(CC) $(CFLAGS) $(FILES) -o $(OUT) $(LIBS)
	@echo "=======RUNNING======= "

run: build
	@./$(OUT)



.PHONY: all build run