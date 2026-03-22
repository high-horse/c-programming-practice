#include <stdio.h>
#include "cstring.h"

int main() {
    CString first = new_cstring("Hellot");
    CString second = new_cstring("hello");
    CString third = new_cstring("");
    
    printf("first %s \t second %s\n", first.str, second.str);
    printf("is empty => %d\n", cstring_isempty(&third));
 
    printf("compares => %d\n", cstring_compare(&first, &second));
    printf("equals => %d\n", cstring_equals(&first, &second));
    printf("equals ignore case => %d\n", cstring_equals_ignore_case(&first, &second));
    printf("cstring_find => %d\n", cstring_indexof(&first, "t"));
    printf("contains => %d \n", cstring_contains(&first, "Hello"));
    
    printf("cstring_starts_with => %d\n", cstring_starts_with(&first, "H"));
    printf("cstring_ends_with => %d\n", cstring_ends_with(&first, "t"));
    
}