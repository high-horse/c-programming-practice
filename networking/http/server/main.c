#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <sys/socket.h>

#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>


#define PORT 8080
#define BUFFER_SIZE 1024

int main()
{
	int socket_fd, error , new_socket;
	int opt = 1;
	struct sockaddr_in address;
	char buffer[BUFFER_SIZE] = {0};

	socket_fd = socket(AF_INET, SOCK_STREAM, 0);
	if(socket_fd == -1) {
		perror("SOCKET FAILED");
		return EXIT_FAILURE;
	}


	// atach port to socket
	error = setsockopt(socket_fd, SOL_SOCKET, SO_REUSEADDR , &opt, sizeof(opt));
	if(error == -1) {
		perror("SETSOCKET ADDR FAILED");
		return EXIT_FAILURE;
	}

	address.sin_family = AF_INET;
	address.sin_addr.s_addr = INADDR_ANY;
	address.sin_port = htons(PORT); 

	printf("Socket bond to port %d\n", PORT);


	error = bind(socket_fd, (struct sockaddr *) &address, sizeof(address));
	if(error != 0) {
		perror("BIND FAILED");
		return EXIT_FAILURE;
	}

	error = listen(socket_fd, 3);
	if(error !=0 ) {
		perror("LISTEN FAILED");
		return EXIT_FAILURE;
	}

	new_socket = accept(socket_fd, (struct sockaddr *)&address, sizeof(address));
	if(new_socket == -1) {
		perror("ACCEPT FAILED");
		return EXIT_FAILURE;
	}

	// subtract 1 for the null
    // terminator at the end
	size_t read_size = read(new_socket, buffer, BUFFER_SIZE-1 );
	printf("Recieved message : %s\n", buffer);
	char *response = "hello world";
	send(new_socket, response, strlen(response), 0);
	printf("response sent\n");

	close(new_socket);
	close(socket_fd);
	return EXIT_SUCCESS;
}
