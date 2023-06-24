// clent
#include <unistd.h>
#include <string.h>
#include <unistd.h>
#include <stdio.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define PORT 8080

int main() {
	int status, client_fd;
	struct sockaddr_in serv_addr;
	char* hello = "Hello from client";
	char buffer[1024] = {0};
	if ((client_fd = socket(AF_INET, SOCK_STREAM, 0 )) == -1) {
		printf("\n Socket creation error\n");
		exit(EXIT_FAILURE);
	}
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_family = htons(PORT);

	if (inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr) == -1) {
		printf("\nInvalid address\nAddress not supported\n");
		return -1;
	}

	if ((status = connect(client_fd, (struct sockaddr*)&serv_addr, sizeof(serv_addr))) == -1) {
		printf("\nConnection failed\n");	
		return -1;
	}
	send(client_fd, hello, strlen(hello), 0);
	printf("Hello message sent\n");
	read(client_fd, buffer, 1024);
	printf("%s\n", buffer);

	close(client_fd);


	return 0;
}
