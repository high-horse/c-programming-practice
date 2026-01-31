#include <stdio.h>
#include<stdlib.h>

int main(void) {
    printf("%d and %d", 5, '5');
}


void les_1(void) {
    int a = 10;
    char b = 'B';
    printf("%c\n", a + b);
}

void num_to_str() {
    char s[10];
    float f = 12.234;

    sprintf(s, "%f", f);
    // snprintf(s, 10, "%f", f); // same as above
    printf("%s \n", s);
}

void str_to_num(void) {
    char *s = "14522";
    int x = strtol(s, nullptr, 10);
    printf("%d\n", x);
}

void str_to_ul(void) {
    char *s = "3490";
    unsigned long int x;
    x = strtoul(s, nullptr, 10);
}

void les_2(void) {
    char *pi = "3.1415";
    float f;
    int i;
    f = atof(pi);
    i = atoi(pi);
    printf("%f\t %d\n", f, i);
}

void les_3(void) {
    char *s = "3423";
    char *badchar;
    long int a = strtol(s, &badchar, 10);

    if (*badchar == '\0') {
        printf("success conversion %d\n", a);
    } else {
        printf("Partial conversion %d\n", a);
        printf("bad characters %c\n", *badchar);
    }
}