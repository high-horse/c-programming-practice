#include <stdio.h>
#include <pthread.h>
#include <stdbool.h>
#include <time.h>
#include <unistd.h>

void* myturn(void * arg) {
    while(true) {
        sleep(1);
        printf("my turn\n");
    }
    return NULL;
}

void* yourturn(void *arg) {
    while (true) {
        sleep(1);
        printf("your turn\n");
    }
}

int main() {
    pthread_t newthread;
    pthread_create(&newthread, NULL, myturn, NULL);
    yourturn(NULL);
}