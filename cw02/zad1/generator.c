#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int main(int argc, char *args[]) {
	const char* fileName;
	char *buffer;
	char tmp;
	int size, amount, wr, i ,j;
	FILE *output;

	
	
	srand(time(NULL));
	
	if(argc != 4) {
		fprintf(stderr, "usage: <file name> <size of record> <number of records>\n");
		exit(EXIT_FAILURE);
	}
	
	fileName = args[1];
	size = atoi(args[2]);
	amount = atoi(args[3]);
	
	output = fopen(fileName, "w");
	if(output == NULL) {
		fprintf(stderr, "fopen error\n");
		exit(EXIT_FAILURE);
	}
	
	buffer = malloc(sizeof(char) * size);
	if(buffer == NULL) {
		fprintf(stderr, "malloc error\n");
		exit(EXIT_FAILURE);
	}
	
	for(i = 0; i < amount; i++) {
		for(j = 0; j < size; j++) {
			tmp = 'A' + rand() % 26;
			buffer[j] = tmp;
		}
		wr = fwrite(buffer, sizeof(char), size, output);
		if(wr != size) {
			fprintf(stderr, "fwrite error\n");
		}
	}
	
	fclose(output);
	
	return 0;
}
