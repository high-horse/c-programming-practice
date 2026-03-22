#include "cstring.h"
#include <stdlib.h>
#include <string.h>


CString new_cstring(const char *data) {
    size_t length = strlen(data);
    size_t unit_size = sizeof(data[0]);
    char *str = calloc(length, unit_size);
    memcpy(str, data, length * unit_size);
    return (CString){str, length, length};
}


void destroy_cstring(CString *self){
    free(self->str);
    self->str = NULL;
    self->len = 0;
    self->capacity = 0;
}

int cstring_len(const CString *self) {
    return self->len;
}


bool cstring_isempty(const CString *self){
    return self->len == 0;
}


int cstring_compare(const CString *s1, const CString *s2){
    return strcmp(s1->str, s2->str);
}

bool cstring_equals(const CString *s1, const CString *s2){
    return strcmp(s1->str, s2->str) == 0;
}

bool cstring_equals_ignore_case(const CString *s1, const CString *s2){
    return strcasecmp(s1->str, s2->str) == 0;
}

int cstring_find(const CString *self, const char *substr){
    return strstr(self->str, substr) - self->str;
}

bool cstring_contains(const CString *self, const char *substr){
    return strstr(self->str, substr) != NULL;
}