#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <stdbool.h>


void *wait_fn(void *arg) {
    int pid = getpid();
    printf("thread process  id -> %d\n", pid);
    sleep(5);
    printf("DONE\n");
    return NULL;
}


int main() {
    pthread_t thread;
    int processid = getpid();
    printf("process id is %d\n", processid);

    int err = pthread_create(&thread, NULL, wait_fn, NULL);
    if(err != false) {
        perror("FAILED TO CREATE THREAD::\n");
        return EXIT_FAILURE;
    }
    printf("waiting for thread to end\n");
    pthread_join(thread, NULL);
    printf("Success\n");
    

    return EXIT_SUCCESS;
}