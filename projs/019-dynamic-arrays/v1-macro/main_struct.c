#include <stdio.h>
#define DARRAY_IMPLEMENTATION
#include "libs/darray.h"

typedef struct {
    char* name;
    int age;
    float score;
} Student;

void student_copy(void* dest, const void* src) {
    const Student* s = src;
    Student* d = dest;
    d->name = s->name ? strdup(s->name) : NULL;
    d->age = s->age;
    d->score = s->score;
}

void student_free(void* elem) {
    Student* s = elem;
    free(s->name);
    s->name = NULL;
}

int student_cmp(const void* a, const void* b) {
    const Student* sa = a;
    const Student* sb = b;
    return strcmp(sa->name, sb->name);
}

int main() {
    darray(Student) students = darray_new_custom(Student, student_copy, student_free);
    
    Student s = {"Alice", 20, 95.5};
    darray_push(&students, s);
    
    // Deep copy ensures safe memory management
    darray_free(&students);
    return 0;
}