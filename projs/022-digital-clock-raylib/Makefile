do:
	gcc -std=c23 -Wall -Wextra  src/*.c -o bin/a.out -lraylib && ./bin/a.out

build-static-linux:
	gcc -std=c23 -Wall -Wextra -Werror \
	-o bin/clock \
	src/main.c  \
	-Wl,-Bstatic -lraylib -Wl,-Bdynamic \
	-lm -ldl -lpthread -lrt -lX11 -lGL
	
build-linux:
	gcc -std=c23 -Wall -Wextra -Werror \
	-o bin/clock src/main.c \
	-lraylib -lm -ldl -lpthread -lrt -lX11 -lGL