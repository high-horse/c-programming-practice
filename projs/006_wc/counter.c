#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define BUF_SIZE 1024

int count_words(const char *filename, int *count){
    *count = 0;

    char buffer[BUF_SIZE];
    FILE *fp = fopen(filename, "r");
    if(!fp) {
        perror("Could not open file\n");
        return EXIT_FAILURE;
    }

    char *delm = " .";

    while (fgets(buffer, BUF_SIZE, fp) != NULL) 
    {
        char buf_cpy[BUF_SIZE];
        strcpy(buf_cpy, buffer);


        char *token = strtok(buf_cpy, delm);
        while (token != NULL) {
            *count += 1;
            token = strtok(NULL, delm);
        }
    }


    fclose(fp);

    return EXIT_SUCCESS;
}