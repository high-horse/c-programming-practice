#ifndef CSTRING_H
#define CSTRING_H

#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
    char *str;
    size_t len;
    size_t capacity;
} CString;

CString new_cstring(const char *data);
bool destroy_cstring(CString *self);

int cstring_len(const CString *self);
bool cstring_isempty(const CString *self);

// compare
bool cstring_compare(const CString *s1, const CString *s2);
bool cstring_equals(const CString *s1, const CString s2);
bool cstring_equals_ignore_case(const CString *s1, const CString *s2);

// find substring
int cstring_find(const CString *self, const char *substr);
int cstring_contains(const CString *self, const char *substr);
bool cstring_starts_with(CString *self, const char *prefix);
bool cstring_ends_with(CString *self, const char *suffix);


// modify
bool cstring_append(CString *self, const char *suffix);
bool cstring_append_char(CString *self, const char suffix_char);
bool cstring_prepend(CString *self, const char *prefix);
bool cstring_prepend_char(CString *self, const char item, char prefix_char);

bool cstring_insert_char(CString *self, size_t pos, char new_char);
bool cstring_insert(CString *self, size_t pos, const char *text);
bool cstring_replace(CString *self, const char *oldstr, const char *newstr);


// trim
bool cstring_trim(CString *self);
bool cstring_ltrim(CString *self);
bool cstring_rtrim(CString *self);


// split/ join
CString *cstring_split(CString *self, const char *deliim, int *count);
CString cstring_join(CString *self, int count, const char *delim);

// substring
CString cstring_substring(CString *self, size_t start, size_t len);


bool cstring_to_upper(CString *self);
bool cstring_to_lower(CString *self);

// remove chars
bool cstring_remove(CString *self, const char *chars);
bool cstring_strip(CString *self, const char *chars);

// conversion
int cstring_to_int(CString *self);
double cstring_to_double(CString *self);

bool cstring_reverse(CString *self);
CString cstring_repeat(CString *self, size_t times);
#endif