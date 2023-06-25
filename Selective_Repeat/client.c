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
	struct timeval timeout, no_timeout;
	timeout.tv_sec = 10;
	timeout.tv_usec = 0;


	if (setsockopt(sockfd, SOL_SOCKET, SO_RCVTIMEO, &timeout, sizeof(timeout)) < 0) {
		perror("Setsockopt failed");
		return;
	}

	char addRes[] = "Initial Address resolution\n";
	sendto(sockfd, addRes, sizeof(addRes), 0, (struct sockaddr *)NULL, sizeof(servaddr));

	int w_sz;
	// waiting till all the packet data are entered
	recvfrom(sockfd, &w_sz, sizeof(w_sz), 0, (struct sockaddr *)NULL, NULL);

	Packets recvP[w_sz];

	printf("\n");
	int ack = 0, idx = 0, pack_recv_idx = 0, start_window = 0, last_set = 0;
	while (1) {
		if (pack_recv_idx == w_sz) {
			if (last_set) {
				printf("\nClient Exiting...\n");
				break;
			}
			// printf("Next set received\n");
			start_window += w_sz;
			pack_recv_idx = 0;
			continue;
		}

		if (recvfrom(sockfd, &recvP[pack_recv_idx], sizeof(recvP[pack_recv_idx]), 0, (struct sockaddr *)NULL, NULL) == -1) {
			printf("\nTimeout!!\n");
			continue;
		}

		if (recvP[pack_recv_idx].delay > 5) {
			continue;
		}

		idx = recvP[pack_recv_idx].idx;
		if (idx > start_window + w_sz - 1 || idx < start_window) {
			printf("start: %d idx: %d\n", start_window, idx);
			printf("Out of set packet received!!\n");
			continue;
		}
		ack = recvP[pack_recv_idx].idx + 1;

		sleep(recvP[pack_recv_idx].delay);
		sendto(sockfd, &ack, sizeof(ack), 0, (struct sockaddr *)NULL, sizeof(servaddr));
		printf("Packet [ %d ] received\tAck sent: %d\tMsg: %s\n", recvP[pack_recv_idx].idx + 1, ack, recvP[pack_recv_idx].msg);

		if (recvP[pack_recv_idx].last)
			last_set = 1;
		++pack_recv_idx;
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
