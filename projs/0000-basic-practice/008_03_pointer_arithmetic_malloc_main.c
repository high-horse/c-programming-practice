#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int main(void)
{
    int *p = malloc(sizeof(int) * 10);
    int *q = malloc(sizeof(int) * 10);

    // sets 1st to 1, else garbage
    memset(p, 1, sizeof(int));

    for (int i = 0; i < 10; i++)
    {
        printf("%d => %d |", i, p[i]);
    }
    printf("\n");

    // sets all 10 elements to default value of 1
    for (int i = 0; i < 10; i++)
    {
        printf("%d => %d |", i, q[i]);
    }
    printf("\n");
}

void les_1(void)
{
    // int* p = malloc(sizeof(int)); // same as below.
    int *p = malloc(sizeof(*p)); // same as above
    *p = 100;
    printf("%d\n", *p);
    free(p);

    // use after free.
    // p = NULL;
    // *p=101;
    // printf("%d\n", *p);
}

int les_2(void)
{
    int *x;
    x = malloc(sizeof(int) * 10);
    if (x == NULL)
    {
        printf("Error allocating 10 ints\n");
        return 1;
    }
    free(x);
    return 0;
}

void les_3(void)
{
    int *x;
    if ((x = malloc(sizeof(int) * 10)) == NULL)
    {
        printf("Error allocating 10 ints\n");
    }

    // allocate array with malloc;
    char c[10] = malloc(sizeof(char) * 10);
    if (c == NULL)
    {
        printf("Failed to allocate array of char \n");
        return 1;
    }
}