#include <stdio.h>
#include <stdlib.h>

typedef struct packets {
	char msg[200];
	int idx;
} Packets;

typedef struct NODE {
	int data;
	struct NODE *next;
} node;

void Enqueue(node **head, int data) {
	node *newNode = (node *)malloc(sizeof(node));
	newNode->data = data;
	newNode->next = NULL;
	if (!*head)
		*head = newNode;
	else {
		node *temp;
		for (temp = *head; temp->next != NULL; temp = temp->next)
			;
		temp->next = newNode;
	}
}

void Dequeue(node **head) {
	if (!*head) {
		printf("Emtpy queue!!\n");
		return;
	} else {
		node *temp;
		temp = (*head)->next;
		free(*head);
		*head = temp;
	}
}

void Display(node **head) {
	if (!*head) {
		printf("Empty queue!!\n");
		return;
	}
	node *temp;
	for (temp = *head; temp != NULL; temp = temp->next)
		printf("%d ", temp->data);
	printf("\n");
}

int main() {
	node *head = NULL;

	Enqueue(&head, 22);
	Enqueue(&head, 34);
	Enqueue(&head, 35);
	Enqueue(&head, 24);
	Display(&head);
	Dequeue(&head);
	Dequeue(&head);
	Display(&head);
	Dequeue(&head);
	Dequeue(&head);
	Display(&head);
	Dequeue(&head);

	return 0;
}
