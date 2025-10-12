#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define BUFFER_SIZE 4096

void extract_text_from_pdf_(const char *pdf);


void extract_text_from_pdf(const char *filename) {
    FILE *fp = fopen(filename, "rb");
    if (!fp) {
        perror("File open failed");
        return;
    }

    char buffer[BUFFER_SIZE];
    while (fgets(buffer, sizeof(buffer), fp)) {
        char *start = buffer;

        while ((start = strstr(start, "(")) != NULL) {
            char *end = strchr(start + 1, ')');
            if (end) {
                *end = '\0'; // terminate the string
                printf("%s\n", start + 1);
                start = end + 1;
            } else {
                break;
            }
        }
    }

    fclose(fp);
}


int main(int argc, char *argv[]) {
    if (argc < 2) {
        fprintf(stderr, "Usage: %s <pdf-file>\n", argv[0]);
        return EXIT_FAILURE;
    }

    extract_text_from_pdf(argv[1]);

    return EXIT_SUCCESS;
    
}

void extract_text_from_pdf_(const char *pdf){
    FILE *fp = fopen(fp, "r");
    if(!fp) {
        perror("Unable to open file");
        return;
    }

    char buffer[ BUFFER_SIZE ];
    while (fgets(buffer, sizeof(buffer), fp))
    {
        char *start = buffer;
        while ((start = strstr(start, "(") != NULL))
        {
            char *end = strchr(start + 1, ")");
            if (end) {
                *end = '\0';
                printf("%s\n", start+1);
                start = end +1;
            } else {
                break;
            }
        }
        
    }
    fclose(fp);
}

