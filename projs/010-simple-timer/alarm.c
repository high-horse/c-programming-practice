#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/time.h>
#include <signal.h>

#define true 1
#define false 0
#define INTERVAL_MS 1

void handler(int sigalrm) {
    static int count = 1;
    printf("alarm triggered count => %d and sigalrm is %d \n", count, sigalrm);
    count += 1;
}

int main(int argc, char *argv[]) {
    int interval = INTERVAL_MS;
    if(argc == 2) {
        interval = atoi(argv[1]);
    }
    printf("interval ms is %d \n", interval);
    printf("setting alarm \n");
    
    while(true) {
        signal(SIGALRM, handler);
        alarm(interval);
        pause();
    }
    return EXIT_SUCCESS;

    
}