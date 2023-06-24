/*
	In UDP (User Datagram Protocol), the SO_REUSEADDR socket option can also be used to enable address reuse,
	just like in TCP. However, the implications and benefits of using SO_REUSEADDR in UDP are slightly different
	compared to TCP.

	In UDP, the SO_REUSEADDR option allows multiple sockets to bind to the same address and port combination.
	This can be useful in scenarios where you have multiple UDP sockets and you want to receive datagrams on the
	same address and port from different processes or threads.
*/

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

void recvTime(int sockfd, struct sockaddr_in cliaddr) {
	struct timeval timeout;
	timeout.tv_sec = 5;		// no of seconds for timeout(or wait)
	timeout.tv_usec = 0;	// no of microseconds for timeout

	Packets recvP, sendP;
	socklen_t len = sizeof(cliaddr);

	if (setsockopt(sockfd, SOL_SOCKET, SO_RCVTIMEO, &timeout, sizeof(timeout)) < 0) {
		perror("Setsockopt failed");
		return;
	}
	if (recv(sockfd, &recvP, sizeof(recvP), 0) == -1) {
		printf("Timeout!!. Resend again...\n");
	}

	recvfrom(sockfd, &recvP, sizeof(recvP), 0, (struct sockaddr *)&cliaddr, &len);
	printf("\tTime from Client: %s\n", recvP.msg);
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

	int reuse = 1;
	if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &reuse, sizeof(reuse)) < 0) {
		perror("Failed to set SO_REUSEADDR");
		exit(EXIT_FAILURE);
	}

	// bind
	if (bind(sockfd, (struct sockaddr *)&servaddr, sizeof(servaddr)) == -1) {
		printf("Failed to bind\n");
		close(sockfd);
		exit(0);
	} else
		printf("Bind Successfull\n");

	recvTime(sockfd, servaddr);

	close(sockfd);

	return 0;
}
