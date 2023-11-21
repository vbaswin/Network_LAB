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
	struct sockaddr_in servaddr, cliaddr;
	int sockfd;
	if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
		printf("Socket creation failed!\n");
		return 0;
	} else
		printf("Socket creation successfull!\n");


	servaddr.sin_family = AF_INET;
	servaddr.sin_port = htons(8080);
	servaddr.sin_addr.s_addr = htonl(INADDR_ANY);


	int reuse = 1;
	setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &reuse, sizeof(reuse));

	if (bind(sockfd, (struct sockaddr *)&servaddr, sizeof(servaddr))) {
		printf("Socket binding failed!\n");
		return 0;
	} else
		printf("Socket binding successfull!\n");

	if (listen(sockfd, 0)) {
		printf("Socket listening failed!\n");
		return 0;
	} else
		printf("Socket listening ...!\n");

	int connfd;
	socklen_t len = sizeof(cliaddr);
	// accept the connection requests
	if ((connfd = accept(sockfd, (struct sockaddr *)&cliaddr, &len)) == -1) {
		printf("Socket connection failed!\n");
		return 0;
	} else
		printf("Socket connection successfull!\n");

	PACKETS sendP, recvP;

	strcpy(sendP.msg, "This is server");
	send(connfd, &sendP, sizeof(sendP), 0);

	struct timeval timeout;
	timeout.tv_sec = 5;
	timeout.tv_usec = 0;

	setsockopt(connfd, SOL_SOCKET, SO_RCVTIMEO, &timeout, sizeof(timeout));
	if (recv(connfd, &recvP, sizeof(recvP), 0) == -1)
		printf("Timeout\n");
	printf("%s\n", recvP.msg);

	close(sockfd);
	return 0;
}
