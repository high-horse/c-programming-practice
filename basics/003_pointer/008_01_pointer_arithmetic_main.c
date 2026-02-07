#include <stdio.h>

int my_strlen(char *s);

int main(void)
{
    
}

void les_1(void)
{

    int arr[] = {10, 20, 30, 40, 50};
    int *p = arr;

    printf("%d\n", *(p));
    printf("%d\n", *(p + 0));

    // adds to index pointer.
    // adds sizeof(int) to the p, since it is []int.
    printf("%d\n", *(p + 1));
}

void les_2(void)
{
    int arr[] = {10, 20, 30, 40, 50, 60, 70, 80, 80, 999};
    int *p = &arr[0];

    while (*p != 999)
    {
        printf("%d\n", *p);
        p++;
    }
    printf("%d\n", *p);
}

void les_3(void)
{
    char *name = "John Doe";
    printf("%d\n", my_strlen("Hello, world!"));
}
int my_strlen(char *s)
{
    char *p = s;
    while (*p != '\0')
    {
        p++;
    }
    return p - s;
}

void les_4(void)
{

    int arr[10] = {10, 20, 30, 40, 50, 60, 70, 80, 80, 999};
    int *p = arr;

    printf("Array index notation \n");
    for (int i = 0; i < 10; i++)
    {
        printf("%d\t", arr[i]);
    }
    printf("\npointer index notation\n");
    for (int i = 0; i < 10; i++)
    {
        printf("%d\t", p[i]);
    }
    printf("\narray pointer add notation\n");
    for (int i = 0; i < 10; i++)
    {
        printf("%d\t", *(arr + i));
    }
    printf("\npointer add notation \n");
    for (int i = 0; i < 10; i++)
    {
        printf("%d\t", *(p + i));
    }
    printf("\nmove pointer notation\n");
    for (int i = 0; i < 10; i++)
    {
        printf("%d\t", *(p++));
    }
}