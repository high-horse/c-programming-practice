#include<stdio.h>

int shared = 100;
void func1(void) {
    shared ++;

}

void func2(void) {
    printf("%d\n", shared);
}

int main(void) {
    func1();
    func2();

    int a = 100;
    {
        int a = 1;
        printf("int a is %d\n", a);
    }
    printf("int a is %d\n", a);

    for (int i = 0; i < 2; i++)
    {
        int i = 999;
        printf("int i is %d\n", i);

    }
    
}