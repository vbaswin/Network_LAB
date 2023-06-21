/*
	more on fd_set -> info.c
	more on select -> info2.c
*/


// server

#include <arpa/inet.h>
#include <netinet/in.h>
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

void chatLoop(int sockfd, struct sockaddr_in cliaddr) {
	struct timeval timeout;
	timeout.tv_sec = 10;	// no of seconds for timeout(or wait)
	timeout.tv_usec = 0;	// no of microseconds for timeout

	fd_set read_fds;
	FD_ZERO(&read_fds);
	FD_SET(sockfd, &read_fds);

	Packets recvP, sendP;
	int select_result;
	socklen_t len = sizeof(cliaddr);

	while (1) {
		select_result = select(sockfd + 1, &read_fds, NULL, NULL, &timeout);

		if (select_result) {
			recvfrom(sockfd, &recvP, sizeof(recvP), 0, (struct sockaddr *)&cliaddr, &len);
			if (!strcmp(recvP.msg, "Exit") || !strcmp(recvP.msg, "exit")) {
				printf("Server Exiting...");
				break;
			}
			printf("\tmsg from Client: %s\n", recvP.msg);
		} else {
			printf("Timeout\n");
			break;
		}


		printf("Enter msg: ");
		scanf("%[^\n]s", sendP.msg);
		getchar();

		sendto(sockfd, &sendP, sizeof(sendP), 0, (struct sockaddr *)&cliaddr, sizeof(cliaddr));

		if (!strcmp(sendP.msg, "Exit") || !strcmp(sendP.msg, "exit")) {
			printf("Server Exiting...");
			break;
		}
	}
}

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

	chatLoop(sockfd, servaddr);

	close(sockfd);

	return 0;
}
