CC = gcc
FLAGS = -std=c2x -Wall -Wextra 
FILES = src/main.c
OUTPUT = bin/a.out
LIBS = -lncurses

all: $(OUTPUT)

$(OUTPUT): $(FILES)
	$(CC) $(FLAGS) $(FILES) -o $(OUTPUT) $(LIBS)
	
run: $(OUTPUT)
	@./$(OUTPUT) $(args)

clean:
	rm -f $(OUTPUT)

