CC      := gcc
CSTD    := -std=c2x
WARN    := -Wall -Wextra -Wpedantic
DEBUG   := -g -O0
OUT     := bin/a.out
SRC     := main.c



# build:
	# $(CC) kilo.c -o kilo -Wall -Wextra -pedantic -std=
build:
	@$(CC) $(CSTD) $(WARN) $(DEBUG) $(SRC) -o $(OUT) 

run:
	@./bin/a.out

all:
	@make build && make run