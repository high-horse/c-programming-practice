#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

float calculate_result(int first, int second, float third);
void printPointer(int i);
void deReference(void);

int main(void)
{
    deReference();
}

void deReference(void){
    int i;
    int *p;
    p = &i;
    bool pass = true;

    printf("the value of i is %d\n", i);
    printf("the value of de-pointer of p is %d\n", *p);
    printf("pass status is %b\n", pass);

    *p = 20;
    printf("the value of i is %d\n", i);
    printf("the value of de-pointer of p is %d\n", *p);
}

void printPointer(int i)
{
    printf("the value of i is %d\n", i);
    printf("the add ress of i is %p\n", (void *)&i);
}

float calculate_result(int first, int second, float third)
{
    float ans = first + second + third;
    return ans;
}

void checkVersion()
{
    printf("C version: %ld\n", __STDC_VERSION__);
}