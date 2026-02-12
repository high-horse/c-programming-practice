#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdbool.h>

int main() {

    int current_pid = getpid();
    printf("current process id is %d\n", current_pid);

    while (true)
    {
        sleep(10);
    }
    
    return EXIT_SUCCESS;
}