#include <arpa/inet.h>
#include <fcntl.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <time.h>
#include <unistd.h>

typedef struct packets {
	char msg[200];
	int idx;
	int delay;
	int last;
} Packets;

void chatLoop(int sockfd, struct sockaddr_in servaddr) {
	Packets recvP;

	struct timeval timeout, no_timeout;
	timeout.tv_sec = 10;
	timeout.tv_usec = 0;

	no_timeout.tv_sec = 0;
	no_timeout.tv_usec = 22;


	if (setsockopt(sockfd, SOL_SOCKET, SO_RCVTIMEO, &timeout, sizeof(timeout)) < 0) {
		perror("Setsockopt failed");
		return;
	}

	char addRes[] = "Initial Address resolution\n";
	sendto(sockfd, addRes, sizeof(addRes), 0, (struct sockaddr *)NULL, sizeof(servaddr));

	// waiting till all the packet data are entered
	recvfrom(sockfd, addRes, sizeof(addRes), 0, (struct sockaddr *)NULL, NULL);

	printf("\n");
	int ack = 0;
	while (1) {
		if (recvfrom(sockfd, &recvP, sizeof(recvP), 0, (struct sockaddr *)NULL, NULL) == -1) {
			printf("\nTimeout!!. Resend again...\n");
			continue;
		}

		if (recvP.delay > 5) {
			// reduced waiting time of recvfrom to 22 micro sec
			// cannot to 0 -> blocks till received ;) <==
			if (setsockopt(sockfd, SOL_SOCKET, SO_RCVTIMEO, &no_timeout, sizeof(no_timeout)) < 0) {
				perror("Setsockopt failed");
				return;
			}
			// to make recvfrom non-blocking <===
			// maybe difficult to remember the syntax so above could be better
			// int flags = fcntl(sockfd, F_GETFL, 0);
			// fcntl(sockfd, F_SETFL, flags | O_NONBLOCK);

			// to consume the received packet buffer
			while (recvfrom(sockfd, &recvP, sizeof(recvP), 0, (struct sockaddr *)NULL, NULL) > 0)
				;

			// to make recvfrom blocking <====
			// flags = fcntl(sockfd, F_GETFL, 0);
			// fcntl(sockfd, F_SETFL, flags & (~O_NONBLOCK));

			// setting the waiting time of recvfrom back to 5 sec
			setsockopt(sockfd, SOL_SOCKET, SO_RCVTIMEO, &timeout, sizeof(timeout));
			continue;
		}

		// out of packets may not always occur
		if (recvP.idx != ack) {
			printf("Out of order packet!!. Resend again...\n");
			sendto(sockfd, &ack, sizeof(ack), 0, (struct sockaddr *)NULL, sizeof(servaddr));
			continue;
		}
		ack = recvP.idx + 1;

		sleep(recvP.delay);
		sendto(sockfd, &ack, sizeof(ack), 0, (struct sockaddr *)NULL, sizeof(servaddr));
		printf("Packet [ %d ] received\tAck sent: %d\tMsg: %s\n", recvP.idx + 1, ack, recvP.msg);

		if (recvP.last) {
			printf("\nClient Exiting...\n");
			break;
		}
	}
}

int main() {
	freopen("c.in", "r", stdin);
	int sockfd, status, connfd;
	struct sockaddr_in servaddr, cliaddr;

	// socket creation
	if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) == -1) {
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

	chatLoop(sockfd, servaddr);

	close(sockfd);

	return 0;
}
