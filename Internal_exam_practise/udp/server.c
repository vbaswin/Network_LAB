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
	servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
	servaddr.sin_port = htons(8080);

	int sockfd = socket(AF_INET, SOCK_DGRAM, 0);

	bind(sockfd, (struct sockaddr *)&servaddr, sizeof(servaddr));

	pack recvP, sendP;
	socklen_t len = sizeof(cliaddr);
	recvfrom(sockfd, &recvP, sizeof(recvP), 0, (struct sockaddr *)&cliaddr, &len);

	strcpy(sendP.msg, "How are you");
	sendto(sockfd, &sendP, sizeof(sendP), 0, (struct sockaddr *)&cliaddr, sizeof(cliaddr));

	printf("%s\n", recvP.msg);

	close(sockfd);

	return 0;
}
