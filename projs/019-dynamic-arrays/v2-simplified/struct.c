#define DARRAY_IMPLEMENTATION
#include "libs/darray.h"

typedef struct {
    char* name;
    int age;
} Person;

void person_copy(void* dest, const void* src) {
    const Person* p = src;
    Person* d = dest;
    d->name = p->name ? strdup(p->name) : NULL;
    d->age = p->age;
}

void person_free(void* elem) {
    Person* p = elem;
    free(p->name);
}

int person_cmp(const void* a, const void* b) {
    const Person* pa = a;
    const Person* pb = b;
    return strcmp(pa->name, pb->name);
}

int main() {
    darray people = darray_create(sizeof(Person));
    
    /* Set up lifecycle management */
    people.copy_fn = person_copy;
    people.free_fn = person_free;
    people.cmp_fn = person_cmp;
    
    Person p = {"Alice", 30};
    darray_push(&people, &p);
    
    /* Deep copy ensures safe memory */
    darray_destroy(&people);
}