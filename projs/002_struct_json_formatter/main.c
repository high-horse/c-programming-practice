#include <stdio.h>
#include <stdlib.h>
#include <string.h>

const char *PERSON_FORMAT_IN = "{\n\t\"name\": \"%[^\"]\",\n\t\"age\": %d,\n\t\"gender\": \"%c\"\n}";

const char *PERSON_FORMAT_OUT = "{\n\t\"name\": \"%s\",\n\t\"age\": %d,\n\t\"gender\": \"%c\"\n}";

typedef struct
{
    char name[20];
    int age;
    char gender;
} Person;

int writeTxt(void)
{
    char *filename = "test.txt";
    char *mode = "a+";

    Person persons[3] = {
        {.name = "ram", .age = 10, .gender = 'M'},
        {.name = "Hari", .age = 15, .gender = 'M'},
        {.name = "Gita", .age = 10, .gender = 'F'},
    };
    FILE *fp;
    fp = fopen(filename, mode);
    if (!fp)
    {
        perror("File open failed");
        return 1;
    }

    // when opened in "a+" mode fseek end is not needed.
    // fseek(fp, 0, SEEK_END);
    for (int i = 0; i < 3; i++)
    {
        fprintf(fp, PERSON_FORMAT_OUT, persons[i].name, persons[i].age, persons[i].gender);
    }

    fclose(fp);
}

int main(void)
{
    char *filename = "test.txt";
    char *mode = "r";
    FILE *fp;
    fp = fopen(filename, mode);
    if (!fp)
    {
        perror("failed to open file");
        return 1;
    }

    char name[20];
    int age;
    char gender;
    Person persons[3];

    int i = 0;

    while (fscanf(fp, PERSON_FORMAT_IN, name, &age, &gender) != EOF)
    {
        strcpy(persons[i].name, name);
        // persons[i].name=name;
        persons[i].age = age;
        persons[i].gender = gender;
        i++;
    }

    for (int j = i - 1; j >= 0; j--)
    {
        printf("name : %s\t age: %d\t gender:%c\n", persons[j].name, persons[j].age, persons[j].gender);
    }

    fclose(fp);
}