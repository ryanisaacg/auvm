#include "machine.h"

#include "io.h"
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
	execute_bytecode(bios);
	write_disk();
	fclose(disk);
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
	FILE* bios_file = fopen("bios", "r");
	bios = read_file(bios_file, &bios_size);
	fclose(bios_file);
}

void load_disk() {
	if(disk_buffer != NULL) {
		free(disk_buffer);
	}
	disk_buffer = malloc(1024);
	if(disk_buffer == NULL) {
		fprintf(stderr, "Memory allocation failed in load_disk\n");
	}
	disk_buffer = read_file(disk, &disk_size);
	fseek(disk, 0, SEEK_SET);
}

void write_disk() {
	fwrite(disk_buffer, 1, disk_size, disk);
}

number get_value(ubyte *instruction) {
	ubyte type = instruction[0];
	number value, pointer;
	switch(type) {
		case REGISTER:
			memcpy(&value, instruction + 1, 4);
			return registers[value];
		case REGISTER_VALUE:
		case POINTER:
			memcpy(&pointer, instruction + 1, 4);
			memcpy(&value, ram + pointer, 4);
			return value;
		case CONSTANT:
			memcpy(&value, instruction + 1, 4);
			return value;
		default:
			fprintf(stderr, "Failed switched statement in get_value");
			exit(-1);
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
	ubyte *arguments = data + 1;
	*keep_going = true;
	switch(command) {
		case MOV: {
			number source = get_value(arguments);
			set_value(arguments + 5, source);
			*new_position = position + 11;
		} break;
		case WTO: {
			number source = get_value(arguments);
			putc(source, stdout);
			*new_position = position + 5;
		} break;
	}
}
