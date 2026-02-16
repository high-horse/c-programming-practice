#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <unistd.h>

int main() {

    int parent_pid = getpid();
    printf("My current pid is %d \n", parent_pid);
    sleep(1);

    int child_pid = fork();
    if(child_pid != parent_pid) {

        printf("logging from child_pid => expected child Pid is %d  and actual is %d\n", child_pid, getpid());
    }
    sleep(10);
}