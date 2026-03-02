#include <stdio.h>
#include <stdint.h>
#include <pthread.h>
#include <time.h>

#define BIG 1000000000UL
uint32_t counter = 0;
pthread_mutex_t counter_mutex = PTHREAD_MUTEX_INITIALIZER;

void *count_to_big(void * arg) {
    for(uint32_t i = 0; i < BIG; i++) {
        pthread_mutex_lock(&counter_mutex);
        counter++;
        pthread_mutex_unlock(&counter_mutex);
    }
    return NULL;
}

int main() {
    pthread_t child_thread;
    pthread_create(&child_thread, NULL, count_to_big, NULL); // (ref_to_thread, no_idea, function_execute, passed argument to thread)
    // count_to_big(NULL);
    count_to_big(NULL);
    pthread_join(child_thread, NULL); // (thread , return)
    printf("done , counter recved => %d\n", counter);
}