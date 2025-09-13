#include <stdio.h>
#include<stdlib.h>

int main(void)
{
    char *pi = "3.1415";
    float f;
    int i;
    f = atof(pi);
    i = atoi(pi);
    printf("%f\t %d\n", f, i);
}


void les_1(void)
{
    int a = 10;
    char b = 'B';
    printf("%c\n", a + b);
}

void num_to_str(){
    char s[10];
    float f = 12.234;

    sprintf(s, "%f", f);
    // snprintf(s, 10, "%f", f); // same as above
    printf("%s \n", s);
}

void str_to_num(void){

}