#include "cstring.h"
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

CString new_cstring(const char *data) {
    size_t length = strlen(data);
    size_t unit_size = sizeof(data[0]);
    char *str = calloc(length + 1, sizeof(char));
    memcpy(str, data, length);
    str[length] = '\0';
    return (CString){length, length+1, str};
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
    if(!suffix) {
        return  false;
    }
    size_t suffix_len = strlen(suffix);
    size_t new_len = self->len + suffix_len;
    
    char *src = (char *)suffix;
    char *temp = NULL;
    // in case of same string
    if((suffix >= self->str) && (suffix < self->str + self->len)) {
        temp = malloc(suffix_len);
        if(!temp) {
            perror("FAILED TO ALLOCATE MEMORY");
            return false;
        }
        memcpy(temp, suffix, suffix_len);
        src = temp;
    } 

    if(self->capacity < new_len +1) {
        size_t new_capacity = (self->capacity == 0) ? 16 : self->capacity *2;
        while (new_capacity < new_len + 1) {
            new_capacity *= 2;
        }
        char *new_str = realloc(self->str, new_capacity); // +1 for null terminating the string.
        if(!new_str){
            perror("FAILED TO REALLOC THE STRING:");
            free(temp);
            return false;
        }
        self->str = new_str;
        self->capacity = new_capacity;
    }

    memcpy(self->str + self->len, src, suffix_len);
    self->str[new_len] = '\0';
    self->len = new_len;
    free(temp);
    return true;
}

bool cstring_append_cstring(CString *self, const CString *append_cstring) {
    size_t suffix_len = append_cstring->len;
    size_t new_len = self->len + suffix_len;

    char *src = append_cstring->str;
    char *temp = NULL;
    if((src >= self->str) && (src  < self->str + self->len)) {
        temp = malloc(append_cstring->len);
        if(!temp) {
            perror("FAILED TO ALLOCATE MEMORY:");
            return false;
        }
        memcpy(temp, append_cstring->str, suffix_len);
        src = temp;
    }

    if(self->capacity < new_len +1) {
        size_t new_capacity = (self->capacity == 0) ? 16 : self->capacity *2;
        while (new_capacity < new_len + 1) {
            new_capacity *= 2;
        }
        char *new_str = realloc(self->str, new_capacity); // +1 for null terminating the string.
        if(!new_str){
            perror("FAILED TO REALLOC THE STRING:");
            free(temp);
            return false;
        }
        self->str = new_str;
        self->capacity = new_capacity;
    }

    memcpy(self->str + self->len, src, suffix_len);
    self->str[new_len] = '\0';
    self->len = new_len;
    free(temp);
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

    char *src = (char *)prefix;
    char *temp = NULL;
    if(src >= self->str && src < self->str + self->len) {
        temp = malloc(prefix_len);
        if(!temp) {
            perror("FAILED TO MALLOC");
            return false;
        }
        memcpy(temp, src, prefix_len);
        src = temp;
    }
    
    if(self->capacity < new_len + 1) {
        size_t new_capacity = (self->capacity == 0) ? 16 : self->capacity * 2;
        while (new_capacity < new_len + 1) {
            new_capacity *= 2;
        }
        char *new_str = realloc(self->str, new_capacity);
        if(!new_str) {
            perror("FAILED TO REALLOC THE STRING:");
            free(temp);
            return false;
        }
        self->str = new_str;
        self->capacity = new_capacity;
    }

    memmove(self->str + prefix_len, self->str, self->len);
    memcpy(self->str, src, prefix_len);
    self->str[new_len] = '\0';
    self->len = new_len;
    free(temp);
    return true;
}

bool cstring_prepend_cstring(CString *self, CString *prepend_cstring) {
    size_t prefix_len = prepend_cstring->len;
    size_t new_len = self->len + prefix_len;

    char *src = prepend_cstring->str;
    char *temp = NULL;
    if((src >= self->str) && (src  < (self->str + self->len))) {
        temp = malloc(prefix_len);
        if(!temp) {
            perror("FAILED TO ALLOCATE:");
            return false;
        }
        memcpy(temp, prepend_cstring->str, prefix_len);
        src = temp;
    }
    
    if(self->capacity < new_len +1) {
        size_t new_capacity = (self->capacity == 0) ? 16 : self->capacity * 2;
        while(new_capacity < new_len + 1) {
            new_capacity *= 2;
        }

        char *new_str = realloc(self->str, new_capacity);
        if(!new_str) {
            perror("FAILED TO REALLOC:");
            free(temp);
            return false;
        }

        self->str = new_str;
        self->capacity = new_capacity;
    }

    memmove(self->str + prefix_len, self->str, self->len);
    memcpy(self->str, src, prefix_len);
    self->str[new_len] = '\0';
    self->len = new_len;

    free(temp);
    return true;
}


bool cstring_prepend_char(CString *self, char prefix_char){
    size_t new_len = self->len + 1;
    if(self->capacity < new_len +1) {
        size_t new_capacity = (self->capacity == 0) ? 16 : self->capacity * 2;
        while(new_capacity < new_len +1 ) {
            new_capacity *= 2;
        }
        char *new_str = realloc(self->str, new_capacity);
        if(!new_str) {
            perror("FAILED TO REALLOC MEMORY");
            return false;
        }
        
        self->str = new_str;
        self->capacity = new_capacity;
    }
    
    memmove(self->str + 1, self->str, self->len);
    self->str[0] = prefix_char;
    self->str[new_len] = '\0';
    return true;
}

bool cstring_insert_char(CString *self, size_t pos, char new_char) {
    if(pos > self->len) {
        return false;
    }

    size_t new_len = self->len + 1;
    if(self->capacity < new_len + 1) {
        size_t new_capacity = (self->capacity == 0) ? 16 : self->capacity * 2;
        while(new_capacity < new_len + 1) {
            new_capacity *= 2;
        }
        char *new_str = realloc(self->str, new_capacity);
        if(!new_str) {
            perror("FAILED TO REALLOC MEMORY");
            return false;
        }
        
        self->str = new_str;
        self->capacity = new_capacity;
    }

    memmove(self->str + pos + 1, self->str + pos, self->len - pos);
    self->str[pos] = new_char;
    self->str[new_len] = '\0';
    self->len = new_len;
    return true;
}

bool cstring_insert(CString *self, size_t pos, const char *text) {
    if(!text) return false;
    if(pos == AT_LAST_INDEX) pos = self->len;
    if (pos > self->len) return false;
    
    size_t src_len = strlen(text);
    size_t new_len = src_len + self->len;
    char *src = (char*) text;
    char *temp = NULL;
    if(src >= self->str && src < self->str + self->len) {
        temp = malloc(src_len);
        if(!temp){
            perror("FAILED TO ALLOCATE MEMORY:");
            return  false;
        }
        
        memcpy(temp, src, src_len);
        src = temp;
    }
    
    if(self->capacity < new_len + 1) {
        size_t new_capacity = (self->capacity == 0) ? 16 : self->capacity * 2;
        while (new_capacity < new_len +1) new_capacity *= 2;
        
        char *new_str = realloc(self->str, new_capacity);
        if(!new_str) {
            perror("FAILED TO ALLOCATE MEMORY:");
            free(temp);
            return false;
        }
        
        self->str = new_str;
        self->capacity = new_capacity;
    }
    
    memmove(self->str + pos + src_len, self->str + pos, self->len - pos + 1);
    memcpy(self->str + pos, src, src_len);
    
    self->len = new_len;
    free(temp);
    return true;
}

bool cstring_replace(CString *self, const char *oldstr, const char *newstr) {
    if(!oldstr || !newstr) return  false;
    size_t old_len = strlen(oldstr);
    size_t new_len = strlen(newstr);
    const char *pos = strstr(self->str, oldstr);
    if(!pos) return false;
    
    size_t pos_offset = pos - self->str;
    size_t new_total_len = self->len - old_len + new_len;
    
    if(self->capacity < new_total_len + 1) {
        size_t new_capacity = (self->capacity == 0) ? 16 : self->capacity * 2;
        while (new_capacity < new_total_len + 1) new_capacity *= 2;
        
        char *new_str = realloc(self->str, new_capacity);
        if(!new_str) {
            perror("FAILED TO ALLOCATE MEMORY:");
            return false;
        }
        
        self->str = new_str;
        self->capacity = new_capacity;
    }
    
    memmove(self->str + pos_offset + new_len, self->str + pos_offset + old_len, self->len - pos_offset - old_len + 1);
    memcpy(self->str + pos_offset, newstr, new_len);
    
    self->len = new_total_len;
    return true;
}

bool cstring_replaceall(CString *self, const char *oldstr, const char *newstr) {
    bool replaced = false;
    if(cstring_contains(self, oldstr)) {
        replaced = true;
        while (cstring_replace(self, oldstr, newstr)) ;
    } 
    return replaced;
}

void cstring_ltrim(CString *self) {
    if(self->len == 0 ) return;
    
    size_t start = 0;
    while(start < self->len && isspace(self->str[start])) {
        start ++;
    };
    
    if(start > 0) {
        memmove(self->str, self->str + start, self->len - start + 1);
        self->len -= start;
    }
    return;
}


void cstring_rtrim(CString *self) {
    if (self->len == 0) return;

    size_t end = self->len;
    while (end > 0 && isspace((unsigned char)self->str[end - 1])) {
        end--;
    }

    self->len = end;
    self->str[self->len] = '\0';
}

void cstring_trim(CString *self) {
    cstring_ltrim(self);
    cstring_rtrim(self);
}

void cstring_to_upper(CString *self){
    for (size_t i = 0; i < self->len; i++) {
        self->str[i] = toupper(self->str[i]);
    }
}

void cstring_to_lower(CString *self) {
    for (size_t i = 0; i < self->len; i++) {
        self->str[i] = tolower(self->str[i]);
    }
}

void cstring_array_free(CStringArray *arr) {
    for(size_t i = 0; i < arr->len; i++) {
        free(arr->item[i].str);
    }
    free(arr->item);
    
    arr->item = NULL;
    arr->len = 0;
}

CStringArray cstring_split(CString *self, const char *delim) {
    CStringArray result = (CStringArray){
      .item = NULL,
      .len = 0,
      .capacity = 2,
    };
    
    char *duplicate_copy = strdup(self->str);
    if(duplicate_copy == NULL) return result;
    
    result.item = malloc(result.capacity * sizeof(CString));
    if(result.item == NULL) {
        free(duplicate_copy);
        return result;
    }
    
    char *token = strtok(duplicate_copy, delim);
    
    while(token != NULL) {
        if(result.len == result.capacity) {
            size_t new_capacity = result.capacity * 2;
            CString *tmp = realloc(result.item, new_capacity * sizeof(CString));
            if(tmp == NULL) {
                for(size_t i = 0; i < result.len; i++) {
                    free(result.item[i].str);
                }
                
                free(result.item);
                free(duplicate_copy);
                
                result.item = NULL;
                result.len = 0;
                result.capacity = 0;
                return result;
            }
            
            result.item = tmp;
            result.capacity = new_capacity;
        }

        result.item[result.len] = new_cstring(token);
        result.len ++;
        
        token = strtok(NULL, delim);
    }
    free(duplicate_copy);
    return result;
}

CString cstring_substring(CString *self, size_t start, size_t len){
    if(start >= self->len) return new_cstring("");
    if(len == 0) return new_cstring("");
    
    size_t actual_len = (start + len > self->len) ? (self->len - start) : len;
    char *substr = malloc(actual_len + 1);
    if(substr == NULL) return new_cstring("");
    
    memcpy(substr, self->str + start, actual_len);
    substr[actual_len] = '\0';
    
    CString result = new_cstring(substr);
    free(substr);
    return result;
}


static void reverseString(char *str) {
    char *start = str;
    char *end = str + strlen(str) - 1;
    char temp;
    
    while(start < end) {
        temp =  *start;
        *start = *end;
        *end = temp;
        
        start ++;
        end --;
    }
}

void cstring_reverse(CString *self){
    reverseString(self->str);
 }
 
 CString cstring_repeat(CString *self, size_t times) {
     if(times < 0) return new_cstring("");
     
     CString result = new_cstring("");
     for(int i = 0; i< times; i++) {
         cstring_append_cstring(&result, self);
     }
     return result;
 }