do:
	gcc -std=c2x -Wall -Wextra  src/*.c src/libs/* -o bin/dev/clock -lraylib && ./bin/dev/clock

build-static-linux:
	gcc -std=c2x -Wall -Wextra -Werror \
	-o bin/static/clock \
	src/main.c src/libs/*  \
	-Wl,-Bstatic -lraylib -Wl,-Bdynamic \
	-lm -ldl -lpthread -lrt -lX11 -lGL
	
build-linux:
	gcc -std=c2x -Wall -Wextra -Werror \
	-o bin/linux/clock src/main.c src/libs/* \
	-lraylib -lm -ldl -lpthread -lrt -lX11 -lGL
