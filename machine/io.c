#include "io.h"

#include <stdlib.h>
#include <string.h>

#define BUFFER_SIZE 1024

unsigned char *read_file(FILE *file, size_t *length) {
	unsigned char buffer[BUFFER_SIZE];
	unsigned char *data = malloc(BUFFER_SIZE);
	if(data == NULL) {
		fprintf(stderr, "Memory reallocation failed in read_file\n");
	}
	size_t position = 0, size = BUFFER_SIZE;
	while(!feof(file)) {
		size_t read = fread(buffer, 1, BUFFER_SIZE, file);
		if(position + read < size) {
			memcpy(data + position, buffer, read);
		} else {
			data = realloc(data, size * 2);
			if(data == NULL) {
				fprintf(stderr, "Memory reallocation failed in read_file\n");
			}
			size *= 2;
			memcpy(data + position, buffer, read);
		}
		position += read;
	}
	*length = position;
	return data;
}
