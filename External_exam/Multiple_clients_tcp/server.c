#include <arpa/inet.h>
#include <netinet/in.h>
#include <pthread.h>
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

pthread_mutex_t mutex;

typedef struct {
	char msg[100];
} PACKETS;

typedef struct {
	int connfd;
	PACKETS sendP;
	PACKETS *recvP;
} threadArgs;

// void chatFunc(int connfd, PACKETS sendP, PACKETS *recvP) {
// needs to be void * as return type
void *chatFunc(void *args) {
	threadArgs *arg = (threadArgs *)args;

	send((*arg).connfd, &(*arg).sendP, sizeof((*arg).sendP), 0);
	recv((*arg).connfd, &*((*arg).recvP), sizeof(*((*arg).recvP)), 0);
}

int main() {
	struct sockaddr_in servaddr, cliaddr;
	int sockfd;
	if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
		printf("Socket creation failed!\n");
		return 0;
	} else
		printf("Socket creation successfull!\n");


	servaddr.sin_family = AF_INET;
	servaddr.sin_port = htons(8080);
	servaddr.sin_addr.s_addr = htonl(INADDR_ANY);


	int reuse = 1;
	setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &reuse, sizeof(reuse));

	if (bind(sockfd, (struct sockaddr *)&servaddr, sizeof(servaddr))) {
		printf("Socket binding failed!\n");
		return 0;
	} else
		printf("Socket binding successfull!\n");
	if (listen(sockfd, 0)) {
		printf("Socket listening failed!\n");
		return 0;
	} else
		printf("Socket listening ...!\n");


	int connfd[2];
	socklen_t len = sizeof(cliaddr);

	for (int i = 0; i < 2; ++i) {
		// accept the connection requests
		if ((connfd[i] = accept(sockfd, (struct sockaddr *)&cliaddr, &len)) == -1) {
			printf("Socket connection failed!\n");
			return 0;
		} else
			printf("Socket connection successfull!\n");
	}

	PACKETS sendP1, sendP2, recvP1, recvP2;

	strcpy(sendP1.msg, "This is server, Hi client1\n");
	strcpy(sendP2.msg, "This is server, Hi client2\n");

	threadArgs arg1, arg2;
	arg1.connfd = connfd[0];
	arg1.sendP = sendP1;
	arg1.recvP = &recvP1;

	arg2.connfd = connfd[1];
	arg2.sendP = sendP2;
	arg2.recvP = &recvP2;

	pthread_t thread1, thread2;

	pthread_mutex_init(&mutex, NULL);

	pthread_create(&thread1, NULL, chatFunc, (void *)&arg1);
	pthread_create(&thread2, NULL, chatFunc, (void *)&arg2);
	// chatFunc((void *)&arg1);
	// chatFunc((void *)&arg2);


	// second para -> return from thread
	pthread_join(thread1, NULL);
	pthread_join(thread2, NULL);

	printf("%s\n", recvP1.msg);
	printf("%s\n", recvP2.msg);
	pthread_mutex_destroy(&mutex);


	/*
	struct timeval timeout;
	timeout.tv_sec = 5;
	timeout.tv_usec = 0;

	setsockopt(connfd, SOL_SOCKET, SO_RCVTIMEO, &timeout, sizeof(timeout));
	if (recv(connfd, &recvP, sizeof(recvP), 0) == -1)
		printf("Timeout\n");
	else
		printf("%s\n", recvP.msg);
	*/

	// recv(connfd[1], &recvP, sizeof(recvP), 0)
	// printf("%s\n", recvP.msg);

	close(connfd[0]);
	close(connfd[1]);
	close(sockfd);
	return 0;
}
