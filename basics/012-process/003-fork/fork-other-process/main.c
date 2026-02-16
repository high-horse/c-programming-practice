#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main() {
    int a = 0;
    if(fork() == 0) {
        a += 5;
        printf("%d , %p \n", a, &a);
    } else {
        a-= 5;
        printf("%d , %p \n", a, &a);
    }


}