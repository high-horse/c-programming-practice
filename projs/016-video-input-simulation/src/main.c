#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <fcntl.h>

int main(int argc, char *argv[])
{
    if(argc != 2) {
        printf("usage %s </dev/media_file_name>\n", argv[0]);
        return EXIT_FAILURE;
    }

    const char *filename = argv[1];
    int fd = open(filename, O_RDWR);
    if(fd < 0) {
        perror("FAILED TO OPEN FILE DESCRIPTOR:");
        return EXIT_FAILURE;
    }

    printf("File desc got %d\n", fd);
    

    printf("hello world\n");


    return EXIT_SUCCESS;
}