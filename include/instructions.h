#pragma once

enum Instruction {
	MOV, ADD, SUB, MUL, DIV, MOD, RFI, WTO, CMP, BRN, PFS, POP, AND, IOR, XOR, RHD, WHD, EXE
};

typedef enum Instruction Instruction;

enum Condition {
	UN, EQ, GR, LS, NE, NG, NL
};

typedef enum Condition Condition;
