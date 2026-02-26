# Define the compiler and flags
CC = gcc
CFLAGS = -Wall -Wextra -std=c2x
# Fetch flags and libs using pkg-config
FFMPEG_LIBS = $(shell pkg-config --cflags --libs libavformat libavcodec libavutil libswscale)

build:
	mkdir -p bin
	$(CC) $(CFLAGS) src/*.c -o bin/a.out $(FFMPEG_LIBS)

clean:
	rm -rf bin/a.out