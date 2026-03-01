#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <unistd.h>

#define PORT 9999
#define BUFFER_SIZE 1024

bool run_server = true;

int main(){
    int server_sock_fd = socket(AF_INET, SOCK_STREAM, 0);
    if(server_sock_fd < 0) {
        perror("FAILED TO CREATE SERVER FD");
        exit(EXIT_FAILURE);
    }
    
    struct sockaddr_in server_address = {
        .sin_family = AF_INET,
        .sin_port = htons(PORT),
        .sin_addr.s_addr = INADDR_ANY,
    };
    
    if(bind(server_sock_fd, (struct sockaddr *)&server_address, sizeof(server_address)) != EXIT_SUCCESS){
        perror("FAILED TO BIND:");
        exit(EXIT_FAILURE);
    }
    
    if(listen(server_sock_fd, 10) != EXIT_SUCCESS) {
        perror("FAILED TO LISTEN:");
        exit(EXIT_FAILURE);
    }
    printf("listening on port %d\nWaiting for connection\n", PORT);
    while(run_server) {
        struct sockaddr_in client_address;
        socklen_t client_len = sizeof(client_address);
        int client_socket = accept(server_sock_fd, (struct sockaddr *)&client_address, &client_len);
        if(client_socket < 0) {
            perror("FAILED TO ACCEPT CONN:");
            continue;
        }
        printf("client connected\n");
        close(client_socket);
        printf("client disconnected\n");
        
    }
    return EXIT_SUCCESS;
}