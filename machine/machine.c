#include "machine.h"

#include "instructions.h"
#include <stdbool.h>
#include <string.h>

void initialize_hardware();
void load_disk();
void write_disk();
void execute_bytecode(ubyte *data);
void execute_statement(ubyte *data, size_t position, size_t *new_position, bool *keep_going);
number get_value(ubyte *instruction);
void set_value(ubyte *instruction, number value);

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
	if(disk_buffer == NULL) {
		fprintf(stderr, "Memory allocation failed in load_disk\n");
	}
	disk_size = 1024;
	ubyte disk_copy[1024];
	size_t disk_position = 0;
	while(!feof(disk)) {
		size_t disk_read = fread(disk_copy, 1, 1024, disk);
		if(disk_position + disk_read < disk_size) {
			memcpy(disk_buffer + disk_position, disk_copy, 1024);
		} else {
			disk_buffer = realloc(disk_buffer, disk_size * 2);
			if(disk_buffer == NULL) {
				fprintf(stderr, "Memory reallocation failed in load_disk\n");
			}
			disk_size *= 2;
			memcpy(disk_buffer + disk_position, disk_copy, 1024);
		}
		disk_position += disk_read;
	}
	disk_size = disk_position;
}

void write_disk() {
	fwrite(disk_buffer, 1, disk_size, disk);
}

int get_value(ubyte *instruction) {
	ubyte type = instruction[0];
	number value, pointer;
	switch(type) {
		case REGISTER:
			memcpy(&value, instruction + 1, 4);
			return registers[value];
			break;
		case REGISTER_VALUE:
		case POINTER:
			memcpy(&pointer, instruction + 1, 4);
			memcpy(&value, ram + pointer, 4);
			return value;
		case CONSTANT:
			memcpy(&value, instruction + 1, 4);
			return value;
	}
}

void set_value(ubyte *instruction, int value) {
	ubyte type = instruction[0];
	number index, pointer;
	switch(type) {
		case REGISTER:
			memcpy(&index, instruction + 1, 4);
			registers[index] = value;
			break;
		case REGISTER_VALUE:
			memcpy(&index, instruction + 1, 4);
			pointer = registers[index];
			memcpy(ram + pointer, &value, 4);
			break;
		case POINTER:
			memcpy(&pointer, instruction + 1, 4);
			memcpy(ram + pointer, &value, 4);
			break;
		case CONSTANT:
			fprintf(stderr, "Cannot set the value of a constant.\n");
			exit(-1);
			break;
	}
}

void execute_bytecode(ubyte *data) {
	size_t i = 0;
	bool keep_going = true;
	while(keep_going) {
		execute_statement(data, i, &i, &keep_going);
	}
}

void execute_statement(ubyte *data, size_t position, size_t *new_position, bool *keep_going) {
	data += position;
	ubyte command = data[0];
}
