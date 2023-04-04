// server 

#include <sys/socket.h>
#include <sys/types.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <netinet/in.h>
#define PORT 8080

int main() {
	
	int server_fd, new_socket, opt = 1;
	struct sockaddr_in address;

	int addrlen= sizeof(address);
	char buffer[1024] = {0};
	char* hello = "Hello from server";

	if ((server_fd = socket(AF_INET,SOCK_STREAM, 0)) == -1) {
		perror("socket failed");
		exit(EXIT_FAILURE);
	}
	if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt))) {
		perror("setsockopt");
		exit(EXIT_FAILURE);
	}
	printf("sockid: %d\n", server_fd);

	address.sin_family = AF_INET;
	address.sin_addr.s_addr = INADDR_ANY;
	address.sin_port = htons(PORT);

	if (bind(server_fd, (struct sockaddr*)&address, sizeof(address)) == -1) {
		perror("bind failed");
		exit(EXIT_FAILURE);
	}
	if (listen(server_fd, 3) == -1) {
		perror("listen");
		exit(EXIT_FAILURE);
	}

	if (new_socket = accept(server_fd, (struct sockaddr*)&address, (socklen_t*)(&addrlen)) == -1) {
		perror("accept");
		exit(EXIT_FAILURE);
	}

	read(new_socket, buffer, 1024);
	printf("%s\n", buffer);
	send(new_socket, hello, strlen(hello), 0);
	printf("Hello message sent\n");

	close(server_fd);
	shutdown(server_fd, SHUT_RDWR);
	
	return 0;
}	
