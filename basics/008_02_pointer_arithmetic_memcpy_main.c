#include <stdio.h>
#include <string.h>
#include <stdlib.h>

void *my_memcpy(void *dest, void *src, int byte_count);
typedef struct
{
    char *name;
    int leg_count;
} animal;

int compar(const void *elem1, const void *elem2)
{
    const animal *animal1 = elem1;
    const animal *animal2 = elem2;

    if (animal1->leg_count > animal2->leg_count)
    {
        return 1;
    }
    else if (animal1->leg_count < animal2->leg_count)
    {
        return -1;
    }
    return 0;
}

int main(void)
{
    animal animals[] = {
        {.name = "dog", .leg_count = 4},
        {.name = "monkey", .leg_count = 2},
        {.name = "antelop", .leg_count = 4},
        {.name = "snake", .leg_count = 0},
    };

    qsort(animals, 4, sizeof(animal), compar);
    for (int i = 0; i < 4; i++)
    {
        printf("%d: %s\n", animals[i].leg_count, animals[i].name);
    }
}

void *my_memcpy(void *dest, void *src, int byte_count)
{
    // convert void to char
    char *d = dest, *s = src;

    // Now that we have char*s, we can dereference and copy them
    while (byte_count--)
    {
        *d++ = *s++;
    }

    // Most of these functions return the destination, just in case
    // that's useful to the caller.
    return dest;
}

void les_1(void)
{
    char s[] = "hello";
    char t[100];

    memcpy(t, s, sizeof(s));
    printf("%s\n", t);

    int a[] = {10, 20, 30, 40};
    int a__dest[100];
    memcpy(a__dest, a, 3 * sizeof(int));
    printf("%d \n", a__dest[0]);
    printf("%zu\n", sizeof(a__dest));
}

void les_2(void)
{
    animal dog = {.name = "dog", .leg_count = 4};
    animal dog_2;
    memcpy(&dog_2, &dog, sizeof(dog));

    printf("aminal %s has %d legs.\n", dog_2.name, dog_2.leg_count);
}