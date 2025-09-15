//
// Created by camel on 9/14/25.
//
#include <stdio.h>

int main(void) {
    int n[] = {10,20};
    const int *p = n;
    p++;
    // *p = 30;
}

void les_1(void) {
    const int i = 100;
    printf("%d\n", i);
    int *ip = &i;
    *ip = 101;
    printf("%d\n", i);
}
