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
	servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
	servaddr.sin_port = htons(8080);

	int sockfd = socket(AF_INET, SOCK_STREAM, 0);
	int reuse = 1;
	setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &reuse, sizeof(reuse));
	bind(sockfd, (struct sockaddr *)&servaddr, sizeof(servaddr));

	listen(sockfd, 0);
	// int connfd = connect(sockfd, (struct sockaddr *)&cliaddr, )
	socklen_t len = sizeof(cliaddr);
	int connfd = accept(sockfd, (struct sockaddr *)&cliaddr, &len);

	pack sendP, recvP;
	strcpy(sendP.msg, "Hi");

	send(connfd, &sendP, sizeof(sendP), 0);
	recv(connfd, &recvP, sizeof(recvP), 0);
	printf("%s\n", recvP.msg);

	close(sockfd);
	return 0;
}
