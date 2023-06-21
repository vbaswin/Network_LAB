// server

#include <stdio.h>
#include <sys/socket.h>
#include <unistd.h>
// #include <sys/types.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <string.h>

void chatFunc(int connfd, int sockfd) {
	char buff[20], rev[20];
	while (1) {
		memset(buff, 0, 20);
		memset(rev, 0, 20);
		recv(connfd, buff, sizeof(buff), 0);
		if (!strcmp(buff, "exit")) {
			printf("Server exiting...\n");
			close(sockfd);
			exit(0);
		}
		printf("From Server: %s\n\tTo Client: ", buff);
		int n = strlen(buff);
		for (int i = 0; i < n; ++i)
			rev[i] = buff[n - i - 1];
		printf("%s\n", rev);
		send(connfd, buff, sizeof(buff), 0);
	}
}

int main() {
	int sockfd, status, connfd;
	struct sockaddr_in servaddr, cli;

	// socket creation
	if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
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


	// listen
	if ((status = listen(sockfd, 5)) == -1) {
		printf("Error in listening\n");
		close(sockfd);
		exit(0);
	} else
		printf("Listening...\n");

	int len = sizeof(cli);

	// accept
	if ((connfd = accept(sockfd, (struct sockaddr *)&cli, &len)) == -1) {
		printf("Error in accepting\n");
		close(sockfd);
		exit(0);
	} else
		printf("Accepting...:\n");

	chatFunc(connfd, sockfd);

	// closing socket
	close(sockfd);

	return 0;
}
