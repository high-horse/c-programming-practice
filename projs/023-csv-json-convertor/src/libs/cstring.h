#ifndef CSTRING_H
#define CSTRING_H

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

#define AT_FIRST_INDEX 0
#define AT_LAST_INDEX -1

typedef struct {
    size_t len;
    size_t capacity;
    char *str;
} CString;

typedef struct {
    size_t len;
    size_t capacity;
    CString *item;
} CStringArray;

typedef struct {
    const char * str; 
    size_t len;
} CStringView;

typedef struct {
    size_t len;
    CStringView *item;
} CStringViewArray;

typedef struct {
    size_t len;
    size_t size;
    size_t capacity;
    char *data;
} CStringUTF8;

CString new_cstring(const char *data);
void destroy_cstring(CString *self);

int cstring_len(const CString *self);
bool cstring_isempty(const CString *self);

// compare
int cstring_compare(const CString *s1, const CString *s2);
bool cstring_equals(const CString *s1, const CString *s2);
bool cstring_equals_ignore_case(const CString *s1, const CString *s2);

// find substring
int cstring_indexof(const CString *self, const char *substr);
bool cstring_contains(const CString *self, const char *substr);
bool cstring_starts_with(CString *self, const char *prefix);
bool cstring_ends_with(CString *self, const char *suffix);

// modify
bool cstring_append(CString *self, const char *suffix);
bool cstring_append_cstring(CString *self, const CString *append_cstring);
bool cstring_append_char(CString *self, const char suffix_char);

bool cstring_prepend(CString *self, const char *prefix);
bool cstring_prepend_cstring(CString *self, CString *prepend_cstring);
bool cstring_prepend_char(CString *self, char prefix_char);

bool cstring_insert_char(CString *self, size_t pos, char new_char);
bool cstring_insert(CString *self, size_t pos, const char *text);

bool cstring_replace(CString *self, const char *oldstr, const char *newstr);
bool cstring_replaceall(CString *self, const char *oldstr, const char *newstr);

bool cstring_replace_char(CString *self, const char oldchar, const char newchar);


// trim
void cstring_ltrim(CString *self);
void cstring_rtrim(CString *self);
void cstring_trim(CString *self);

// upper / lower case
void cstring_to_upper(CString *self);
void cstring_to_lower(CString *self);

// split/ join
CStringArray cstring_split(CString *self, const char *delim);
// CString cstring_join(CString *self, const char *delim, size_t count);

// substring
CString cstring_substring(CString *self, size_t start, size_t len);

// reverse
void cstring_reverse(CString *self);

// remove chars
int cstring_remove(CString *self, const char *chars);

// conversion
// int cstring_to_int(CString *self);
// double cstring_to_double(CString *self);

CString cstring_repeat(CString *self, size_t times);

void cstring_array_free(CStringArray *arr);

#endif