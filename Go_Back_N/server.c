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
	struct timeval timeout, timeout_send;
	timeout.tv_sec = 0;
	timeout.tv_usec = 0;

	timeout_send.tv_sec = 0;
	timeout_send.tv_usec = 0;

	socklen_t len = sizeof(cliaddr);

	// making recvfrom blocking - to prevent invalid address resoultion -> due to timeout
	if (setsockopt(sockfd, SOL_SOCKET, SO_RCVTIMEO, &timeout, sizeof(timeout)) < 0) {
		perror("Setsockopt failed");
		return;
	}

	char addRes[100];
	// initial address resolution
	recvfrom(sockfd, addRes, sizeof(addRes), 0, (struct sockaddr *)&cliaddr, &len);

	// setting to real timeout 5s
	timeout.tv_sec = 5;
	setsockopt(sockfd, SOL_SOCKET, SO_RCVTIMEO, &timeout, sizeof(timeout));


	int n, w_sz;
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

	printf("\nEnter window size: ");
	while (1) {
		scanf("%d", &w_sz);
		if (w_sz < n)
			break;
		printf("\nWindow size should be greater than no of packets!!\n\n");
	}

	sendto(sockfd, &w_sz, sizeof(w_sz), 0, (struct sockaddr *)&cliaddr, sizeof(cliaddr));


	int ack;
	printf("\n\n");

	int i = 0, j = 0;

	for (int k = 0; k < w_sz; ++k, ++i) {
		sendto(sockfd, &sendP[i], sizeof(sendP[i]), 0, (struct sockaddr *)&cliaddr, sizeof(cliaddr));
		printf("Packet [ %d ] sent\n", i + 1);
		fflush(stdout);
	}
	while (j < n) {
		if (recvfrom(sockfd, &ack, sizeof(ack), 0, (struct sockaddr *)&cliaddr, &len) == -1) {
			printf("\tTimeout!!. Resending again...\n");
			i = j;
			sendP[i].delay = 2;
			for (int k = 0; k < w_sz && i < n; ++k, ++i) {
				sendto(sockfd, &sendP[i], sizeof(sendP[i]), 0, (struct sockaddr *)&cliaddr, sizeof(cliaddr));
				printf("Packet [ %d ] sent\n", i + 1);
				fflush(stdout);
			}
			continue;
		}

		if (ack != (j + 1)) {
			printf("\tInvalid Ack. Resending again...\n");
			i = j;

			for (int k = 0; k < w_sz && i < n; ++k, ++i) {
				sendto(sockfd, &sendP[i], sizeof(sendP[i]), 0, (struct sockaddr *)&cliaddr, sizeof(cliaddr));
				printf("Packet [ %d ] sent\n", i + 1);
				fflush(stdout);
			}
			continue;
		}
		printf("\tAck received: %d\n", ack);
		++j;

		if (i < n) {
			sendto(sockfd, &sendP[i], sizeof(sendP[i]), 0, (struct sockaddr *)&cliaddr, sizeof(cliaddr));
			printf("Packet [ %d ] sent\n", i + 1);
			fflush(stdout);
			++i;
		}
	}
	printf("\nServer Exiting...\n");
}

int main() {
	freopen("c.in", "r", stdin);

	// socket creation
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
