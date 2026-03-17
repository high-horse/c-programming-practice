#include <stdio.h>
#define DARRAY_IMPLEMENTATION
#include "libs/darray.h"

int main() {
    darray names = darray_create_string();  /* Automatically handles strdup/free */
    
    darray_push_string(&names, "Alice");
    darray_push_string(&names, "Bob");
    
    char** name = darray_get(&names, 0);  /* Returns char** */
    printf("%s\n", *name);  /* Prints "Alice" */
    
    darray_destroy(&names);  /* All strings freed automatically */
}