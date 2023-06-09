// client

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
} Packets;

void sendTime(int sockfd, struct sockaddr_in servaddr) {
	Packets recvP, sendP;
	strcpy(sendP.msg, "Address resolution");

	sendto(sockfd, &sendP, sizeof(sendP), 0, (struct sockaddr *)NULL, sizeof(servaddr));

	if (recvfrom(sockfd, &recvP, sizeof(recvP), 0, (struct sockaddr *)NULL, NULL) == -1)
		printf("Timeout!!. Resend again...\n");

	printf("\nTime from Server: %s\nClient Exiting...\n", recvP.msg);
}

int main() {
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

	sendTime(sockfd, servaddr);

	close(sockfd);

	return 0;
}
