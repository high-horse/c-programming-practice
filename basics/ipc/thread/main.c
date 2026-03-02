#include <stdio.h>
#include <pthread.h>
#include <stdbool.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include <stdlib.h>

void* myturn(void * arg) {
    int *count = (int *)arg; 
    int i = 0;
    while(i < 5) {
        sleep(1);
        printf("my turn counter = %d iter = %d\n", (*count), i);
        (*count)--;
        i++;
    }
    return "hello world from seperate thread";
}

void* yourturn(void *arg) {
    int i = 0;
    while (i<3) {
        sleep(1);
        printf("your turn\n");
        i++;
    }
}

int main() {
    pthread_t newthread;
    int count = 100;
    pthread_create(&newthread, NULL, myturn, &count);
    yourturn(NULL);
    char *msg;
    pthread_join(newthread, (void **)&msg);
    printf("count decremented result %d \n", count);
    printf("message got %s\n", msg);
}