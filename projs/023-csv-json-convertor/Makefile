.PHONY: build run clean

build:
	@echo "building..."
	gcc -std=c2x -Wall  src/main.c src/libs/* -o bin/parser.out
	@echo "build complete"
	@echo  "===running==="

run: build
	@./bin/parser.out sample/simple.csv

clean:
	rm -f bin/parser.out
	
all: build, run
