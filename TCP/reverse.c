#include <stdio.h>
#include <string.h>

int main() {
	char buff[20], rev[20];

	strcpy(buff, "Aswin\n");
	
	int n = strlen(buff);
	for (int i = 0; i < n; ++i)
		rev[i] = buff[n - i - 1];
	
	printf("%s\n", rev);

	return 0;	
}
