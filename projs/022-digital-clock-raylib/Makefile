do:
	gcc -std=c23 -Wall -Wextra  src/*.c src/libs/* -o bin/a.out -lraylib && ./bin/a.out

build-static-linux:
	gcc -std=c23 -Wall -Wextra -Werror \
	-o bin/clock \
	src/main.c src/libs/*  \
	-Wl,-Bstatic -lraylib -Wl,-Bdynamic \
	-lm -ldl -lpthread -lrt -lX11 -lGL
	
build-linux:
	gcc -std=c2x -Wall -Wextra -Werror \
	-o bin/clock src/main.c src/libs/* \
	-lraylib -lm -ldl -lpthread -lrt -lX11 -lGL
