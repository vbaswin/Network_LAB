#include <arpa/inet.h>
#include <netinet/in.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

typedef struct packets {
	char msg[200];
	int idx;
} Packets;

void chatLoop(int connfd) {
	struct timeval timeout;
	timeout.tv_sec = 10;	// no of seconds for timeout(or wait)
	timeout.tv_usec = 0;	// no of microseconds for timeout

	fd_set read_fds;
	FD_ZERO(&read_fds);
	FD_SET(connfd, &read_fds);

	Packets recvP, sendP;
	int select_result;

	while (1) {
		select_result = select(connfd + 1, &read_fds, NULL, NULL, &timeout);

		if (select_result) {
			recv(connfd, &recvP, sizeof(recvP), 0);
			if (!strcmp(recvP.msg, "Exit") || !strcmp(recvP.msg, "exit")) {
				printf("Server Exiting...");
				break;
			}
			printf("\tmsg from Client: %s\n", recvP.msg);
		} else {
			printf("Timeout\n");
			break;
		}

		if (!strcmp(recvP.msg, "Exit") || !strcmp(recvP.msg, "exit")) {
			printf("Server Exiting...");
			break;
		}
		memset(sendP.msg, 0, sizeof(sendP.msg));

		printf("Enter msg: ");
		scanf("%[^\n]s", sendP.msg);
		getchar();

		send(connfd, &sendP, sizeof(sendP), 0);
		if (!strcmp(sendP.msg, "Exit") || !strcmp(sendP.msg, "exit")) {
			printf("Server Exiting...");
			break;
		}
	}
}

int main() {
	int sockfd, status, connfd;
	struct sockaddr_in servaddr, cliaddr;

	// AF_INET : AF -> Address Family AF_INET-> IPv4 address family
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

	if ((status = listen(sockfd, 5)) == -1) {
		printf("Error in listening\n");
		close(sockfd);
		exit(0);
	} else
		printf("Listening...\n");


	int len = sizeof(cliaddr);

	if ((connfd = accept(sockfd, (struct sockaddr *)&cliaddr, &len)) == -1) {
		printf("Error in accepting\n");
		close(sockfd);
		exit(0);
	} else
		printf("Accepting...\n");

	chatLoop(connfd);
	// printf("%d %d\n", sockfd, connfd); // they are different

	close(sockfd);

	return 0;
}
