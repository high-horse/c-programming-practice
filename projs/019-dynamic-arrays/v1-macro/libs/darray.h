/**
 * darray.h - Dynamic Array Header-Only Library for C
 * 
 * A type-safe, generic dynamic array implementation supporting:
 * - Primitive types (int, float, etc.)
 * - Strings (char*)
 * - Custom structs
 * 
 * Usage:
 *   Define DARRAY_IMPLEMENTATION before including in ONE .c file
 *   #define DARRAY_IMPLEMENTATION
 *   #include "darray.h"
 * 
 * Example:
 *   darray_int numbers = darray_new(int);
 *   darray_push(&numbers, 42);
 *   darray_free(&numbers);
 */

#ifndef DARRAY_H
#define DARRAY_H

#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <stdbool.h>

/* ============================================================================
 * CONFIGURATION
 * ============================================================================ */

#ifndef DARRAY_INITIAL_CAPACITY
#define DARRAY_INITIAL_CAPACITY 8
#endif

#ifndef DARRAY_GROWTH_FACTOR
#define DARRAY_GROWTH_FACTOR 2
#endif

/* ============================================================================
 * TYPE DEFINITIONS
 * ============================================================================ */

/**
 * Core dynamic array structure
 * Use the darray(TYPE) macro to create typed arrays
 */
typedef struct {
    void* data;           /* Raw data buffer */
    size_t size;          /* Current number of elements */
    size_t capacity;      /* Allocated capacity */
    size_t elem_size;     /* Size of each element */
    
    /* Function pointers for type-specific operations */
    void (*copy_fn)(void* dest, const void* src);      /* Deep copy element */
    void (*free_fn)(void* elem);                       /* Free element resources */
    int (*cmp_fn)(const void* a, const void* b);       /* Compare elements (optional) */
} darray_base;

/* ============================================================================
 * STRING TYPE SUPPORT
 * ============================================================================ */

/* String element operations */
static inline void darray_string_copy(void* dest, const void* src) {
    const char* s = *(const char**)src;
    *(char**)dest = s ? strdup(s) : NULL;
}

static inline void darray_string_free(void* elem) {
    char** s = (char**)elem;
    free(*s);
    *s = NULL;
}

static inline int darray_string_cmp(const void* a, const void* b) {
    const char* sa = *(const char**)a;
    const char* sb = *(const char**)b;
    if (!sa && !sb) return 0;
    if (!sa) return -1;
    if (!sb) return 1;
    return strcmp(sa, sb);
}

/* ============================================================================
 * GENERIC TYPE SUPPORT (for structs)
 * ============================================================================ */

/* Default shallow copy - just memcpy */
static inline void darray_shallow_copy(void* dest, const void* src) {
    /* Assumes elem_size is set in context - used internally */
    /* For public use, elements must provide their own copy_fn */
}

/* Default no-op free */
static inline void darray_noop_free(void* elem) {
    (void)elem; /* Suppress unused warning */
}

/* ============================================================================
 * CORE API (Type-safe wrappers use these internally)
 * ============================================================================ */

#ifdef DARRAY_IMPLEMENTATION

static inline darray_base darray_base_new(size_t elem_size) {
    darray_base arr = {0};
    arr.elem_size = elem_size;
    arr.capacity = DARRAY_INITIAL_CAPACITY;
    arr.data = malloc(elem_size * arr.capacity);
    if (!arr.data && arr.capacity > 0) {
        arr.capacity = 0;
    }
    return arr;
}

static inline void darray_base_free(darray_base* arr) {
    if (!arr) return;
    
    /* Call custom free for each element if provided */
    if (arr->free_fn) {
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

static inline bool darray_base_reserve(darray_base* arr, size_t new_capacity) {
    if (!arr || new_capacity <= arr->capacity) return true;
    
    void* new_data = realloc(arr->data, new_capacity * arr->elem_size);
    if (!new_data) return false;
    
    arr->data = new_data;
    arr->capacity = new_capacity;
    return true;
}

static inline bool darray_base_resize(darray_base* arr, size_t new_size) {
    if (!arr) return false;
    
    if (new_size > arr->capacity) {
        size_t new_cap = arr->capacity;
        while (new_cap < new_size) {
            new_cap *= DARRAY_GROWTH_FACTOR;
        }
        if (!darray_base_reserve(arr, new_cap)) return false;
    }
    
    /* Initialize new elements to zero */
    if (new_size > arr->size) {
        char* start = (char*)arr->data + arr->size * arr->elem_size;
        size_t bytes = (new_size - arr->size) * arr->elem_size;
        memset(start, 0, bytes);
    }
    
    arr->size = new_size;
    return true;
}

static inline void* darray_base_push(darray_base* arr, const void* elem) {
    if (!arr) return NULL;
    
    if (arr->size >= arr->capacity) {
        if (!darray_base_reserve(arr, arr->capacity * DARRAY_GROWTH_FACTOR)) {
            return NULL;
        }
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

static inline void darray_base_pop(darray_base* arr, void* out_elem) {
    if (!arr || arr->size == 0) return;
    
    arr->size--;
    void* slot = (char*)arr->data + arr->size * arr->elem_size;
    
    if (out_elem) {
        memcpy(out_elem, slot, arr->elem_size);
    }
    
    if (arr->free_fn) {
        arr->free_fn(slot);
    }
}

static inline void* darray_base_get(darray_base* arr, size_t index) {
    if (!arr || index >= arr->size) return NULL;
    return (char*)arr->data + index * arr->elem_size;
}

static inline bool darray_base_set(darray_base* arr, size_t index, const void* elem) {
    if (!arr || index >= arr->size) return false;
    
    void* slot = (char*)arr->data + index * arr->elem_size;
    
    /* Free old value if custom free provided */
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

static inline bool darray_base_insert(darray_base* arr, size_t index, const void* elem) {
    if (!arr || index > arr->size) return false;
    
    if (arr->size >= arr->capacity) {
        if (!darray_base_reserve(arr, arr->capacity * DARRAY_GROWTH_FACTOR)) {
            return false;
        }
    }
    
    /* Shift elements to make room */
    char* insert_pos = (char*)arr->data + index * arr->elem_size;
    memmove(insert_pos + arr->elem_size, insert_pos, (arr->size - index) * arr->elem_size);
    
    if (arr->copy_fn) {
        arr->copy_fn(insert_pos, elem);
    } else {
        memcpy(insert_pos, elem, arr->elem_size);
    }
    
    arr->size++;
    return true;
}

static inline bool darray_base_remove(darray_base* arr, size_t index) {
    if (!arr || index >= arr->size) return false;
    
    void* slot = (char*)arr->data + index * arr->elem_size;
    
    if (arr->free_fn) {
        arr->free_fn(slot);
    }
    
    /* Shift elements to fill gap */
    char* next = (char*)slot + arr->elem_size;
    memmove(slot, next, (arr->size - index - 1) * arr->elem_size);
    
    arr->size--;
    return true;
}

static inline ssize_t darray_base_find(darray_base* arr, const void* elem) {
    if (!arr || !arr->cmp_fn) return -1;
    
    for (size_t i = 0; i < arr->size; i++) {
        void* slot = (char*)arr->data + i * arr->elem_size;
        if (arr->cmp_fn(slot, elem) == 0) {
            return (ssize_t)i;
        }
    }
    return -1;
}

static inline void darray_base_clear(darray_base* arr) {
    if (!arr) return;
    
    if (arr->free_fn) {
        char* ptr = arr->data;
        for (size_t i = 0; i < arr->size; i++) {
            arr->free_fn(ptr + i * arr->elem_size);
        }
    }
    
    arr->size = 0;
}

static inline void darray_base_shrink_to_fit(darray_base* arr) {
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

static inline darray_base darray_base_clone(const darray_base* src) {
    darray_base dst = {0};
    if (!src) return dst;
    
    dst = *src;
    dst.data = malloc(src->capacity * src->elem_size);
    if (!dst.data) {
        dst.capacity = dst.size = 0;
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

/* ============================================================================
 * PUBLIC TYPE-SAFE API
 * ============================================================================ */

/**
 * Macro to declare a typed dynamic array
 * Usage: darray(int) my_array;
 */
#define darray(TYPE) \
    struct { \
        darray_base base; \
        TYPE* data; /* Typed access to base.data */ \
    }

/**
 * Initialize a new dynamic array
 * Usage: darray(int) arr = darray_new(int);
 */
#define darray_new(TYPE) \
    { \
        .base = darray_base_new(sizeof(TYPE)), \
        .data = NULL \
    }

/**
 * Initialize a string array (char* with deep copy/strdup)
 * Usage: darray(char*) arr = darray_new_string();
 */
#define darray_new_string() \
    { \
        .base = darray_base_new(sizeof(char*)), \
        .data = NULL \
    }

/**
 * Initialize array with custom type handlers
 * Usage with struct Person:
 *   darray(struct Person) arr = darray_new_custom(struct Person, 
 *                                                 person_copy, 
 *                                                 person_free);
 */
#define darray_new_custom(TYPE, COPY_FN, FREE_FN) \
    ({ \
        darray_base base = darray_base_new(sizeof(TYPE)); \
        base.copy_fn = (void (*)(void*, const void*))(COPY_FN); \
        base.free_fn = (void (*)(void*))(FREE_FN); \
        __typeof__(darray(TYPE)) arr = { .base = base, .data = NULL }; \
        arr; \
    })

/**
 * Free array and all resources
 */
#define darray_free(ARR) \
    do { \
        darray_base_free(&(ARR)->base); \
        (ARR)->data = NULL; \
    } while(0)

/**
 * Update typed data pointer (call after any operation that might realloc)
 */
#define darray_sync(ARR) \
    do { \
        (ARR)->data = (ARR)->base.data; \
    } while(0)

/**
 * Get current size
 */
#define darray_size(ARR) ((ARR)->base.size)

/**
 * Get current capacity
 */
#define darray_capacity(ARR) ((ARR)->base.capacity)

/**
 * Check if array is empty
 */
#define darray_empty(ARR) ((ARR)->base.size == 0)

/**
 * Reserve capacity (preallocate memory)
 */
#define darray_reserve(ARR, CAP) \
    darray_base_reserve(&(ARR)->base, (CAP))

/**
 * Resize array (adds zero-initialized elements or removes from end)
 */
#define darray_resize(ARR, NEW_SIZE) \
    (darray_base_resize(&(ARR)->base, (NEW_SIZE)) ? (darray_sync(ARR), true) : false)

/**
 * Push element to end (returns pointer to inserted element or NULL)
 */
#define darray_push(ARR, ELEM) \
    ({ \
        __typeof__(ELEM) _elem = (ELEM); \
        void* _ptr = darray_base_push(&(ARR)->base, &_elem); \
        darray_sync(ARR); \
        (__typeof__((ARR)->data))_ptr; \
    })

/**
 * Push string (strdup automatically)
 */
#define darray_push_string(ARR, STR) \
    ({ \
        const char* _str = (STR); \
        char* _copy = _str ? strdup(_str) : NULL; \
        void* _ptr = darray_base_push(&(ARR)->base, &_copy); \
        darray_sync(ARR); \
        (char**)_ptr; \
    })

/**
 * Pop element from end (optionally store in out)
 */
#define darray_pop(ARR) \
    ({ \
        __typeof__(*(ARR)->data) _tmp; \
        memset(&_tmp, 0, sizeof(_tmp)); \
        darray_base_pop(&(ARR)->base, &_tmp); \
        _tmp; \
    })

/**
 * Get element at index (returns pointer, NULL if out of bounds)
 */
#define darray_get(ARR, IDX) \
    ((__typeof__((ARR)->data))darray_base_get(&(ARR)->base, (IDX)))

/**
 * Set element at index
 */
#define darray_set(ARR, IDX, ELEM) \
    ({ \
        __typeof__(ELEM) _elem = (ELEM); \
        darray_base_set(&(ARR)->base, (IDX), &_elem); \
    })

/**
 * Access element directly (no bounds check)
 */
#define darray_at(ARR, IDX) ((ARR)->data[(IDX)])

/**
 * First element pointer
 */
#define darray_front(ARR) darray_get(ARR, 0)

/**
 * Last element pointer
 */
#define darray_back(ARR) \
    ((ARR)->base.size > 0 ? darray_get(ARR, (ARR)->base.size - 1) : NULL)

/**
 * Insert element at index
 */
#define darray_insert(ARR, IDX, ELEM) \
    ({ \
        __typeof__(ELEM) _elem = (ELEM); \
        bool _ok = darray_base_insert(&(ARR)->base, (IDX), &_elem); \
        if (_ok) darray_sync(ARR); \
        _ok; \
    })

/**
 * Remove element at index
 */
#define darray_remove(ARR, IDX) \
    darray_base_remove(&(ARR)->base, (IDX))

/**
 * Clear all elements (keep capacity)
 */
#define darray_clear(ARR) \
    darray_base_clear(&(ARR)->base)

/**
 * Shrink capacity to fit size
 */
#define darray_shrink_to_fit(ARR) \
    ({ \
        darray_base_shrink_to_fit(&(ARR)->base); \
        darray_sync(ARR); \
    })

/**
 * Find element (requires cmp_fn to be set)
 */
#define darray_find(ARR, ELEM) \
    ({ \
        __typeof__(ELEM) _elem = (ELEM); \
        darray_base_find(&(ARR)->base, &_elem); \
    })

/**
 * Iterate over array
 * Usage: darray_foreach(darray(int)*, arr, int*, elem) { ... }
 */
#define darray_foreach(TYPE, ARR, ELEM_TYPE, ELEM_NAME) \
    for (size_t _i = 0, _size = darray_size(ARR); \
         _i < _size && ((ELEM_NAME) = (ELEM_TYPE)darray_get(ARR, _i)); \
         _i++)

/**
 * Iterate with index
 */
#define darray_foreach_i(TYPE, ARR, ELEM_TYPE, ELEM_NAME, IDX_NAME) \
    for (size_t (IDX_NAME) = 0, _size = darray_size(ARR); \
         (IDX_NAME) < _size && ((ELEM_NAME) = (ELEM_TYPE)darray_get(ARR, (IDX_NAME))); \
         (IDX_NAME)++)

/**
 * Setup string operations for a char* array
 * Call after darray_new_string()
 */
#define darray_setup_string_ops(ARR) \
    do { \
        (ARR)->base.copy_fn = darray_string_copy; \
        (ARR)->base.free_fn = darray_string_free; \
        (ARR)->base.cmp_fn = darray_string_cmp; \
    } while(0)

/**
 * Setup custom operations for struct arrays
 */
#define darray_setup_ops(ARR, COPY_FN, FREE_FN, CMP_FN) \
    do { \
        (ARR)->base.copy_fn = (void (*)(void*, const void*))(COPY_FN); \
        (ARR)->base.free_fn = (void (*)(void*))(FREE_FN); \
        (ARR)->base.cmp_fn = (int (*)(const void*, const void*))(CMP_FN); \
    } while(0)

/**
 * Clone array (deep copy if copy_fn is set)
 */
#define darray_clone(DST, SRC) \
    do { \
        (DST)->base = darray_base_clone(&(SRC)->base); \
        darray_sync(DST); \
    } while(0)

/* ============================================================================
 * SORTING SUPPORT (using qsort)
 * ============================================================================ */

/**
 * Sort array using comparator
 * CMP should be: int cmp(const TYPE* a, const TYPE* b)
 */
#define darray_sort(ARR, CMP) \
    do { \
        qsort((ARR)->base.data, (ARR)->base.size, (ARR)->base.elem_size, \
              (int (*)(const void*, const void*))(CMP)); \
    } while(0)

/**
 * Binary search (array must be sorted)
 * Returns pointer to found element or NULL
 */
#define darray_bsearch(ARR, KEY, CMP) \
    ({ \
        __typeof__(KEY) _key = (KEY); \
        void* _res = bsearch(&_key, (ARR)->base.data, (ARR)->base.size, \
                            (ARR)->base.elem_size, \
                            (int (*)(const void*, const void*))(CMP)); \
        (__typeof__((ARR)->data))_res; \
    })

/* ============================================================================
 * UTILITY MACROS
 * ============================================================================ */

/**
 * Convert to raw pointer (for C interop)
 */
#define darray_data(ARR) ((ARR)->base.data)

/**
 * Get byte size of used data
 */
#define darray_data_size(ARR) ((ARR)->base.size * (ARR)->base.elem_size)

/**
 * Check if index is valid
 */
#define darray_valid_index(ARR, IDX) ((IDX) < darray_size(ARR))

/**
 * Swap two arrays (shallow swap of handles)
 */
#define darray_swap(A, B) \
    do { \
        __typeof__(*(A)) _tmp = *(A); \
        *(A) = *(B); \
        *(B) = _tmp; \
    } while(0)

#endif /* DARRAY_H */