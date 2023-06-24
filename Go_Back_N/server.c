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
	timeout.tv_sec = 5;
	timeout.tv_usec = 0;

	socklen_t len = sizeof(cliaddr);

	if (setsockopt(sockfd, SOL_SOCKET, SO_RCVTIMEO, &timeout, sizeof(timeout)) < 0) {
		perror("Setsockopt failed");
		return;
	}

	char addRes[100];
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


	strcpy(addRes, "Entering info");
	sendto(sockfd, addRes, sizeof(addRes), 0, (struct sockaddr *)&cliaddr, sizeof(cliaddr));

	int ack;
	printf("\n\n");

	int i = 0;
	while (i < n) {
		sendto(sockfd, &sendP[i], sizeof(sendP[i]), 0, (struct sockaddr *)&cliaddr, sizeof(cliaddr));
		printf("Packet [ %d ] sent", i + 1);
		fflush(stdout);
		++i;

		if (recvfrom(sockfd, &ack, sizeof(ack), 0, (struct sockaddr *)&cliaddr, &len) == -1) {
			printf("\tTimeout!!. Resending again...\n");
			sendP[--i].delay = 3;
			continue;
		}

		if (ack != i) {
			printf("\tInvalid Ack. Resending again...\n");
			--i;
			continue;
		}
		printf("\tAck received: %d\n", ack);
	}
	printf("\nServer Exiting...\n");
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
