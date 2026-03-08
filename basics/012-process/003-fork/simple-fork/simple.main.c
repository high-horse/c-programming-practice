#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main() {

    int parent_pid = getpid();
    fork();
    fork();
    fork();
    fork();

    printf("parent PID => %d \n and child forked %d\n", parent_pid, getpid());
}