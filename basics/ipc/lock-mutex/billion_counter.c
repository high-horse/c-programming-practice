#include <stdio.h>
#include <stdint.h>
#include <pthread.h>
#include <time.h>

#define BIG 1000000000UL
uint32_t counter = 0;

void *count_to_big(void * arg) {
    for(uint32_t i = 0; i < BIG; i++) {
        counter++;
    }
    return NULL;
}

int main() {
    pthread_t child_thread;
    pthread_create(&child_thread, NULL, count_to_big, NULL); // (ref to thread, no_idea, function, passed argument to thread)
    // count_to_big(NULL);
    count_to_big(NULL);
    pthread_join(child_thread, NULL); // (thread , return)
    printf("done , counter recved => %d\n", counter);
}