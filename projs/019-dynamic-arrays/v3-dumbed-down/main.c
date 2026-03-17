/* main.c */
#define SIMPLE_DARRAY_IMPLEMENTATION
#include "libs/simple_darray.h"
#include <stdio.h>

/* Define a custom struct */
typedef struct {
    char name[50];
    int age;
    float score;
} Student;

int main() {
    printf("=== Integer Array Demo ===\n");
    
    /* Create array and add numbers */
    IntArray numbers = int_array_new();
    
    int_array_add(&numbers, 10);
    int_array_add(&numbers, 20);
    int_array_add(&numbers, 30);
    
    /* Print them */
    int i;
    for (i = 0; i < numbers.count; i++) {
        printf("numbers[%d] = %d\n", i, int_array_get(&numbers, i));
    }
    
    /* Change a value */
    int_array_set(&numbers, 1, 99);
    printf("After change: numbers[1] = %d\n", int_array_get(&numbers, 1));
    
    /* Clean up */
    int_array_free(&numbers);
    
    
    printf("\n=== String Array Demo ===\n");
    
    StringArray names = string_array_new();
    
    string_array_add(&names, "Alice");
    string_array_add(&names, "Bob");
    string_array_add(&names, "Charlie");
    
    for (i = 0; i < names.count; i++) {
        printf("names[%d] = %s\n", i, string_array_get(&names, i));
    }
    
    string_array_free(&names);
    
    
    printf("\n=== Struct Array Demo ===\n");
    
    StructArray students = struct_array_new(sizeof(Student));
    
    /* Create and add students */
    Student s1 = {"Alice", 20, 95.5};
    struct_array_add(&students, &s1);
    
    Student s2 = {"Bob", 22, 87.0};
    struct_array_add(&students, &s2);
    
    /* Read them back */
    for (i = 0; i < students.count; i++) {
        Student* s = struct_array_get(&students, i);
        printf("Student %d: %s, age %d, score %.1f\n", 
               i, s->name, s->age, s->score);
    }
    
    struct_array_free(&students);
    
    return 0;
}