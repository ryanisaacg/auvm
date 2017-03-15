#pragma once

#include <stdint.h>
#include <stddef.h>

typedef unsigned char ubyte;
typedef char byte;
typedef int32_t number;

enum Instruction {
	MOV, ADD, SUB, MUL, DIV, MOD, RFI, WTO, CMP, BRN, GTB, GCB, PFS, POP, AND, IOR, XOR, RHD, WHD, END, LBL, DBG
};

typedef enum Instruction Instruction;

enum Condition {
	UN, EQ, GR, LS, NE, NG, NL
};

typedef enum Condition Condition;

enum Type {
	REGISTER, REGISTER_VALUE, POINTER, CONSTANT
};

typedef enum Type Type;

size_t command_length(ubyte command) {
	switch(command) {
		case CMP:
		case MOV:
			return 10;
		case ADD:
		case SUB:
		case MUL:
		case DIV:
		case MOD:
		case AND:
		case IOR:
		case XOR:
			return 15;
		case BRN:
		case RFI:
		case WTO:
		case RHD:
		case WHD:
		case LBL:
		case GTB:
		case GCB:
			return 5;
	}
	return 0;
}

void set_number(ubyte *bytes, number n) {
	bytes[0] = (n >> 24) & 0xff;
	bytes[1] = (n >> 16) & 0xff;
	bytes[2] = (n >> 8) & 0xff;
	bytes[3] = n & 0xff;
}

number get_number(ubyte *bytes) {
	number value = 0;
	value += bytes[3];
	value += bytes[2] << 8;
	value += bytes[1] << 16;
	value += bytes[0] << 24;
	return value;
}
