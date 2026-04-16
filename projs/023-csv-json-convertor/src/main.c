#define _POSIX_C_SOURCE 200809L
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/mman.h>
#include <string.h>


void parse_heading(){
    
}

int main(int argc, char *argv[]) {
    if(argc != 2) {
        fprintf(stderr, "USAGE: %s <csv-file-to-parse>\n", argv[0]);
        return  EXIT_FAILURE;
    }
    const char *filename = argv[1];
    
    int fd = open(filename, O_RDWR);
    if(fd < 0) {
        perror("Failed to open file");
        return  EXIT_FAILURE;
    }

    struct stat fd_stat;
    if(fstat(fd, &fd_stat) < 0) {
        perror("Failed to read file status");
        goto failure_cleanup;
        
    }
    
    char *mapped = mmap(NULL, fd_stat.st_size, PROT_READ | PROT_WRITE, MAP_PRIVATE, fd, 0);
    if(mapped == MAP_FAILED) {
        perror("Failed to allocate memory");
        goto failure_cleanup;
    }
    
    
    char *first_line_end = memchr(mapped, '\n', fd_stat.st_size);
    if(first_line_end == NULL) {
        perror("No newline found in file");
        goto failure_cleanup;
    }
    
    size_t header_len = first_line_end - mapped;
    char *header_copy = calloc(header_len + 1, sizeof(char));
    
    memcpy(header_copy, mapped, header_len);
    header_copy[header_len] = '\0';
 
    char **headings = NULL;
    int count = 0 , capacity = 0;
    
    const char *delim = ",";
    char *heading = strtok(header_copy, delim);
    while(heading != NULL) {
        if(count >= capacity) {
            capacity = (capacity == 0) ? 2 : capacity * 2;
            headings = realloc(headings, capacity * sizeof(char *));
            if(headings == NULL) {
                perror("Failed to allocate");
                goto failure_cleanup;
            }
        }
        
        headings[count++] = heading;
        heading = strtok(NULL, delim);
    }
    
    for(int i = 0; i < count; i++) {
        printf("%s\n", headings[i]);
    }
    
    goto success_cleanup;
    
    failure_cleanup:
        if(mapped) munmap(mapped, fd_stat.st_size);
        if(header_copy) free(header_copy);
        if(headings) free(headings);
        if(fd > 0) close(fd);
        return EXIT_FAILURE;
        
    success_cleanup:
        if(mapped) munmap(mapped, fd_stat.st_size);
        if(header_copy) free(header_copy);
        if(headings) free(headings);
        if(fd > 0) close(fd);
        return EXIT_SUCCESS;
}