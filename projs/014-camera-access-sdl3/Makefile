.PHONY: build

build:
	@echo "building ..."
	gcc -std=c2x -Wall -Wextra src/main.c -o bin/a.out -lSDL3
	@echo "built complete"

static:
	gcc -std=c2x -Wall -Wextra src/main.c  \
	-lSDL3 \
	-Wl,-Bstatic -lSDL3 \
	-Wl,-Bdynamic \
	-o app



run:
	@./bin/a.out
	@echo "---------COMPLETED-------"

clean: rm -rf ./bin/
