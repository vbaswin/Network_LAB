#include <arpa/inet.h>
#include <netinet/in.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

typedef struct packets {
	char msg[200];
	int idx;
} Packets;

void chatLoop(int connfd) {
	struct timeval timeout;
	timeout.tv_sec = 10;
	timeout.tv_usec = 0;

	Packets recvP, sendP;

	if (setsockopt(connfd, SOL_SOCKET, SO_RCVTIMEO, &timeout, sizeof(timeout)) < 0) {
		perror("Setsockopt failed");
		return;
	}

	pid_t pid = getpid();
	send(connfd, &pid, sizeof(pid), 0);

	printf("\n");
	char file_name[200], exist_msg[] = "exists", not_exist_msg[] = "nexists";
	while (1) {
		if (recv(connfd, file_name, sizeof(file_name), 0) == -1) {
			printf("Timeout!!. Resend again...\n");
			continue;
		}
		printf("%s\t", file_name);
		if (access(file_name, F_OK) != -1) {
			printf("File exists\n");
			send(connfd, exist_msg, sizeof(exist_msg), 0);
			break;
		} else {
			printf("File does not exist\n");
			send(connfd, not_exist_msg, sizeof(not_exist_msg), 0);
		}
	}

	printf("\nSending file %s\n", file_name);

	FILE *fp = fopen(file_name, "r+");
	char line[200], end_of_line[] = "EOF";

	while (fgets(line, sizeof(line), fp)) {
		send(connfd, line, sizeof(line), 0);
	}
	send(connfd, end_of_line, sizeof(end_of_line), 0);
	printf("File send successfully!!\n");

	fclose(fp);
}

int main() {
	int sockfd, status, connfd;
	struct sockaddr_in servaddr, cliaddr;

	// AF_INET : AF -> Address Family AF_INET-> IPv4 address family
	if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
		printf("Socket creation failed\n");
		close(sockfd);
		exit(0);
	} else
		printf("Socket Creation Successfull\n");


	printf("Sockid: %d\n", sockfd);

	servaddr.sin_family = AF_INET;
	servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
	servaddr.sin_port = htons(8080);

	/*
		==> set the SO_REUSEADDR socket option to allow the immediate reuse of the address. This option allows a
		socket to bind to an address that is in the TIME_WAIT state <==
	*/

	int reuse = 1;
	if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &reuse, sizeof(reuse)) < 0) {
		perror("Failed to set SO_REUSEADDR");
		exit(EXIT_FAILURE);
	}

	// bind
	if (bind(sockfd, (struct sockaddr *)&servaddr, sizeof(servaddr)) == -1) {
		printf("Failed to bind\n");
		close(sockfd);
		exit(0);
	} else
		printf("Bind Successfull\n");

	if ((status = listen(sockfd, 5)) == -1) {
		printf("Error in listening\n");
		close(sockfd);
		exit(0);
	} else
		printf("Listening...\n");


	int len = sizeof(cliaddr);

	if ((connfd = accept(sockfd, (struct sockaddr *)&cliaddr, &len)) == -1) {
		printf("Error in accepting\n");
		close(sockfd);
		exit(0);
	} else
		printf("Accepting...\n");


	chatLoop(connfd);
	// printf("%d %d\n", sockfd, connfd); // they are different

	close(sockfd);

	return 0;
}
