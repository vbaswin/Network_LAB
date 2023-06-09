#include <arpa/inet.h>
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

	struct timeval timeout;
	timeout.tv_sec = 10;	// no of seconds for timeout(or wait)
	timeout.tv_usec = 0;	// no of microseconds for timeout

	int ack;

	char addRes[] = "Initial Address resolution\n";
	sendto(sockfd, addRes, sizeof(addRes), 0, (struct sockaddr *)NULL, sizeof(servaddr));

	// waiting till all the packet data are entered
	recvfrom(sockfd, addRes, sizeof(addRes), 0, (struct sockaddr *)NULL, NULL);

	printf("\n");
	while (1) {
		if (recvfrom(sockfd, &recvP, sizeof(recvP), 0, (struct sockaddr *)NULL, NULL) == -1) {
			printf("\nTimeout!!. Resend again...\n");
			continue;
		}

		ack = recvP.idx + 1;

		if (recvP.delay > 5) {
			continue;
		}
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
