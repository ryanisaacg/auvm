#pragma once

#include <stdint.h>
#include <stddef.h>

typedef unsigned char ubyte;
typedef char byte;
typedef int32_t number;

enum Instruction {
	MOV, ADD, SUB, MUL, DIV, MOD, RFI, WTO, CMP, BRN, GTB, GCB, PFS, POP, AND, IOR, XOR, RHD, WHD, END, LBL
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

number get_number(ubyte *bytes) {
	number value = 0;
	value += bytes[3];
	value += bytes[2] * 10;
	value += bytes[1] * 100;
	value += (bytes[0] % 8) * 1000;
	if(bytes[0] != 0 && bytes[0] % 8 == 0) {
		value *= -1;
	}
	return value;
}
