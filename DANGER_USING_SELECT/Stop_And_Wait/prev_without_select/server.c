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
	int resend;
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
	
	
	Packets recvP;
	recvfrom(sockfd, &recvP, 100, 0, (struct sockaddr *)&cliaddr, &len);
	
	printf("%s\n", recvP.msg);
	
	
	int n;
	printf("Enter no of packets to send: ");
	scanf("%d", &n);
	
	sendto(sockfd, &n, sizeof(int), 0, (struct sockaddr *)&cliaddr , sizeof(servaddr));

	
	Packets sendP[n];
	getchar();
	
	for (int i = 0; i < n; ++i) {
		printf("Enter msg[%d]: ", i);
		scanf("%[^\n]s", sendP[i].msg);
		sendP[i].idx = 1000 + i;
		sendP[i].resend = 0;
		getchar();
	}

	int ack, idx = 0;
	
	printf("\n");
	while(idx < n) {
		sendto(sockfd, &sendP[idx], sizeof(sendP[idx]), 0, (struct sockaddr *)&cliaddr , sizeof(servaddr));
		
		recvfrom(sockfd, &ack, sizeof(int) , 0, (struct sockaddr *)&cliaddr, &len);
		
		if (ack == (sendP[idx].idx + 1)) {
			printf("Packet[%d] ack received!\n", idx);
			++idx;
			sendP[idx].resend = 0;
		} else {
			printf("Incorrect ack for packet[%d], resending!\n", idx);
			sendP[idx].resend = 1;
		}
	}

	
	close(sockfd);

	return 0;
}
