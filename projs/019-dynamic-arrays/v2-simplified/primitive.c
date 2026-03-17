#include <stdio.h>
#define DARRAY_IMPLEMENTATION
#include "libs/darray.h"

int main() {
    darray nums = darray_create(sizeof(int));
    
    int x = 10;
    darray_push(&nums, &x);
    
    x = 20;
    darray_push(&nums, &x);
    
    /* Access */
    int* first = darray_front(&nums);  /* Returns int* */
    int* val = darray_get(&nums, 1);   /* Returns int* */
    
    /* Iterate */
    for (size_t i = 0; i < darray_size(&nums); i++) {
        int* n = darray_get(&nums, i);
        printf("%d\n", *n);
    }
    
    darray_destroy(&nums);
}