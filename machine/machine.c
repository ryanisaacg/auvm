#include "machine.h"

#include "instructions.h"
#include <string.h>

void initialize_hardware();
void load_disk();
void write_disk();
void execute_bytecode(ubyte *data, size_t length);
size_t execute_statement(ubyte *data, size_t length, size_t position);

int main() {
	initialize_hardware();
	load_disk();
	write_disk();
}

void initialize_hardware() {
	disk = fopen("harddisk", "r+");
	ram = malloc(RAM_SIZE);
	if(disk == NULL) {
		fprintf(stderr, "Failed to initialize hard disk.");
		exit(-1);
	}
	if(ram == NULL) {
		fprintf(stderr, "Failed to initialize RAM.");
		exit(-1);
	}
	disk_buffer = NULL;
	disk_size = 0;
}

void load_disk() {
	if(disk_buffer != NULL) {
		free(disk_buffer);
	}
	disk_buffer = malloc(1024);
	disk_size = 1024;
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
	disk_size = disk_position;
}

void write_disk() {
	fwrite(disk_buffer, 1, disk_size, disk);
}

