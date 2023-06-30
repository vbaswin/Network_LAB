#include <arpa/inet.h>
#include <netinet/in.h>
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

typedef struct {
	char msg[100];
} pack;

int main() {
	struct sockaddr_in servaddr, cliaddr;

	servaddr.sin_family = AF_INET;
	servaddr.sin_addr.s_addr = inet_addr("127.0.0.1");
	servaddr.sin_port = htons(8080);

	int sockfd = socket(AF_INET, SOCK_DGRAM, 0);
	int connfd = connect(sockfd, (struct sockaddr *)&servaddr, sizeof(servaddr));

	pack sendP, recvP;
	strcpy(sendP.msg, "Hi");

	sendto(sockfd, &sendP, sizeof(sendP), 0, (struct sockaddr *)NULL, sizeof(servaddr));
	recvfrom(sockfd, &recvP, sizeof(recvP), 0, (struct sockaddr *)NULL, NULL);

	printf("%s\n", recvP.msg);

	return 0;
}
