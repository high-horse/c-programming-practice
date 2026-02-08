#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <unistd.h>
#include <time.h>

// #include <linux/uinput.h>
#include <libevdev-1.0/libevdev/libevdev.h>
#include <libevdev-1.0/libevdev/libevdev-uinput.h>

int main(int argc, char *argv[])
{
    (void)argc;
    (void)argv;

    // create new fake mouse
    struct libevdev *device = libevdev_new();
    struct libevdev_uinput *uidev = NULL;

    // define mouse
    libevdev_set_name(device, "USBBBBBBBBBBBB Bruh mouse");
    libevdev_enable_event_type(device, EV_REL);
    libevdev_enable_event_code(device, EV_REL, REL_X, NULL);
    libevdev_enable_event_code(device, EV_REL, REL_Y, NULL);
    libevdev_enable_event_code(device, EV_SYN, SYN_REPORT, NULL);



    int err = libevdev_uinput_create_from_device(device, LIBEVDEV_UINPUT_OPEN_MANAGED, &uidev);
    if (err != 0)
    {
        perror("ERROR:");
        return EXIT_FAILURE;
    }


  
    // create fake mouse event
    srand(time(NULL));
    while (1)
    {
        // random -20 to 20
        // int amplitude = rand() % 40 - 20;
        int direction = rand() % 2  == 0 ?  REL_X : REL_Y;
        int amplitude = 1;
        libevdev_uinput_write_event(uidev, EV_REL, direction, amplitude);
        libevdev_uinput_write_event(uidev, EV_SYN, SYN_REPORT, 0);
        usleep(5000);

    }

    return EXIT_SUCCESS;
}