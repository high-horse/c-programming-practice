CC = gcc
CSTD = c2x
CFLAGS = -std=$(CSTD) -Wall -Wextra -Werror

TARGET = bin/main
SRC = src/main.c src/libs/*
BIN_DIR = bin

all: $(TARGET)

$(TARGET): $(SRC) | $(BIN_DIR)
	$(CC) $(CFLAGS) -o $(TARGET) $(SRC)  -lraylib

$(BIN_DIR):
	mkdir -p $(BIN_DIR)

run: all
	@./$(TARGET) $(ARGS)

clean:
	rm -f $(TARGET)

re: clean all

.PHONY: all clean run re