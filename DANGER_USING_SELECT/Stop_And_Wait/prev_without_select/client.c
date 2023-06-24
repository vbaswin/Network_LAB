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
	int resend;
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
	
	Packets sendP, recvP;
	
	strcpy(sendP.msg, "Client connected!");
	printf("%s\n", sendP.msg);
	
	
	// initial address resolution
	sendto(sockfd, &sendP, sizeof(sendP), 0, (struct sockaddr *)NULL, sizeof(servaddr));
	
	int n;
	
	recvfrom(sockfd, &n, sizeof(int), 0, (struct sockaddr *)NULL, NULL);
	
	printf("\n");

	int ack, idx = 0;
	
	while(idx < n) {
		recvfrom(sockfd, &recvP, sizeof(recvP), 0, (struct sockaddr *)NULL, NULL);
		
		if (!recvP.resend)
			++idx;
		
		printf("Packet msg: %s\n",recvP.msg);
			
		ack = recvP.idx + 1;
		
		sleep(1);
		sendto(sockfd, &ack, sizeof(ack), 0, (struct sockaddr *)NULL, sizeof(servaddr));
		
	}
	
	close(sockfd);

	return 0;
}
