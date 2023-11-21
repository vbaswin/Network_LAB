#include <arpa/inet.h>
#include <netinet/in.h>
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

typedef struct {
	char msg[100];
} PACKETS;

int main() {
	struct sockaddr_in servAddr;
	int sockfd;
	if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) == -1) {
		printf("Socket creation Failed!\n");
		return 0;
	} else
		printf("Socket creation successfull!\n");

	servAddr.sin_family = AF_INET;
	servAddr.sin_port = htons(8080);
	servAddr.sin_addr.s_addr = inet_addr("127.0.0.1");

	PACKETS sendP, recvP;

	struct timeval timeout;
	timeout.tv_sec = 5;
	timeout.tv_usec = 0;

	strcpy(sendP.msg, "This is client\n");
	sendto(sockfd, &sendP, sizeof(sendP), 0, (struct sockaddr *)&servAddr, sizeof(servAddr));

	socklen_t len = sizeof(servAddr);
	recvfrom(sockfd, &recvP, sizeof(recvP), 0, (struct sockaddr *)&servAddr, &len);
	printf("%s", recvP.msg);
	strcpy(sendP.msg, "Another msg\n");
	// sleep(5);
	sendto(sockfd, &sendP, sizeof(sendP), 0, (struct sockaddr *)&servAddr, sizeof(servAddr));

	setsockopt(sockfd, SOL_SOCKET, SO_RCVTIMEO, &timeout, sizeof(timeout));
	if (recvfrom(sockfd, &recvP, sizeof(recvP), 0, (struct sockaddr *)&servAddr, &len) == -1)
		printf("Timeout\n");
	else
		printf("%s", recvP.msg);

	close(sockfd);
}
