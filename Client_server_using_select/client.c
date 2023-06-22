#include <arpa/inet.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <time.h>
#include <unistd.h>

typedef struct packets {
	char msg[200];
	int idx;
} Packets;

void chatLoop(int sockfd, struct sockaddr_in servaddr) {
	Packets sendP, recvP;

	struct timeval timeout;
	timeout.tv_sec = 10;	// no of seconds for timeout(or wait)
	timeout.tv_usec = 0;	// no of microseconds for timeout

	fd_set read_fds;
	FD_ZERO(&read_fds);
	FD_SET(sockfd, &read_fds);

	int select_result;
	printf("\n");

	while (1) {
		memset(sendP.msg, 0, sizeof(sendP.msg));

		printf("Enter msg: ");
		scanf("%[^\n]s", sendP.msg);
		getchar();
		// initial address resolution
		sendto(sockfd, &sendP, sizeof(sendP), 0, (struct sockaddr *)NULL, sizeof(servaddr));

		if (!strcmp(sendP.msg, "Exit") || !strcmp(sendP.msg, "exit")) {
			printf("\nClient Exiting...");
			break;
		}

		select_result = select(sockfd + 1, &read_fds, NULL, NULL, &timeout);

		if (select_result == -1) {
			perror("Select");
			exit(EXIT_FAILURE);
		} else if (select_result) {
			recvfrom(sockfd, &recvP, sizeof(recvP), 0, (struct sockaddr *)NULL, NULL);
			if (!strcmp(recvP.msg, "Exit") || !strcmp(recvP.msg, "exit")) {
				printf("\nClient Exiting...");
				break;
			}
			printf("\tmsg from Server: %s\n", recvP.msg);
		} else {
			printf("\nTimeout!!\n");
			break;
		}
	}
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

	chatLoop(sockfd, servaddr);

	close(sockfd);

	return 0;
}
