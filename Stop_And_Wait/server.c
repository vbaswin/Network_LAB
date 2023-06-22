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
	int delay;
	int last;
} Packets;

void chatLoop(int sockfd, struct sockaddr_in cliaddr) {
	struct timeval timeout;
	timeout.tv_sec = 5;		// no of seconds for timeout(or wait)
	timeout.tv_usec = 0;	// no of microseconds for timeout

	fd_set read_fds;
	FD_ZERO(&read_fds);
	FD_SET(sockfd, &read_fds);

	socklen_t len = sizeof(cliaddr);


	char addRes[100];
	// initial address resolution
	recvfrom(sockfd, addRes, sizeof(addRes), 0, (struct sockaddr *)&cliaddr, &len);

	int n;
	printf("\nEnter no of packets: ");
	scanf("%d", &n);
	Packets sendP[n];

	printf("Enter predicted delay and packet msg\n");
	for (int i = 0; i < n; ++i) {
		printf("[ %d ]: ", i + 1);
		scanf("%d %[^\n]s", &sendP[i].delay, sendP[i].msg);
		sendP[i].idx = i;
		sendP[i].last = 0;
	}
	sendP[n - 1].last = 1;

	// printf("\n");
	// for (int i = 0; i < n; ++i) {
	// 	printf("[ %d ]: %s %d %d\n", i + 1, sendP[i].msg, sendP[i].delay, sendP[i].last);
	// }

	strcpy(addRes, "Entering info");
	sendto(sockfd, addRes, sizeof(addRes), 0, (struct sockaddr *)&cliaddr, sizeof(cliaddr));

	int select_result, ack;
	printf("\n");

	int i = 0;
	while (i < n) {
		sendto(sockfd, &sendP[i], sizeof(sendP[i]), 0, (struct sockaddr *)&cliaddr, sizeof(cliaddr));
		printf("Packet [ %d ] sent\n", i + 1);
		++i;

		select_result = select(sockfd + 1, &read_fds, NULL, NULL, &timeout);

		if (select_result == -1) {
			perror("Select");
			exit(EXIT_FAILURE);
		} else if (select_result) {
			recvfrom(sockfd, &ack, sizeof(ack), 0, (struct sockaddr *)&cliaddr, &len);
			if (ack != i) {
				--i;
				printf("\nInvalid Ack!!. Resending...");
				continue;
			}
			printf("Ack: %d\n", ack);
		} else {
			printf("\nTimeout!!. Resending...\n");
			printf("chumma: %s\n", sendP[--i].msg);
			sendP[i].delay = 0;
			printf("i::%d", i);
		}
	}
	printf("\nServer Exiting...\n");

	/*
	while (1) {
		select_result = select(sockfd + 1, &read_fds, NULL, NULL, &timeout);

		if (select_result == -1) {
			perror("Select");
			exit(EXIT_FAILURE);
		} else if (select_result) {
			recvfrom(sockfd, &recvP, sizeof(recvP), 0, (struct sockaddr *)&cliaddr, &len);
			if (!strcmp(recvP.msg, "Exit") || !strcmp(recvP.msg, "exit")) {
				printf("\nServer Exiting...");
				break;
			}
			printf("\tmsg from Client: %s\n", recvP.msg);
		} else {
			printf("\nTimeout!!\n");
			break;
		}

		memset(sendP.msg, 0, sizeof(sendP.msg));

		printf("Enter msg: ");
		scanf("%[^\n]s", sendP.msg);
		getchar();

		sendto(sockfd, &sendP, sizeof(sendP), 0, (struct sockaddr *)&cliaddr, sizeof(cliaddr));

		if (!strcmp(sendP.msg, "Exit") || !strcmp(sendP.msg, "exit")) {
			printf("\nServer Exiting...");
			break;
		}
	}
	*/
}

int main() {
	freopen("c.in", "r", stdin);
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
