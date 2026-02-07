#ifndef TEMP_KEY_EVENTS
#define TEMP_KEY_EVENTS

#define BUFFER_SIZE 24
#define true 1
#define false 0
#include <linux/input.h>

void get_keyboard_codes(struct input_event *p_event, char *k_code);

#endif