#include "instructions.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef unsigned char ubyte;
typedef char byte;

FILE *disk;
ubyte *ram, *disk_buffer;
size_t ram_size, disk_size;
int registers[8];

void load_disk() {
	ubyte disk_copy[1024];
	size_t disk_position = 0;
	while(!feof(disk)) {
		fread(disk_copy, 1, 1024, disk);
		if(disk_position + 1024 < disk_size) {
			memcpy(disk_buffer + disk_position, disk_copy, 1024);
			disk_position += 1024;
		} else {
			void *old = disk_buffer;
			disk_buffer = realloc(disk_buffer, disk_size * 2);
			free(old);
			disk_size *= 2;
			memcpy(disk_buffer + disk_position, disk_copy, 1024);
			disk_position += 1024;
		}
	}
}

void write_disk() {
	fwrite(disk_buffer, 1, disk_size, disk);
}

