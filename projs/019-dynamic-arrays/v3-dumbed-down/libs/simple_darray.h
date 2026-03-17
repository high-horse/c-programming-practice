/**
 * simple_darray.h - The simplest dynamic array for C beginners
 * 
 * To use:
 * 1. Copy this file to your project
 * 2. In ONE .c file (like main.c), write this at the top:
 *    #define SIMPLE_DARRAY_IMPLEMENTATION
 *    #include "simple_darray.h"
 * 3. In other .c files, just write:
 *    #include "simple_darray.h"
 */

#ifndef SIMPLE_DARRAY_H
#define SIMPLE_DARRAY_H

/* ============ PART 1: INTEGER ARRAYS ============ */

typedef struct {
    int* data;          /* Pointer to the actual integers */
    int count;          /* How many integers we have */
    int capacity;       /* How many integers we can hold */
} IntArray;

/* Create a new empty integer array */
IntArray int_array_new(void);

/* Add one integer to the end */
void int_array_add(IntArray* arr, int value);

/* Get integer at position (0 is first) */
int int_array_get(IntArray* arr, int position);

/* Change integer at position */
void int_array_set(IntArray* arr, int position, int value);

/* Remove last integer */
void int_array_remove_last(IntArray* arr);

/* Free all memory */
void int_array_free(IntArray* arr);

/* ============ PART 2: STRING ARRAYS ============ */

typedef struct {
    char** data;        /* Pointer to array of strings */
    int count;          /* How many strings we have */
    int capacity;       /* How many strings we can hold */
} StringArray;

/* Create a new empty string array */
StringArray string_array_new(void);

/* Add a string (makes a copy automatically) */
void string_array_add(StringArray* arr, const char* text);

/* Get string at position */
char* string_array_get(StringArray* arr, int position);

/* Free all memory (frees strings too) */
void string_array_free(StringArray* arr);

/* ============ PART 3: CUSTOM STRUCT ARRAYS ============ */

/* 
 * Example: Let's say you have a struct like this:
 * 
 *     typedef struct {
 *         char name[50];
 *         int age;
 *     } Person;
 * 
 * To make a dynamic array of Person, you need to tell the library
 * how big each Person is. Here's how:
 */

typedef struct {
    void* data;         /* Generic pointer - will point to your structs */
    int count;
    int capacity;
    int item_size;      /* Size of your struct in bytes */
} StructArray;

/* Create array for your struct. Example:
 *     StructArray people = struct_array_new(sizeof(Person));
 */
StructArray struct_array_new(int item_size);

/* Add a struct. You pass the address of your struct. Example:
 *     Person p = {"Alice", 25};
 *     struct_array_add(&people, &p);
 */
void struct_array_add(StructArray* arr, void* item);

/* Get pointer to struct at position. Example:
 *     Person* p = struct_array_get(&people, 0);
 *     printf("%s is %d years old\n", p->name, p->age);
 */
void* struct_array_get(StructArray* arr, int position);

/* Free memory */
void struct_array_free(StructArray* arr);

#endif /* SIMPLE_DARRAY_H */

/* ============================================================================
 * IMPLEMENTATION - Only compile this part once!
 * ============================================================================ */

#ifdef SIMPLE_DARRAY_IMPLEMENTATION

#include <stdlib.h>   /* For malloc, free, realloc */
#include <string.h>   /* For strcpy, strlen, memcpy */

/* ============ INTEGER ARRAY IMPLEMENTATION ============ */

IntArray int_array_new(void) {
    IntArray arr;
    arr.count = 0;
    arr.capacity = 8;  /* Start with room for 8 integers */
    arr.data = malloc(arr.capacity * sizeof(int));
    return arr;
}

void int_array_add(IntArray* arr, int value) {
    /* If array is full, make it bigger */
    if (arr->count >= arr->capacity) {
        arr->capacity = arr->capacity * 2;  /* Double the size */
        arr->data = realloc(arr->data, arr->capacity * sizeof(int));
    }
    
    /* Add the new value at the end */
    arr->data[arr->count] = value;
    arr->count = arr->count + 1;
}

int int_array_get(IntArray* arr, int position) {
    return arr->data[position];
}

void int_array_set(IntArray* arr, int position, int value) {
    arr->data[position] = value;
}

void int_array_remove_last(IntArray* arr) {
    if (arr->count > 0) {
        arr->count = arr->count - 1;
    }
}

void int_array_free(IntArray* arr) {
    free(arr->data);
    arr->data = NULL;
    arr->count = 0;
    arr->capacity = 0;
}

/* ============ STRING ARRAY IMPLEMENTATION ============ */

StringArray string_array_new(void) {
    StringArray arr;
    arr.count = 0;
    arr.capacity = 8;
    arr.data = malloc(arr.capacity * sizeof(char*));
    return arr;
}

void string_array_add(StringArray* arr, const char* text) {
    /* If array is full, make it bigger */
    if (arr->count >= arr->capacity) {
        arr->capacity = arr->capacity * 2;
        arr->data = realloc(arr->data, arr->capacity * sizeof(char*));
    }
    
    /* Allocate memory for the string and copy it */
    int len = strlen(text);
    char* copy = malloc(len + 1);  /* +1 for the null terminator */
    strcpy(copy, text);
    
    /* Store the copy */
    arr->data[arr->count] = copy;
    arr->count = arr->count + 1;
}

char* string_array_get(StringArray* arr, int position) {
    return arr->data[position];
}

void string_array_free(StringArray* arr) {
    /* First free each string */
    int i;
    for (i = 0; i < arr->count; i++) {
        free(arr->data[i]);
    }
    
    /* Then free the array itself */
    free(arr->data);
    arr->data = NULL;
    arr->count = 0;
    arr->capacity = 0;
}

/* ============ STRUCT ARRAY IMPLEMENTATION ============ */

StructArray struct_array_new(int item_size) {
    StructArray arr;
    arr.count = 0;
    arr.capacity = 8;
    arr.item_size = item_size;
    arr.data = malloc(arr.capacity * item_size);
    return arr;
}

void struct_array_add(StructArray* arr, void* item) {
    /* If array is full, make it bigger */
    if (arr->count >= arr->capacity) {
        arr->capacity = arr->capacity * 2;
        arr->data = realloc(arr->data, arr->capacity * arr->item_size);
    }
    
    /* Copy the item byte by byte to the end of the array */
    /* We use char* because we want to copy byte by byte */
    char* destination = (char*)arr->data + (arr->count * arr->item_size);
    char* source = (char*)item;
    
    int i;
    for (i = 0; i < arr->item_size; i++) {
        destination[i] = source[i];
    }
    
    arr->count = arr->count + 1;
}

void* struct_array_get(StructArray* arr, int position) {
    /* Return pointer to the item at this position */
    char* ptr = (char*)arr->data;
    return ptr + (position * arr->item_size);
}

void struct_array_free(StructArray* arr) {
    free(arr->data);
    arr->data = NULL;
    arr->count = 0;
    arr->capacity = 0;
}

#endif /* SIMPLE_DARRAY_IMPLEMENTATION */