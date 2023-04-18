#include <stdio.h>
#include <time.h>

int main() {
	time_t t;
	time(&t);
	printf("%s", ctime(&t));
	char *msg = ctime(&t);	
	printf("%s", msg);
	return 0;
}
