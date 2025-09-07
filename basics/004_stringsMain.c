#include<stdio.h>
#include<string.h>

int custom_str_len(char *s);

int main(void) {
    char s[] = "hello world";
    char *t;
    
    // makes copu of the pointer
    t = s;
    // modify the pointer.
    t[0] = 'z';

    printf("original => %s \t t => %s\n", s, t);

    // copying a string.
    char str[] = "Hello world";
    char str_copy[100];
    strcpy(str_copy, str);
    printf("src => %s \t dest => %s\n", str, str_copy);
    str[0] ='z';
    printf("src => %s \t dest => %s\n", str, str_copy);
}



void len_1(void) {
    char *s1 = "hello world\n";
    char s2[] = "Hello world \n";
    char s3[14] = "Hello world\n";

    // You cannot mutate stting literams
    // s1[0] = 'a';

    printf("%s\n%s\n%s", s1, s2, s3); // print whole string

    for (int i = 0; i < 14; i++)
    {
        printf("%c \t%c\t%c\n", s1[i], s2[i], s3[i]);
    }
}

void len_2_stringLength() {
  // getting string literal length
    // strlen() returns size_t which is an integer 
    char *s = "hello world";
    printf("the string is %zu bytes long \n", strlen(s));

    int len = custom_str_len(s);
    printf("%d", len);
}
int custom_str_len(char *s){
    int count =0;
    while (s[count] != '\0')
    {
        count++;
    }
    return count;
    
}