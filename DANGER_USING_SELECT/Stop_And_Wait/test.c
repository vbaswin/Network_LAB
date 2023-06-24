#include <stdio.h>

int main() {
	int n;
	char s[100];
	scanf("%d %[^\n]s", &n, s);
	printf("%d\n%s", n, s);

	return 0;
}