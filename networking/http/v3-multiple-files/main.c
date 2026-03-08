#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <time.h>
#include <unistd.h>
#include <string.h>

#define PORT 9999
#define BUFFER_SIZE 1024

bool run_server = true;

void send_HTML(int *sock, const char *file) {
    printf("parsed route %s\n", file);
    FILE *html = fopen(file, "rb");
    if(html == NULL) {
        perror("FAILED TO OPEN FILE");
        return;
    }
    
    char buffer[BUFFER_SIZE] = {0};
    size_t read_size = 0;
    char *header =  "HTTP/1.1 200 OK\r\n"
           "Content-Type: text/html\r\n"
           "Connection: close\r\n"
           "\r\n";
    
    send(*sock, header, strlen(header), 0);
    while ((read_size = fread(buffer, 1, BUFFER_SIZE, html)) > 0) {
        send(*sock, buffer, read_size, 0);
    }
    fclose(html);

}

char* parse_route(const char *token){
    if(
        strcmp(token, "/home") == EXIT_SUCCESS || 
        strcmp(token, "/home.html") == EXIT_SUCCESS || 
        strcmp(token, "/index") == EXIT_SUCCESS || 
        strcmp(token, "/index.html") == EXIT_SUCCESS || 
        strcmp(token, "/") == EXIT_SUCCESS
    ) {
        return "static/index.html";
    } else if(
        strcmp(token, "/contact") == EXIT_SUCCESS || 
        strcmp(token, "/contact.html") == EXIT_SUCCESS
    ) {
        return "static/contact.html";
    } else if(
        strcmp(token, "/about") == EXIT_SUCCESS || 
        strcmp(token, "/about.html") == EXIT_SUCCESS
    ) {
        return "static/about.html";
    } else if(
        strcmp(token, "/projects") == EXIT_SUCCESS ||
        strcmp(token, "/projects.html") == EXIT_SUCCESS
    ) {
        return "static/projects.html";
    } else if(strcmp(token, "favicon.ico")) {
        return "static/facicon.ico";
    }
    return "static/404.html";
}

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
    
    struct sockaddr_in client_address;
    socklen_t client_len = sizeof(client_address);
    int client_socket;
    
    while(run_server) {
        client_socket = accept(server_sock_fd, (struct sockaddr *)&client_address, &client_len);
        if(client_socket < 0) {
            perror("FAILED TO ACCEPT CONN:");
            continue;
        }
        // printf("client connected\n");
        char request_buffer[BUFFER_SIZE] = {0};
        recv(client_socket, request_buffer, BUFFER_SIZE, 0);
        char *token = request_buffer + 4;
        char *route = strtok(token, " ");
        // printf("========tokenized========\n%s\n", route);
        
        send_HTML(&client_socket, parse_route(token));
        shutdown(client_socket, SHUT_WR);
        close(client_socket);
        // printf("client disconnected\n");
        
    }
    close(server_sock_fd);
    return EXIT_SUCCESS;
}