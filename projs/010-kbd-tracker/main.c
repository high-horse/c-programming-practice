#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <linux/input.h>


#define BUFFER_SIZE 24
#define true 1
#define false 0

int main(int argc, char *argv[]) {
    if (argc != 2) {
        printf("Usage: %s <keyboard-event-file>\n", argv[0]);
        return EXIT_FAILURE;
    }

    const char *filename = argv[1];
    int fp = open(filename, O_RDONLY | O_NONBLOCK);
    if(fp < 0) {
        printf("Failed to open file discriptor : %s \n", filename);
        return EXIT_FAILURE;
    }

    struct input_event ev ;
    int running = true;
    while (running) {
        read(fp, &ev, sizeof ev);
        if(ev.type == EV_KEY) {
            continue;
        }

        printf("Event: time %ld.%06ld, type %d, code %d, value %d\n",
               ev.time.tv_sec, ev.time.tv_usec, ev.type, ev.code, ev.value);
    }

    close(fp);




    return EXIT_SUCCESS;
}