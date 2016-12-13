#pragma once

enum Instruction {
	MOV, ADD, SUB, MUL, DIV, MOD, NEG, RFI, WTO, BIE, BIL, BIG, BNE, BNL, BNG, PFS, POP, AND, IOR, XOR
};

typedef enum Instruction Instruction;
