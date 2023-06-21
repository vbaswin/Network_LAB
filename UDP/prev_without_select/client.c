// client

#include <stdio.h>
#include <unistd.h>
#include <sys/socket.h>
// #include <sys/types.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

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

	time_t t;
	time(&t);
	char buff[20];
	char* msg = ctime(&t);
	
    	int len;
sendto(sockfd, msg, 100, 0, (struct sockaddr *)NULL, sizeof(servaddr));

	recvfrom(sockfd, buff, sizeof(buff), 0, (struct sockaddr *)NULL, NULL);
	printf("%s\n", buff);
	
	strcpy(msg, "Client Exiting...");
	sendto(sockfd, msg, 100, 0, (struct sockaddr *)NULL, sizeof(servaddr));

	// closing socket
	close(sockfd);

	return 0;
}
