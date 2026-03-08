#include <stdlib.h>
#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include <unistd.h>

int main(int argc, char *argv[]) {
    if(argc != 4) {
        printf("USAGE: %s <peer_ip> <peer_port> <message>\n", argv[0]);
        exit(EXIT_FAILURE);
    }
    // 
    const char * peer_ip = argv[1];
    const int peer_port = atoi(argv[2]);
    if(peer_port <= 0) {
        perror("INVALID PORT:");
        exit(EXIT_FAILURE);
    }
    const char * message = argv[3];
    
    struct sockaddr_in peer_addr = {
        .sin_family = AF_INET,
        .sin_port = htons(peer_port),
        .sin_addr.s_addr = INADDR_ANY,
    };
    
    // convert ip to network binary printable.
    if(inet_pton(AF_INET, peer_ip, &(peer_addr.sin_addr)) <= 0){
        perror("INVALID IP ADDRESS:");
        exit(EXIT_FAILURE);
    }
    
    int udp_socket = socket(AF_INET, SOCK_DGRAM, 0);
    if(udp_socket < 0) {
        perror("FAILED TO CREATE SOCKET:");
        exit(EXIT_FAILURE);
    }
    
    if(sendto(udp_socket, message, strlen(message) + 1, 0, (const struct sockaddr *)&peer_addr, sizeof(peer_addr)) < 0){
        perror("FAILED TO SEND MESSAGE:");
        close(udp_socket);
        exit(EXIT_FAILURE);
    };
    
    printf("sent \"%s\" to %s:%d\n ", message, peer_ip, peer_port);
    close(udp_socket);
    
    return  EXIT_SUCCESS;
}