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
	@./bin/a.out /dev/input/mouse1

all:
	@make build && make run

less:
	less -f eventfile 

xxd: 	
	xxd eventfile

find:
	find /usr/include -name "input-event-codes.h"

eventcodes:
	cat /usr/include/linux/input-event-codes.h

resource :
	https://www.kernel.org/doc/Documentation/input/input.txt
