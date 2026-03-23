#include "cstring.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


CString new_cstring(const char *data) {
    size_t length = strlen(data);
    size_t unit_size = sizeof(data[0]);
    char *str = calloc(length + 1, sizeof(char));
    memcpy(str, data, length);
    str[length] = '\0';
    return (CString){str, length, length+1};
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

bool cstring_append(CString *self, const char *suffix) {
    size_t suffix_len = strlen(suffix);
    size_t new_len = self->len + suffix_len;
    
    if(self->capacity < new_len +1) {
        size_t new_capacity = (self->capacity == 0) ? 16 : self->capacity *2;
        while (new_capacity < new_len + 1) {
            new_capacity *= 2;
        }
        char *new_str = realloc(self->str, new_capacity); // +1 for null terminating the string.
        if(!new_str){
            perror("FAILED TO REALLOC THE STRING:");
            return false;
        }
        self->str = new_str;
        self->capacity = new_capacity;
    }
    
    memcpy(self->str + self->len, suffix, suffix_len);
    self->str[new_len] = '\0';
    self->len = new_len;
    return true;
}

bool cstring_append_cstring(CString *self, const CString *append_cstring) {
    size_t suffix_len = append_cstring->len;
    size_t new_len = self->len + suffix_len;
    
    if(self->capacity < new_len +1) {
        size_t new_capacity = (self->capacity == 0) ? 16 : self->capacity *2;
        while (new_capacity < new_len + 1) {
            new_capacity *= 2;
        }
        char *new_str = realloc(self->str, new_capacity); // +1 for null terminating the string.
        if(!new_str){
            perror("FAILED TO REALLOC THE STRING:");
            return false;
        }
        self->str = new_str;
        self->capacity = new_capacity;
    }
    
    memcpy(self->str + self->len, append_cstring->str, suffix_len);
    self->str[new_len] = '\0';
    self->len = new_len;
    return true;
}


bool cstring_append_char(CString *self, const char suffix_char) {
    size_t new_len = self->len + 1;
    
    if(self->capacity < new_len +1) {
        size_t new_capacity = (self->capacity == 0) ? 16 : self->capacity *2;
        while (new_capacity < new_len + 1) {
            new_capacity *= 2;
        }
        char *new_str = realloc(self->str, new_capacity); // +1 for null terminating the string.
        if(!new_str){
            perror("FAILED TO REALLOC THE STRING:");
            return false;
        }
        self->str = new_str;
        self->capacity = new_capacity;
    }
    
    self->str[self->len] = suffix_char;
    self->str[new_len] = '\0';
    self->len = new_len;
    return true;
}


bool cstring_prepend(CString *self, const char *prefix){
    size_t prefix_len = strlen(prefix);
    size_t new_len = self->len + prefix_len;
    
    if(self->capacity < new_len + 1) {
        size_t new_capacity = (self->capacity == 0) ? 16 : self->capacity * 2;
        while (new_capacity < new_len + 1) {
            new_capacity *= 2;
        }
        char *new_str = realloc(self->str, new_capacity);
        if(!new_str) {
            perror("FAILED TO REALLOC THE STRING:");
            return false;
        }
        self->str = new_str;
        self->capacity = new_capacity;
    }
    
    memmove(self->str + prefix_len, self->str, self->len);
    memcpy(self->str, prefix, prefix_len);
    self->str[new_len] = '\0';
    self->len = new_len;
    return true;
}