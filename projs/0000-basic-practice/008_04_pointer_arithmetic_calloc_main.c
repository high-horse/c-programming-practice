#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(void)
{
    // calloc initiates to 0 for int by default.
    int *p = calloc(10, sizeof(10));
    char *c = calloc(10, sizeof(char));
    for (int i = 0; i < 10; i++)
    {
        printf("%d =>'%d' \t", i, p[i]);
    }
    printf("\n");
    
    for (int i = 0; i < 10; i++)
    {
        printf("%d =>'%c' \t", i, c[i]); //
    }
    printf("\n");
    
}

void les_1(void)
{
    int *p = malloc(sizeof(int) * 10);
    int *q = malloc(sizeof(int) * 10);

    // sets 1st to 1, else garbage
    memset(p, 0, sizeof(int));

    for (int i = 0; i < 10; i++)
    {
        printf("%d => %d |", i, p[i]);
    }
    printf("\n");

    // sets all 10 elements to default value of 1
    memset(q, 0, 10 * sizeof(int));
    for (int i = 0; i < 10; i++)
    {
        printf("%d => %d |", i, q[i]);
    }
    printf("\n");
}