#include <stdio.h>

void increment(int *i);

int main(void)
{
}

void size_of_pointer() {
    int i = 10;
    int *p = &i;
    printf("%zu\n", sizeof(i)); // is int so 4 byte; returns 4 byte 
    printf("%zu\n", sizeof(p)); // is type address; returns 8 byte
    printf("%zu\n", sizeof(*p)); // is value of i so returns 4 
    printf("%zu\n", sizeof(&i)); // address of i so returns 8
}

void increment_through_pointer()
{
    int i = 10;
    // int *j = &i;
    int *j;
    j = &i;

    printf("default value of i = %d and de-pointered j = %d\n", i, *j);
    increment(j);
    printf("incremented value of i = %d and de-pointered j = %d\n", i, *j);
    increment(&i);
    printf("incremented value of i = %d and de-pointered j = %d\n", i, *j);
}

void increment(int *i)
{
    *i = *i + 1;
}