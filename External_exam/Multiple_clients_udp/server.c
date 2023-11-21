#include <netinet/in.h>
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

// #include <netinet/ip.h>

typedef struct {
	char msg[100];
	int idx;
} PACKETS;

int main() {
	struct sockaddr_in servAddr, cliAddr;
	int sockfd;
	if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) == -1) {
		printf("Failed to create socket\n");
		return 0;
	} else
		printf("Socket created successfully\n");

	servAddr.sin_family = AF_INET;
	servAddr.sin_port = htons(8080);
	servAddr.sin_addr.s_addr = htonl(INADDR_ANY);

	if (bind(sockfd, (struct sockaddr *)&servAddr, sizeof(servAddr)) == -1) {
		printf("Socket binding Failed!\n");
		return 0;
	} else
		printf("Socket binding successfull!\n");

	PACKETS recvP, sendP;

	socklen_t len = sizeof(cliAddr);
	struct timeval timeout;
	timeout.tv_sec = 5;
	timeout.tv_usec = 0;
	// timeout.tv_nsec = 10;

	struct timespec ts;
	ts.tv_nsec = 5;
	ts.tv_nsec = 20;

	recvfrom(sockfd, &recvP, sizeof(recvP), 0, (struct sockaddr *)&cliAddr, &len);
	printf("%s", recvP.msg);

	strcpy(sendP.msg, "This is server");
	sendP.idx = 0;
	while (1) {
		recvfrom(sockfd, &recvP, sizeof(recvP), 0, (struct sockaddr *)&cliAddr, &len);
		++sendP.idx;
		sendto(sockfd, &sendP, sizeof(sendP), 0, (struct sockaddr *)&cliAddr, sizeof(cliAddr));
	}
	// strcpy(sendP.msg, "This is server\n");
	// sendto(sockfd, &sendP, sizeof(sendP), 0, (struct sockaddr *)&cliAddr, sizeof(cliAddr));

	// setsockopt(sockfd, SOL_SOCKET, SO_RCVTIMEO, &timeout, sizeof(timeout));
	/*
	while (1) {
		if (recvfrom(sockfd, &recvP, sizeof(recvP), 0, (struct sockaddr *)&cliAddr, &len) == -1)
			printf("Timeout\n");
		else {
			strcpy(sendP.msg, recvP.msg);
			sendto(sockfd, &sendP, sizeof(sendP), 0, (struct sockaddr *)&cliAddr, sizeof(cliAddr));
			sendto(sockfd, &sendP, sizeof(sendP), 0, (struct sockaddr *)&cliAddr, sizeof(cliAddr));
			printf("%s\n", sendP.msg);
			if (!strcmp(recvP.msg, "exit") || !strcmp(recvP.msg, "Exit"))
				break;
			printf("%s\n", recvP.msg);
		}
	}
	*/

	// strcpy(sendP.msg, "Yet another msg\n");
	// sleep(5);
	// sendto(sockfd, &sendP, sizeof(sendP), 0, (struct sockaddr *)&cliAddr, sizeof(cliAddr));

	close(sockfd);
}
