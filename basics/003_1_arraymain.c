#include <stdio.h>
#define COUNT  10

void les_1(void);
void les_2(void);
void les_3(void);


int main(void)
{
    int a[COUNT] = {[COUNT-1] = 10000, 22,37,55};
    for (int i = 0; i < COUNT; i++)
    {
        printf("value => %d \tindex => %d\n", a[i], i);
    }
    
}

void les_1(void)
{
    float f[4];

    f[0] = 1.1;
    f[1] = 2.1;
    f[2] = 3.1,
    f[3] = 4.1;

    for (int i = 0; i < 4; i++)
    {
        printf("%f\n", f[i]);
    }
}

// void les_2(void) {
//     void calculate(int x[12])
//     {
//         printf("size of whole %zu\n", sizeof(x));
//         printf("size of unit type %zu\n", sizeof(int));
//         printf("length of array %zu\n", sizeof(x) / sizeof(int));
//     }

//     int x[12];

//     printf("size of whole %zu\n", sizeof(x));
//     printf("size of unit type %zu\n", sizeof(int));
//     printf("length of array %zu\n", sizeof(x) / sizeof(int));

//     calculate(x);
// }

void les_3(void)
{
    int a[5] = {1, 2, 3, 4, 5};
    int b[5] = {1, 2, 3}; // initializes the b[4] = 0, b[5] = 0
    int c[5] = {
        1, 2, 3, 4, 5,
        //  6
    }; // throws error : excess elements in array initializer  c[5]

    for (int i = 0; i < 5; i++)
    {
        printf("value  a => %d, b => %d, b => %d | index => %d\n", a[i], b[i], c[i], i);
    }
}

void les_4(void)
{
    int a[10] = {0};
    for (int i = 0; i < 10; i++)
    {
        printf("value => %d \tindex => %d \n", a[1], i);
    }

    int arr[10] = {0, 1, 2, [9] = 9};
    for (int i = 0; i < 10; i++)
    {
        printf("value => %d \tindex => %d\n", arr[i], i);
    }
}


void func3d(void)
{
    int a[2][5] = {
        {1, 4, 1, 5, 3},
        {5, 4, 2, 5, 5}};

    int count = 5;
    for (int i = 0; i < 2; i++)
    {
        for (int j = 0; j < count; j++)
        {
            printf("(%d, %d) -> %d \n", i, j, a[i][j]);
        }
    }
}

void funcMatrix(void)
{
    int arr[3][3] = {[0][0] = 1, [1][1] = 1, [2][2] = 1};
    int count = 3;
    for (int i = 0; i < count; i++)
    {
        for (int j = 0; j < count; j++)
        {
            printf("%d\t", arr[i][j]);
        }
        printf("\n");
    }
}


void assignment(void)
{
    int a[] = {11, 12, 14};

    int *q;
    int *nice;

    int *p = &a[0]; // same as below assignemnts
    q = &a[0];      // same as above assignment
    nice = a;       // same as above assignment

    printf("%d\t%d\t%d\n", *p, *q, *nice);
}