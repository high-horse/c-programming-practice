#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <linux/input.h>
#include <sys/time.h>


int main(int argc, char *argv[]) {
    if(argc != 2) {
        printf("USAAGE: %s <path-to-file>\n", argv[0]);
        return EXIT_FAILURE;
    }
    char *openingfile = argv[1];
    printf("intersepting %s \n", openingfile);

    int fd = open(openingfile, O_RDONLY, 0);
    if(fd < 0) {
        printf("ERROR OPENING FILE DISCRIPTOR");
        return EXIT_FAILURE;
    }
    printf("Successfulluy opened the file discriptorb %d\n", fd);

    struct input_event captured_event ;
    int reading = 1;
    size_t red ;
    while (reading) {
     red = (fd, &captured_event, sizeof captured_event);
      if (red == (ssize_t)-1) {
            perror("read error");
            break; // Exit the loop on error
        }
     printf("read resp %zd, event type %u, code %u\n", red, captured_event.type, captured_event.code);

    }
    printf("read resp %zu\n", red);


    // int x ;
    // char c;
    // while( read(STDIN_FILENO, &c, 1) == 1 && c != 'q'){
    //     printf("read %c\n", c);
    // }
    
    
    close(fd);
    return EXIT_SUCCESS;
}