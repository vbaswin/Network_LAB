#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int main() {
	char *msg = "hi I am Aswin", *msg2 = (char *)malloc(sizeof(char));
	strcpy(msg2, msg);
	printf("%s\n", msg2);
}
