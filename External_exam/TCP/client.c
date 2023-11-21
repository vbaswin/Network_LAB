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
	struct sockaddr_in servaddr;
	int sockfd;
	if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
		printf("Socket creation failed!\n");
		return 0;
	} else
		printf("Socket creation successfull!\n");

	servaddr.sin_family = AF_INET;
	servaddr.sin_port = htons(8080);
	servaddr.sin_addr.s_addr = inet_addr("127.0.0.1");

	if (connect(sockfd, (struct sockaddr *)&servaddr, sizeof(servaddr)) == -1) {
		printf("Socket connection failed!\n");
		return 0;
	} else
		printf("Socket connection successfull!\n");

	PACKETS sendP, recvP;
	strcpy(sendP.msg, "This is client");

	recv(sockfd, &recvP, sizeof(recvP), 0);
	printf("%s\n", recvP.msg);
	sleep(5);
	send(sockfd, &sendP, sizeof(sendP), 0);

	close(sockfd);

	return 0;
}
