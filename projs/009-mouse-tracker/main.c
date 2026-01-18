#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <linux/input.h>
#include <sys/time.h>

int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        printf("USAAGE: %s <path-to-file>\n", argv[0]);
        return EXIT_FAILURE;
    }
    char *openingfile = argv[1];
    printf("intersepting %s \n", openingfile);

    int fd = open(openingfile, O_RDONLY, 0);
    if (fd < 0)
    {
        printf("ERROR OPENING FILE DISCRIPTOR \n%d\n", fd);
        return EXIT_FAILURE;
    }
    printf("Successfulluy opened the file discriptorb %d\n", fd);

    struct input_event captured_event;
    int reading = 1;
    ssize_t red; // Use ssize_t for the read() return type

    printf("Listening for events. Press Ctrl+C to exit.\n");

    while (reading)
    {
        // CORRECT: Function call syntax
        red = read(fd, &captured_event, sizeof captured_event);

        if (red == (ssize_t)-1)
        {
            perror("read error");
            break; // Exit the loop on error
        }

        if (captured_event.type == EV_KEY)
        {
            // EV_KEY events are for key presses and releases
            printf("Key event: type %u, code %u, value %u (0: release, 1: press, 2: autorepeat)\n",
                   captured_event.type, captured_event.code, captured_event.value);
            // You can use the codes defined in <linux/input-event-codes.h> for meaningful output
        }
        else if (captured_event.type == EV_REL)
        {
            // EV_REL events are for relative movements (e.g., mouse movement)
            printf("Relative event: type %u, code %u, value %u (e.g., REL_X, REL_Y)\n",
                   captured_event.type, captured_event.code, captured_event.value);
        }
        else if (captured_event.type == EV_MSC)
        {
            // EV_REL events are for relative movements (e.g., mouse movement)
            printf("Relative event: type %u, code %u, value %u (e.g., REL_X, REL_Y)\n",
                   captured_event.type, captured_event.code, captured_event.value);
        }
        else if (captured_event.type == EV_ABS)
        {
            // EV_REL events are for relative movements (e.g., mouse movement)
            printf("Relative event EV_ABS: type %u, code %u, value %u (e.g., REL_X, REL_Y)\n",
                   captured_event.type, captured_event.code, captured_event.value);
        }
        else if (captured_event.type == EV_SND)
        {
            // EV_REL events are for relative movements (e.g., mouse movement)
            printf("Relative event: type %u, code %u, value %u (e.g., REL_X, REL_Y)\n",
                   captured_event.type, captured_event.code, captured_event.value);
        }

        // EV_KEY, EV_SW, EV_LED, EV_SND

        // Use %zd for ssize_t
        printf("read resp %zd, event type %u, code %u\n",
               red, captured_event.type, captured_event.code);
        reading = 0;
    }
    // int x ;
    // char c;
    // while( read(STDIN_FILENO, &c, 1) == 1 && c != 'q'){
    //     printf("read %c\n", c);
    // }

    close(fd);
    return EXIT_SUCCESS;
}