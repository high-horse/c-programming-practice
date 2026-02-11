#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <libevdev-1.0/libevdev/libevdev.h>
#include <libevdev-1.0/libevdev/libevdev-uinput.h>

void set_events(void *pdev){
    struct libevdev *dev = pdev;
    libevdev_enable_event_type(dev, EV_KEY);

    libevdev_enable_event_type(dev, KEY_ESC);
    // TODO: handle remaingin events
}

int main(int argc, char *argv[])
{
    (void)(argc);
    (void)argv;

    int err;
    struct libevdev *dev;
    struct libevdev_uinput *uidev;

    dev = libevdev_new();
    libevdev_set_name(dev, "MY FUNNY KEYBOARD");

    err = libevdev_uinput_create_from_device(dev, LIBEVDEV_UINPUT_OPEN_MANAGED, &uidev);
    if (err < 0)
    {
        perror("Error::");
        return EXIT_FAILURE;
    }



    return EXIT_SUCCESS;
}

