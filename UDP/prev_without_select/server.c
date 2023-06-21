// server

#include <stdio.h>
#include <sys/socket.h>
#include <unistd.h>
// #include <sys/types.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <string.h>

int main() {
	int sockfd, status, connfd;
	struct sockaddr_in servaddr, cliaddr;

	// socket creation
	if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) == -1) {
		printf("Socket creation failed\n");
		close(sockfd);
		exit(0);
	} else
		printf("Socket Creation Successfull\n");


	printf("Sockid: %d\n", sockfd);

	servaddr.sin_family = AF_INET;
	servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
	servaddr.sin_port = htons(8080);

	// bind
	if (bind(sockfd, (struct sockaddr *)&servaddr, sizeof(servaddr)) == -1) {
		printf("Failed to bind\n");
		close(sockfd);
		exit(0);
	} else
		printf("Bind Successfull\n");

	char msg[20] = "Server Exiting...", buffer[100];
	int len = sizeof(cliaddr);

	recvfrom(sockfd, buffer, sizeof(buffer), 0, (struct sockaddr *)&cliaddr, &len);
	printf("%s", buffer);

	sendto(sockfd, msg, sizeof(msg), 0, (struct sockaddr *)&cliaddr, sizeof(servaddr));

	recvfrom(sockfd, buffer, sizeof(buffer), 0, (struct sockaddr *)&cliaddr, &len);
	printf("%s\n", buffer);

	// closing socket
	close(sockfd);

	return 0;
}
