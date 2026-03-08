#include <stdio.h>
#include <stdlib.h>
#include "libs/cJSON.h"


#define BUFFER_SIZE 1024
#define PORT 9999

int main() {
    cJSON *json = cJSON_CreateObject();
    cJSON_AddStringToObject(json, "name", "john doe");
    cJSON_AddStringToObject(json, "email", "test@t.com");
    cJSON_AddNumberToObject(json, "age", 50);
    
    char *json_str = cJSON_Print(json);
    FILE *fp = fopen("test.json", "w");
    if( fp == NULL) {
        perror("Failed to open file:");
        exit(EXIT_FAILURE);
    }
    printf("JSON STRING %s \n", json_str);
    fputs(json_str, fp);
    fclose(fp);
    cJSON_free(json_str);
    cJSON_Delete(json);
    return EXIT_SUCCESS;
}