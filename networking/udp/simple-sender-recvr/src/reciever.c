#include <asm-generic/socket.h>
#include <stdio.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <unistd.h>
#include <sys/time.h>
#include <errno.h>

#define BUFFER_SIZE 1024
#define SOCKETERROR (-1)
int check(int exp, const char *msg) {
    if(exp == SOCKETERROR) {
        perror(msg);
        exit(EXIT_FAILURE);
    }
    return exp;
}

int main(int argc, char *argv[]){
     if(argc != 2) {
         printf("USAGE: %s <port>\n", argv[0]);
         exit(EXIT_FAILURE);
     }
     
     int my_port = atoi(argv[1]);
     if(my_port == 0) {
         perror("INVALID PORT :");
         exit(EXIT_FAILURE);
     }
     
     int udp_rx_socket ;
     struct sockaddr_in peer_addr;
     struct sockaddr_in my_addr = {
         .sin_family = AF_INET,
         .sin_addr.s_addr = INADDR_ANY,
         .sin_port = htons(my_port),
     };
     
     char buffer[BUFFER_SIZE];
     udp_rx_socket = socket(AF_INET, SOCK_DGRAM, 0);
     if(udp_rx_socket < 0) {
         perror("FAILED TO CREATE SOCKET:");
         exit(EXIT_FAILURE);
     }
     
     int result = bind(udp_rx_socket, (struct sockaddr *)&my_addr, sizeof(my_addr));
     check(result, "FAILED TO BIND SOCKET TO ADDR:");
     
     struct timeval timeout ={
        .tv_sec = 5,
        .tv_usec = 0
     };
     int err =setsockopt(udp_rx_socket, SOL_SOCKET, SO_RCVTIMEO, &timeout, sizeof(timeout));
     
     check(err, "FAILED TO SET TIMEOUT");
     
     socklen_t addr_len = sizeof(peer_addr);
     int bytes_recvd = recvfrom(udp_rx_socket, buffer, BUFFER_SIZE, 0, (struct sockaddr *)&peer_addr, &addr_len);
     if(bytes_recvd < 1 &&  errno == EWOULDBLOCK) {
         printf("CONNECTION TIMEOUT\n");
         exit(EXIT_FAILURE);
     }
     check(bytes_recvd, "recvfrom FAILED:");
     
     printf("RECIEVED PACKET FROM %s:%d -- Message = %s\n", inet_ntoa(peer_addr.sin_addr), ntohs(peer_addr.sin_port), buffer);
     
     close(udp_rx_socket);
     return EXIT_SUCCESS;
}