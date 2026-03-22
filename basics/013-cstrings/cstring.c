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