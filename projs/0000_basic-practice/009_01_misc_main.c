//
// Created by camel on 9/14/25.
//
#include <stdio.h>

void print_str(char msg[]){
    int x = 0;
    while (msg[x] != '\0') {
        putchar(msg[x]);
        x++;
    }
    putchar('\n');
}

int main(void) {
    char *msg = "some chuccho jaat wala";
    print_str(msg);

}
void add(const int *arr, int len) {

}

// void les_1(void) {
//     const int i = 100;
//     printf("%d\n", i);
//     int *ip = &i;
//     *ip = 101;
//     printf("%d\n", i);
// }

void les_0 () {
    int n[] = {10,20};
    const int *p = n;
    p++;
    // *p = 30;
}