// client

#include <stdio.h>
#include <unistd.h>
#include <sys/socket.h>
// #include <sys/types.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <string.h>

void chatFunc(int sockfd) {
	char buff[20];
	while(1) {
		memset(buff, 0, 20);
		printf("Enter string: "); 
		scanf("%s", buff);
		if (!strcmp(buff, "exit")) {
			write(sockfd, buff, sizeof(buff));
			printf("Client exiting...\n");	
			close(sockfd);
			exit(0);
		}
		write(sockfd, buff, sizeof(buff));
		memset(buff, 0, 20);	
		read(sockfd, buff, sizeof(buff));
		printf("\tFrom server: %s\n", buff);
	}
}

int main() {
	int sockfd, status, connfd;
	struct sockaddr_in servaddr, cli;

	// socket creation
	if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) == -1) { 
		printf("Socket creation failed\n");
		exit(0);
	} else 
		printf("Socket Creation Successfull\n");


	printf("Sockid: %d\n", sockfd);

	servaddr.sin_family = AF_INET;
	servaddr.sin_addr.s_addr = inet_addr("127.0.0.1");
	servaddr.sin_port = htons(8080);
	
	if ((connfd = connect(sockfd, (struct sockaddr *)&servaddr, sizeof(servaddr))) == -1) {
		printf("Connection failed\n");
		exit(0);
	} else 
		printf("Connection successful\n");

	chatFunc(sockfd);

	// closing socket
	close(sockfd);

	return 0;
}
