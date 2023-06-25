#include <stdio.h>
#include <string.h>

int main() {
	char filename[] = "filename.txt", new_file_name[200];

	for (int i = 0; i < strlen(filename) && filename[i] != '.'; ++i)
		new_file_name[i] = filename[i];

	sprintf(new_file_name, "%s_download.txt", new_file_name);
	printf("%s", new_file_name);

	return 0;
}
