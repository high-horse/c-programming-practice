#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <string.h>
#include <unistd.h>

#define PORT 8080
#define BUFFER_SIZE 1024

int main() {
    int status , read_val, client_fd, error;
    struct sockaddr_in server_addr;
    char buffer[BUFFER_SIZE] ={0};

    client_fd = socket(AF_INET, SOCK_STREAM, 0);
    if(client_fd < 0) {
        perror("SOCKET FAILED");
        return EXIT_FAILURE;
    }

    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);

    // convert ip v4 and v6 addresses to binary
    error = inet_pton(AF_INET, "127.0.0.1", &server_addr.sin_addr);
    if(error <= 0) {
        perror("INVALID ADDRESS");
        return EXIT_FAILURE;
    }

    error = connect(client_fd, (struct sockaddr *)&server_addr, sizeof(server_addr));
    if(error < 0){
        perror("CONN Failed");
        return EXIT_FAILURE;
    }

    // subtract 1 for the null
    // terminator at the end
    char *message = "Helllo from client";
    send(client_fd, message, strlen(message), 0);
    printf("Message sent from client\n");

    read_val = read(client_fd, buffer, BUFFER_SIZE-1);
    printf("Message recieved from server : %s\n", buffer);

    close(client_fd);

    return EXIT_SUCCESS;
}