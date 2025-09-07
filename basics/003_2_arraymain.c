#include <stdio.h>


void times1(int *a, int len);
void times2(int a[], int len);
void times3(int a[5], int len);

void array_double(int *a, int len);

void print_2D_arr(int a[3][3]);
int main(void)
{   
    int matrix[3][3] = {[0][0]=1, [1][1]=1, [2][2]=1};
    print_2D_arr(matrix);
}
void print_2D_arr(int a[3][3]){
    for (int i = 0; i < 3; i++)
    {
        for (int j = 0; j < 3; j++)
        {
            printf("%d \t", a[i][j]);
        }
        printf("\n");
    }
    
}

void single_d_arrays(void){
    int len = 5;
    int x[5] = {11,22,33,44,55};
    times1(x, len);
    times2(x,len);
    times3(x,len);

    array_double(x, len);
    times1(x, len);
}

void times1(int *a, int len) {
    for (int i = 0; i < len; i++)
    {
        printf("%d \t", a[i] * 1);
    }
    printf("\n");
}

void times2(int a[], int len){
    for (int i = 0; i < len; i++)
    {
        printf("%d\t", a[i]*2);
    }
    printf("\n");
}

void times3(int a[5], int len){
    for (int i = 0; i < len; i++)
    {
        printf("%d\t", a[i]*3);
    }
    printf("\n");
}

void array_double(int *a, int len) {
    for (int i = 0; i < len; i++)
    {
        a[i] *= 2;
    }
    printf("doubled \n");
}