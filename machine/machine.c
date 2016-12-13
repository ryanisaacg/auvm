#include "machine.h"

#include "instructions.h"
#include <string.h>


void load_disk();
void write_disk();
void execute_bytecode(ubyte *data, size_t length);
size_t execute_statement(ubyte *data, size_t length, size_t position);

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

int main() {
	load_disk();
	write_disk();
}
