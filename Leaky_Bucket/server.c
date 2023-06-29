#include <arpa/inet.h>
#include <netinet/in.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <time.h>
#include <unistd.h>

typedef struct PACK {
	int idx;
	struct PACK *next;
} pack;

int Enqueue(pack ***head, int idx, int bucket_size) {
	pack *newNode = (pack *)malloc(sizeof(pack));
	newNode->idx = idx;
	newNode->next = NULL;
	if (!**head)
		**head = newNode;
	else {
		pack *temp;
		int sz = 0;
		for (temp = **head; temp->next != NULL; temp = temp->next, ++sz) {
			if (sz > bucket_size) {
				printf("Bucket full %d!! cannot push\n", sz);
				free(newNode);
				return 0;
			}
		}
		temp->next = newNode;
	}
	printf("Packet [ %d ] Reached Bucket\n", idx);
	return 1;
}

int Dequeue(pack ***head) {
	if (!**head) {
		printf("Emtpy queue!!\n");
		return -1;
	}
	int idx = (**head)->idx;
	pack *temp;
	temp = (**head)->next;
	free(**head);
	**head = temp;
	printf("Packet [ %d ] sent from Bucket\n", idx);
	return idx;
}

void Display(pack **head) {
	if (!*head) {
		printf("Empty queue!!\n");
		return;
	}
	pack *temp;
	for (temp = *head; temp != NULL; temp = temp->next)
		printf("%d ", temp->idx);
	printf("\n");
}

void push_to_bucket(pack **head, int rand_val[], int no_of_packets, int bucket_size) {
	int temp_no = no_of_packets, to_push, idx = 0;

	for (int i = 0; i < 100 && temp_no; ++i) {
		if (rand_val[i] > temp_no)
			to_push = temp_no;
		else
			to_push = rand_val[i];

		printf("to_push: %d\n", to_push);
		for (int j = 0; j < to_push; ++j, ++idx)
			Enqueue(&head, idx, bucket_size);
		temp_no -= to_push;
		Display(head);
		// sleep(1);
	}
	idx = -1;
	while (!Enqueue(&head, idx, bucket_size))
		;
	Display(head);
}

void leak_from_bucket(pack **head, int max_recv_speed, int no_of_packets, int sockfd, struct sockaddr_in cliaddr) {
	int pack;
	for (int i = 0; i < no_of_packets + 1;) {
		for (int j = 0; j < max_recv_speed && i < no_of_packets + 1; ++j, ++i) {
			pack = Dequeue(&head);
			sendto(sockfd, &pack, sizeof(pack), 0, (struct sockaddr *)&cliaddr, sizeof(cliaddr));
		}
		sleep(1);
	}
}

void calc_rand(int rand_val[], int max_send_speed) {
	srand(time(NULL));
	for (int i = 0; i < 100; ++i)
		rand_val[i] = (rand() % max_send_speed) + 1;
}

void chatLoop(int sockfd, struct sockaddr_in cliaddr) {
	pack *head = NULL;
	struct timeval timeout;
	timeout.tv_sec = 0;
	timeout.tv_usec = 0;

	socklen_t len = sizeof(cliaddr);

	int max_send_speed, max_recv_speed, no_of_packets, bucket_size;
	printf("Enter\nMax senders speed:");
	scanf("%d", &max_send_speed);
	printf("Max receivers speed: ");
	scanf("%d", &max_recv_speed);
	printf("No of packets: ");
	scanf("%d", &no_of_packets);
	printf("Bucket_size: ");
	scanf("%d", &bucket_size);

	int rand_val[100];
	calc_rand(rand_val, max_send_speed);


	// making recvfrom blocking - to prevent invalid address resoultion -> due to timeout
	/*
	if (setsockopt(sockfd, SOL_SOCKET, SO_RCVTIMEO, &timeout, sizeof(timeout)) < 0) {
		perror("Setsockopt failed");
		return;
	}
	*/

	char addRes[100];
	// initial address resolution
	recvfrom(sockfd, addRes, sizeof(addRes), 0, (struct sockaddr *)&cliaddr, &len);

	pthread_t push_thread, send_thread;
	// ThreadArgs

	// pthread_create(&push_thread, NULL, push_to_bucket, )

	push_to_bucket(&head, rand_val, no_of_packets, bucket_size);
	leak_from_bucket(&head, max_recv_speed, no_of_packets, sockfd, cliaddr);
	/*

	int pack = 0;
	for (int i = 0; i < 10; ++i) {
		for (int j = 1; j < 5; ++j) {
			pack += j;
			sendto(sockfd, &pack, sizeof(pack), 0, (struct sockaddr *)&cliaddr, sizeof(cliaddr));
		}
		sleep(1);
	}
	*/

	/*
	// setting to real timeout 5s
	timeout.tv_sec = 5;
	setsockopt(sockfd, SOL_SOCKET, SO_RCVTIMEO, &timeout, sizeof(timeout));

	int n;
	printf("\nEnter no of packets: ");
	scanf("%d", &n);
	Packets sendP[n];

	printf("Enter predicted delay and packet msg\n");
	for (int i = 0; i < n; ++i) {
		printf("[ %d ]: ", i + 1);
		scanf("%d %[^\n]s", &sendP[i].delay, sendP[i].msg);
		sendP[i].idx = i;
		sendP[i].last = 0;
	}
	sendP[n - 1].last = 1;


	strcpy(addRes, "Entering info");
	sendto(sockfd, addRes, sizeof(addRes), 0, (struct sockaddr *)&cliaddr, sizeof(cliaddr));

	int ack;
	printf("\n\n");

	int i = 0;
	while (i < n) {
		sendto(sockfd, &sendP[i], sizeof(sendP[i]), 0, (struct sockaddr *)&cliaddr, sizeof(cliaddr));
		printf("Packet [ %d ] sent", i + 1);
		fflush(stdout);
		++i;

		if (recvfrom(sockfd, &ack, sizeof(ack), 0, (struct sockaddr *)&cliaddr, &len) == -1) {
			printf("\tTimeout!!. Resending again...\n");
			sendP[--i].delay = 3;
			continue;
		}

		if (ack != i) {
			printf("\tInvalid Ack. Resending again...\n");
			--i;
			continue;
		}
		printf("\tAck received: %d\n", ack);
	}
	printf("\nServer Exiting...\n");
	*/
}

int main() {
	freopen("c.in", "r", stdin);
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
	if (bind(sockfd, (struct sockaddr *)&servaddr, sizeof(servaddr)) == -1) {
		printf("Failed to bind\n");
		close(sockfd);
		exit(0);
	} else
		printf("Bind Successfull\n");

	chatLoop(sockfd, servaddr);

	close(sockfd);

	return 0;
}
