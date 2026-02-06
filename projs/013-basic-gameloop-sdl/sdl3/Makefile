.PHONY: build

build:
	@echo "building ..."
	gcc -std=c2x -Wall -Wextra src/* -o bin/a.out -lSDL3
	@echo "built complete"


run:
	@./bin/a.out
	@echo "---------COMPLETED-------"

clean: rm -rf ./bin/
