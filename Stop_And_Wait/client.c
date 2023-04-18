// client

#include <stdio.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

typedef struct packets {
	char msg[200];
	int idx;
} Packets ;

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
	
	Packets recvP;
	
	char msg[] = "Start transmitting";
	
	
	// initial address resolution
	sendto(sockfd, msg, sizeof(msg), 0, (struct sockaddr *)NULL, sizeof(servaddr));
	
	int ack;
	while (1) {
		recvfrom(sockfd, &recvP, sizeof(recvP), 0, (struct sockaddr *)NULL, NULL);
		if (!strcmp(recvP.msg, "EXIT")) {
			printf("Client Exiting...");
			break;
		}
			
		printf("%s\n%d\n", recvP.msg, recvP.idx);
		ack = recvP.idx;
		sendto(sockfd, &ack,4, 0, (struct sockaddr *)NULL, sizeof(servaddr));
	}
	
	
	close(sockfd);

	return 0;
}
