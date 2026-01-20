#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>
#include <linux/input.h>
#include <sys/time.h>
#include <libevdev-1.0/libevdev/libevdev.h>
#include <errno.h>

#include "local_events.h"

#define true 1
#define false 0

void get_event_type(struct input_event *p_event, char *type)
{
    switch (p_event->type)
    {
    case 0:
        strcpy(type, "EV_SYN");
        break;

    case 1:
        strcpy(type, "EV_KEY");
        break;

    case 2:
        strcpy(type, "EV_REL");
        break;

    case 3:
        strcpy(type, "EV_ABS");
        break;

    case 4:
        strcpy(type, "EV_MSC");
        break;

    case 5:
        strcpy(type, "EV_SW");
        break;

    default:
        strcpy(type, "other");
        break;
    }
}


void get_code(struct input_event *p_event, char *code)
{
    switch (p_event->code)
    {
    case REL_X:
        strcpy(code, "REL_X");
        break;

    case REL_Y:
        strcpy(code, "REL_Y");
        break;

    case REL_Z:
        strcpy(code, "REL_Z");
        break;

    case REL_RX:
        strcpy(code, "REL_RX");
        break;

    case REL_RY:
        strcpy(code, "REL_RY");
        break;

    case REL_RZ:
        strcpy(code, "REL_RZ");
        break;

    case REL_HWHEEL:
        strcpy(code, "REL_HWHEEL");
        break;

    case REL_DIAL:
        strcpy(code, "REL_DIAL");
        break;

    case REL_WHEEL:
        strcpy(code, "REL_WHEEL");
        break;

    case REL_MISC:
        strcpy(code, "REL_MISC");
        break;


    // Handling keyboard events.


    default:
        strcpy(code, "others");
        break;
    }
}


int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        printf("Usage: %s, <FILE_NAME>\n", argv[0]);
        return EXIT_FAILURE;
    }
    char *opening_file = argv[1];
    printf("Device %s is being intersepted \n", opening_file);

    int fd = open(opening_file, O_RDONLY);
    if (fd == -1)
    {
        printf("Failed to open file discriptor.\n");
        return EXIT_FAILURE;
    }
    printf("FIle Discriptor read is %d\n", fd);

    struct input_event i_event;

    while (true)
    {
        read(fd, &i_event, sizeof i_event);
        struct timeval time = i_event.time;
        char ev_type[20];
        get_event_type(&i_event, ev_type);

        if(i_event.type == EV_KEY) { // Keyboard events
            if(i_event.code > KEY_MICMUTE) {
                continue;
            }
            char code[20];
            get_keyboard_codes(&i_event, code);
            printf("Keyboard event intersepted\n");
            printf("EVENT @t=%lds %ldus\n", time.tv_sec, time.tv_usec);
            printf("Event type %d => %s, code %d => %s\n", i_event.type, ev_type, i_event.code, code);
            printf("value  %d \n", i_event.value);
        }
        else if(i_event.type == EV_ABS) { // Mouse events
            // char code[20];
            // get_code(&i_event, code);
            // printf("Mouse Event intersepted\n");
            // printf("EVENT @t=%lds %ldus\n", time.tv_sec, time.tv_usec);
            // printf("Event type %d => %s, code %d => %s\n", i_event.type, ev_type, i_event.code, code);
            // printf("value  %d \n", i_event.value);
        }
        else {
            // printf("------\n");
            // printf("OTHER Event intersepted\n");
            // printf("EVENT @t=%lds %ldus\n", time.tv_sec, time.tv_usec);
            // printf("Event type %d , code %d \n", i_event.type, i_event.code);
            // printf("------\n");

        }

    }
}

int main_old(int argc, char *argv[])
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
    int reading = true;
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
        reading = false;
    }
    // int x ;
    // char c;
    // while( read(STDIN_FILENO, &c, 1) == 1 && c != 'q'){
    //     printf("read %c\n", c);
    // }

    close(fd);
    return EXIT_SUCCESS;
}
