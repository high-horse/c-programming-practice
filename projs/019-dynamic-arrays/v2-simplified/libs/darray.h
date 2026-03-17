/**
 * darray.h - Dynamic Array Library for C (Minimal Macro Version)
 * 
 * Usage:
 *   Define DARRAY_IMPLEMENTATION in ONE .c file before including
 *   
 *   #define DARRAY_IMPLEMENTATION
 *   #include "darray.h"
 */

#ifndef DARRAY_H
#define DARRAY_H

#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <stddef.h>

/* ============================================================================
 * Configuration
 * ============================================================================ */

#ifndef DARRAY_INITIAL_CAPACITY
#define DARRAY_INITIAL_CAPACITY 8
#endif

#ifndef DARRAY_GROWTH_FACTOR
#define DARRAY_GROWTH_FACTOR 2
#endif

/* ============================================================================
 * Type Definitions
 * ============================================================================ */

/* Element lifecycle callbacks */
typedef void (*darray_copy_fn)(void* dest, const void* src);
typedef void (*darray_free_fn)(void* elem);
typedef int  (*darray_cmp_fn)(const void* a, const void* b);

/* The dynamic array structure - use as opaque handle */
typedef struct darray {
    void* data;
    size_t size;
    size_t capacity;
    size_t elem_size;
    
    /* Optional type-specific operations */
    darray_copy_fn copy_fn;
    darray_free_fn free_fn;
    darray_cmp_fn  cmp_fn;
} darray;

/* ============================================================================
 * Core API (All functions are actual C functions, not macros)
 * ============================================================================ */

/* Construction / Destruction */
darray darray_create(size_t elem_size);
darray darray_create_string(void);
void   darray_destroy(darray* arr);

/* Memory management */
bool darray_reserve(darray* arr, size_t capacity);
bool darray_resize(darray* arr, size_t size);
void darray_shrink_to_fit(darray* arr);
void darray_clear(darray* arr);

/* Element access (returns pointer to element) */
void* darray_get(darray* arr, size_t index);
void* darray_front(darray* arr);
void* darray_back(darray* arr);
void* darray_data(darray* arr);

/* Modifiers */
void* darray_push(darray* arr, const void* elem);
void  darray_pop(darray* arr, void* out_elem);
bool  darray_set(darray* arr, size_t index, const void* elem);
bool  darray_insert(darray* arr, size_t index, const void* elem);
bool  darray_remove(darray* arr, size_t index);

/* Operations */
ssize_t darray_find(const darray* arr, const void* elem);
void    darray_sort(darray* arr, darray_cmp_fn cmp);
void*   darray_search(const darray* arr, const void* key, darray_cmp_fn cmp);
darray  darray_clone(const darray* src);

/* String-specific helpers */
char** darray_push_string(darray* arr, const char* str);

/* ============================================================================
 * Inline Accessors (Small enough to inline, not complex macros)
 * ============================================================================ */

static inline size_t darray_size(const darray* arr) {
    return arr ? arr->size : 0;
}

static inline size_t darray_capacity(const darray* arr) {
    return arr ? arr->capacity : 0;
}

static inline bool darray_empty(const darray* arr) {
    return !arr || arr->size == 0;
}

static inline size_t darray_elem_size(const darray* arr) {
    return arr ? arr->elem_size : 0;
}

/* ============================================================================
 * Implementation
 * ============================================================================ */

#ifdef DARRAY_IMPLEMENTATION

static void darray_string_copy(void* dest, const void* src) {
    const char* s = *(const char**)src;
    *(char**)dest = s ? strdup(s) : NULL;
}

static void darray_string_free(void* elem) {
    free(*(char**)elem);
    *(char**)elem = NULL;
}

static int darray_string_cmp(const void* a, const void* b) {
    const char* sa = *(const char**)a;
    const char* sb = *(const char**)b;
    if (!sa) return sb ? -1 : 0;
    if (!sb) return 1;
    return strcmp(sa, sb);
}

darray darray_create(size_t elem_size) {
    darray arr = {0};
    arr.elem_size = elem_size;
    arr.capacity = DARRAY_INITIAL_CAPACITY;
    if (elem_size > 0 && arr.capacity > 0) {
        arr.data = calloc(arr.capacity, elem_size);
    }
    return arr;
}

darray darray_create_string(void) {
    darray arr = darray_create(sizeof(char*));
    arr.copy_fn = darray_string_copy;
    arr.free_fn = darray_string_free;
    arr.cmp_fn = darray_string_cmp;
    return arr;
}

void darray_destroy(darray* arr) {
    if (!arr) return;
    
    if (arr->free_fn && arr->data) {
        char* ptr = arr->data;
        for (size_t i = 0; i < arr->size; i++) {
            arr->free_fn(ptr + i * arr->elem_size);
        }
    }
    
    free(arr->data);
    arr->data = NULL;
    arr->size = 0;
    arr->capacity = 0;
}

bool darray_reserve(darray* arr, size_t capacity) {
    if (!arr || capacity <= arr->capacity) return true;
    
    void* new_data = realloc(arr->data, capacity * arr->elem_size);
    if (!new_data) return false;
    
    /* Zero new memory */
    char* old_end = (char*)new_data + arr->capacity * arr->elem_size;
    size_t new_bytes = (capacity - arr->capacity) * arr->elem_size;
    memset(old_end, 0, new_bytes);
    
    arr->data = new_data;
    arr->capacity = capacity;
    return true;
}

bool darray_resize(darray* arr, size_t size) {
    if (!arr) return false;
    
    if (size > arr->capacity) {
        size_t new_cap = arr->capacity;
        while (new_cap < size) new_cap *= DARRAY_GROWTH_FACTOR;
        if (!darray_reserve(arr, new_cap)) return false;
    }
    
    /* Clear removed elements if shrinking */
    if (size < arr->size && arr->free_fn) {
        for (size_t i = size; i < arr->size; i++) {
            char* elem = (char*)arr->data + i * arr->elem_size;
            arr->free_fn(elem);
        }
    }
    
    /* Zero new elements if growing */
    if (size > arr->size) {
        char* start = (char*)arr->data + arr->size * arr->elem_size;
        memset(start, 0, (size - arr->size) * arr->elem_size);
    }
    
    arr->size = size;
    return true;
}

void darray_shrink_to_fit(darray* arr) {
    if (!arr || arr->size == arr->capacity) return;
    
    if (arr->size == 0) {
        free(arr->data);
        arr->data = NULL;
        arr->capacity = 0;
        return;
    }
    
    void* new_data = realloc(arr->data, arr->size * arr->elem_size);
    if (new_data) {
        arr->data = new_data;
        arr->capacity = arr->size;
    }
}

void darray_clear(darray* arr) {
    if (!arr) return;
    
    if (arr->free_fn) {
        char* ptr = arr->data;
        for (size_t i = 0; i < arr->size; i++) {
            arr->free_fn(ptr + i * arr->elem_size);
        }
    }
    
    arr->size = 0;
}

void* darray_get(darray* arr, size_t index) {
    if (!arr || index >= arr->size) return NULL;
    return (char*)arr->data + index * arr->elem_size;
}

void* darray_front(darray* arr) {
    return darray_get(arr, 0);
}

void* darray_back(darray* arr) {
    return arr && arr->size > 0 ? darray_get(arr, arr->size - 1) : NULL;
}

void* darray_data(darray* arr) {
    return arr ? arr->data : NULL;
}

void* darray_push(darray* arr, const void* elem) {
    if (!arr || !elem) return NULL;
    
    if (arr->size >= arr->capacity) {
        if (!darray_reserve(arr, arr->capacity * DARRAY_GROWTH_FACTOR))
            return NULL;
    }
    
    void* slot = (char*)arr->data + arr->size * arr->elem_size;
    
    if (arr->copy_fn) {
        arr->copy_fn(slot, elem);
    } else {
        memcpy(slot, elem, arr->elem_size);
    }
    
    arr->size++;
    return slot;
}

char** darray_push_string(darray* arr, const char* str) {
    char* copy = str ? strdup(str) : NULL;
    return (char**)darray_push(arr, &copy);
}

void darray_pop(darray* arr, void* out_elem) {
    if (!arr || arr->size == 0) return;
    
    arr->size--;
    void* slot = (char*)arr->data + arr->size * arr->elem_size;
    
    if (out_elem) {
        memcpy(out_elem, slot, arr->elem_size);
    }
    
    if (arr->free_fn && !out_elem) {
        arr->free_fn(slot);
    }
}

bool darray_set(darray* arr, size_t index, const void* elem) {
    if (!arr || !elem || index >= arr->size) return false;
    
    void* slot = (char*)arr->data + index * arr->elem_size;
    
    if (arr->free_fn) {
        arr->free_fn(slot);
    }
    
    if (arr->copy_fn) {
        arr->copy_fn(slot, elem);
    } else {
        memcpy(slot, elem, arr->elem_size);
    }
    
    return true;
}

bool darray_insert(darray* arr, size_t index, const void* elem) {
    if (!arr || !elem || index > arr->size) return false;
    
    if (arr->size >= arr->capacity) {
        if (!darray_reserve(arr, arr->capacity * DARRAY_GROWTH_FACTOR))
            return false;
    }
    
    /* Shift elements right */
    char* insert_pos = (char*)arr->data + index * arr->elem_size;
    memmove(insert_pos + arr->elem_size, insert_pos, 
            (arr->size - index) * arr->elem_size);
    
    if (arr->copy_fn) {
        arr->copy_fn(insert_pos, elem);
    } else {
        memcpy(insert_pos, elem, arr->elem_size);
    }
    
    arr->size++;
    return true;
}

bool darray_remove(darray* arr, size_t index) {
    if (!arr || index >= arr->size) return false;
    
    void* slot = (char*)arr->data + index * arr->elem_size;
    
    if (arr->free_fn) {
        arr->free_fn(slot);
    }
    
    /* Shift elements left */
    char* next = (char*)slot + arr->elem_size;
    memmove(slot, next, (arr->size - index - 1) * arr->elem_size);
    
    arr->size--;
    return true;
}

ssize_t darray_find(const darray* arr, const void* elem) {
    if (!arr || !arr->cmp_fn || !elem) return -1;
    
    for (size_t i = 0; i < arr->size; i++) {
        void* slot = (char*)arr->data + i * arr->elem_size;
        if (arr->cmp_fn(slot, elem) == 0) {
            return (ssize_t)i;
        }
    }
    return -1;
}

void darray_sort(darray* arr, darray_cmp_fn cmp) {
    if (!arr || !cmp || arr->size < 2) return;
    qsort(arr->data, arr->size, arr->elem_size, cmp);
}

void* darray_search(const darray* arr, const void* key, darray_cmp_fn cmp) {
    if (!arr || !cmp || !key) return NULL;
    return bsearch(key, arr->data, arr->size, arr->elem_size, cmp);
}

darray darray_clone(const darray* src) {
    darray dst = {0};
    if (!src) return dst;
    
    dst.elem_size = src->elem_size;
    dst.capacity = src->size;
    dst.size = src->size;
    dst.copy_fn = src->copy_fn;
    dst.free_fn = src->free_fn;
    dst.cmp_fn = src->cmp_fn;
    
    if (src->size == 0) return dst;
    
    dst.data = malloc(src->size * src->elem_size);
    if (!dst.data) {
        dst.size = dst.capacity = 0;
        return dst;
    }
    
    if (src->copy_fn) {
        for (size_t i = 0; i < src->size; i++) {
            void* src_slot = (char*)src->data + i * src->elem_size;
            void* dst_slot = (char*)dst.data + i * src->elem_size;
            src->copy_fn(dst_slot, src_slot);
        }
    } else {
        memcpy(dst.data, src->data, src->size * src->elem_size);
    }
    
    return dst;
}

#endif /* DARRAY_IMPLEMENTATION */

#endif /* DARRAY_H */