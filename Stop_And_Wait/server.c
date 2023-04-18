// server

#include <stdio.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>

typedef struct packets {
	char msg[200];
	int idx;
} Packets;

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

	// bind
	if(bind(sockfd, (struct sockaddr *)&servaddr, sizeof(servaddr)) == -1) {
		printf("Failed to bind\n");
		close(sockfd);
		exit(0);
	} else 
		printf("Bind Successfull\n");

	socklen_t len = sizeof(cliaddr);
	
	int sz;
	printf("Enter no of packets to send: ");
	scanf("%d", &sz);
	
	char msg[100];
	recvfrom(sockfd, msg, 100, 0, (struct sockaddr *)&cliaddr, &len);
	printf("msg: %s\n", msg);
	
	Packets sendP[sz + 1];
	
	gets();
	for (int i = 0; i < sz; ++i) {
		printf("Enter msg[%d]:", i);
		gets(&sendP[i].msg);
	}
	;
	// for (int i = 0; i < sz; ++i) 
	//	printf("%s\n", sendP[i].msg);




	for (int i = 0; i < sz; ++i) {
		sendto(sockfd, &sendP[i], sizeof(sendP), 0, (struct sockaddr *)&cliaddr , sizeof(servaddr));
	
		int ack;
		recvfrom(sockfd, &ack, 4, 0, (struct sockaddr *)&cliaddr, &len);
		printf("%d\n", ack);
	}
	
	printf("Server Exiting...");
	strcpy(msg,"EXIT\0");
	sendto(sockfd, msg, sizeof(msg), 0, (struct sockaddr *)&cliaddr , sizeof(servaddr));
	
	
	close(sockfd);

	return 0;
}
