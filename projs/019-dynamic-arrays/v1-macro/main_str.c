#define DARRAY_IMPLEMENTATION
#include "libs/darray.h"

int main() {
    darray(char*) names = darray_new_string();
    darray_setup_string_ops(&names);  // Enable deep copy/free
    
    darray_push_string(&names, "Alice");
    darray_push_string(&names, "Bob");
    darray_push(&names, strdup("Charlie"));  // Or manual strdup
    
    // Automatic memory management - strings freed on darray_free
    darray_free(&names);
    return 0;
}