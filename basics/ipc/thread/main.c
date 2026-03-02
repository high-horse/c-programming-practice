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
    int i = 0;
    while (i<5) {
        sleep(1);
        printf("your turn\n");
        i++;
    }
}

int main() {
    pthread_t newthread;
    pthread_create(&newthread, NULL, myturn, NULL);
    yourturn(NULL);
    pthread_join(&newthread, NULL);
}