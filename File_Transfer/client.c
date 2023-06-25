// client

#include <arpa/inet.h>
#include <netinet/in.h>
#include <signal.h>
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

void chatLoop(int sockfd) {
	Packets sendP, recvP;

	struct timeval timeout;
	timeout.tv_sec = 10;
	timeout.tv_usec = 0;

	if (setsockopt(sockfd, SOL_SOCKET, SO_RCVTIMEO, &timeout, sizeof(timeout)) < 0) {
		perror("Setsockopt failed");
		return;
	}

	pid_t server_pid = 0;
	recv(sockfd, &server_pid, sizeof(server_pid), 0);

	printf("\n");
	char file_name[200], exist_msg[100], new_file_name[200], copy_file_name[200];
	while (1) {
		printf("Enter file name: ");
		scanf("%[^\n]s", file_name);
		getchar();
		send(sockfd, file_name, sizeof(file_name), 0);
		recv(sockfd, exist_msg, sizeof(exist_msg), 0);
		if (!strcmp(exist_msg, "exists"))
			break;
	}

	int i;
	for (i = 0; i < strlen(file_name) && file_name[i] != '.'; ++i)
		new_file_name[i] = file_name[i];

	new_file_name[i] = 0;
	sprintf(new_file_name, "%s_download.txt", new_file_name);

	printf("\nDownloading file %s from server pid: %d\n", file_name, server_pid);
	FILE *fp = fopen(new_file_name, "w+");

	char line[200];
	while (1) {
		recv(sockfd, line, sizeof(line), 0);
		if (!strcmp(line, "EOF"))
			break;
		fprintf(fp, "%s", line);
	}
	printf("File downloaded successfully!!\nSaved as %s\n", new_file_name);

	fclose(fp);
}

int main() {
	int sockfd, status, connfd;
	struct sockaddr_in servaddr, cliaddr;

	// socket creation
	if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
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

	chatLoop(sockfd);

	close(sockfd);

	return 0;
}
