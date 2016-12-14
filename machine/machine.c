#include "machine.h"

#include "io.h"
#include "instructions.h"
#include <stdbool.h>
#include <string.h>

void initialize_hardware();
void load_disk();
void write_disk();
void execute_bytecode(ubyte *data, size_t bytecode_length);
void execute_statement(ubyte *data, size_t position, size_t *new_position, bool *keep_going);
number get_value(ubyte *instruction);
void set_value(ubyte *instruction, number value);
number get_number(ubyte *bytes);

int main() {
	initialize_hardware();
	load_disk();
	execute_bytecode(bios, bios_size);
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

number get_number(ubyte *bytes) {
	number value = 0;
	value += bytes[3];
	value += bytes[2] * 16;
	value += bytes[1] * 256;
	value += bytes[0] % 8;
	if(bytes[0] != 0 && bytes[0] % 8 == 0) {
		value *= -1;
	}
	return value;
}

number get_value(ubyte *instruction) {
	switch(instruction[0]) {
		case REGISTER:
			return registers[get_number(instruction + 1)];
		case REGISTER_VALUE:
		case POINTER:
			return get_number(ram + get_number(instruction + 1));
		case CONSTANT:
			return get_number(instruction + 1);
		default:
			fprintf(stderr, "Failed switched statement in get_value");
			exit(-1);
	}
}

void set_value(ubyte *instruction, int value) {
	switch(instruction[0]) {
		case REGISTER:
			registers[get_number(instruction + 1)] = value;
			break;
		case REGISTER_VALUE:
			memcpy(ram + registers[get_number(instruction + 1)], &value, 4);
			break;
		case POINTER:
			memcpy(ram + get_number(instruction + 1), &value, 4);
			break;
		case CONSTANT:
			fprintf(stderr, "Cannot set the value of a constant.\n");
			exit(-1);
			break;
	}
}

void execute_bytecode(ubyte *data, size_t bytecode_length) {
	size_t i = 0;
	bool keep_going = true;
	while(i < bytecode_length && keep_going) {
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
		case RFI: {
			number input = getc(stdout);
			set_value(arguments, input);
			*new_position = position + 5;
		} break;
		case WTO: {
			number source = get_value(arguments);
			putc(source, stdout);
			*new_position = position + 5;
		} break;
	}
}
