#include <stdio.h>
#define DARRAY_IMPLEMENTATION
#include "libs/darray.h"

int main() {
    darray(int) numbers = darray_new(int);
    
    darray_push(&numbers, 10);
    darray_push(&numbers, 20);
    darray_push(&numbers, 30);
    
    int* first = darray_front(&numbers);  // Pointer to 10
    int val = darray_pop(&numbers);       // Removes and returns 30
    
    darray_foreach(darray(int)*, &numbers, int*, num) {
        printf("%d\n", *num);
    }
    
    darray_free(&numbers);
    return 0;
}