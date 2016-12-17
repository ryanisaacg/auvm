#include "instructions.h"
#include <stdio.h>
#include <stdlib.h>

char *command_to_string(ubyte command);
char *condition_to_string(ubyte condition);

int main(int argc, char *argv[]) {
	if(argc < 3) {
		puts("Please enter an input and output filename.");
	} else {
		FILE *input = fopen(argv[1], "r");
		FILE *output = fopen(argv[2], "w");
		while(!feof(input)) {
			ubyte command = getc(input);
			fputs(command_to_string(command), output);
			ubyte condition = getc(input);
			fputs(condition_to_string(condition), output);
			putc(' ', output);
			size_t others = command_length(command);
			others /= 5;
			for(size_t i = 0; i < others; i++) {
				ubyte type = getc(input);
				switch(type) {
				case REGISTER:
					putc('R', output);
					break;
				case REGISTER_VALUE:
					fputs("R$", output);
					break;
				case CONSTANT:
					putc('=', output);
					break;
				case POINTER:
					break;
				}
				ubyte value[4];
				fread(value, 1, 4, input);
				number val = get_number(value);
				fprintf(output, "%d", val);
				putc(' ', output);
			}
			fputc('\n', output);
		}
	}
	return 0;
}

char *condition_to_string(ubyte condition) {
	switch(condition) {
	case UN:
		return "UN";
		break;
	case EQ:
		return "EQ";
		break;
	case GR:
		return "GR";
		break;
	case LS:
		return "LS";
		break;
	case NE:
		return "NE";
		break;
	case NG:
		return "NG";
		break;
	case NL:
		return "NL";
		break;
	}
	return "ERROR";
}

char *command_to_string(ubyte command) {
	switch(command) {
	case MOV:
		return "MOV";
		break;
	case ADD:
		return "ADD";
		break;
	case SUB:
		return "SUB";
		break;
	case MUL:
		return "MUL";
		break;
	case DIV:
		return "DIV";
		break;
	case MOD:
		return "MOD";
		break;
	case RFI:
		return "RFI";
		break;
	case WTO:
		return "WTO";
		break;
	case CMP:
		return "CMP";
		break;
	case BRN:
		return "BRN";
		break;
	case PFS:
		return "PFS";
		break;
	case POP:
		return "POP";
		break;
	case AND:
		return "AND";
		break;
	case IOR:
		return "IOR";
		break;
	case XOR:
		return "XOR";
		break;
	case RHD:
		return "RHD";
		break;
	case WHD:
		return "WHD";
		break;
	case END:
		return "END";
		break;
	}
	return "ERROR";
}
