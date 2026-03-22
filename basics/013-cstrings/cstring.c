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

int cstring_indexof(const CString *self, const char *substr){
    if(self == NULL || self->str == NULL || substr == NULL) return -1;
    
    char *found = strstr(self->str, substr) ;
    if(!found) return -1;
    
    return found - self->str;
}

bool cstring_contains(const CString *self, const char *substr){
    return strstr(self->str, substr) != NULL;
}

bool cstring_starts_with(CString *self, const char *prefix) {
    if(self == NULL || self->str == NULL || prefix == NULL) return false;
    return strncmp(self->str, prefix, strlen(prefix)) == 0;
}

bool cstring_ends_with(CString *self, const char *suffix) {
    if(self == NULL || self->str == NULL || suffix == NULL) return false;
    size_t suffix_len = strlen(suffix);
    return suffix_len <= self->len && strcmp(self->str + self->len - suffix_len, suffix) == 0;
}