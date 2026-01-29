#include <stdio.h>

typedef struct animal
{
    char* name;
    int leg_count, speed ;
} animal;

typedef struct 
{
    char* name;
    char* family;
    int leg_count, speed;
} insect;


int main(void)
{
    typedef int plant;
    plant lily = 1;

    struct animal deer;
    animal antelop;

    insect mosquito;
}