#include "instructions.h"
#include <stdio.h>
#include <stdlib.h>

char lowercase_char(char c);
void lowercase(char *string);
void output_command(FILE *in, FILE *out);
void output_condition(FILE *in, FILE *out);
void output_parameter(FILE *in, FILE *out);

int main(int argc, char *argv[]) {
	if(argc < 3) {
		puts("Please enter input and output filenames.");
		return -1;
	} else {
		FILE *input = fopen(argv[1], "r");
		FILE *output = fopen(argv[2], "w");
		char chunk[3];
		while(!feof(input)) {
			output_command(input, output);
			char next = getc(input);
			if(next != ' ' && next != '\t') {
				output_condition(input, output);
			} 
		}
		fclose(input);
		fclose(output);
		return 0;
	}
	error:
	fprintf(stderr, "Error occured during assembly.");
	return -1;
}


char lowercase_char(char c) {
	if(c >= 'A' && c <= 'Z') {
		return c + 'a' - 'A';
	} else {
		return c;
	}
}

void lowercase(char *string) {
	while(*string) {
		char c = *string;
		if(c >= 'A' && c <= 'Z') {
			*string = lowercase_char(c);
		}
		string++;
	}
}

void output_command(FILE *input, FILE *output) {
	char current = lowercase_char(getc(input));
	char next = lowercase_char(getc(input));
	switch(current) {
	case 'a':
		if(next == 'n') {
			putc(AND, output);
		} else if(next == 'd') {
			putc(ADD, output);
		} else {
			goto error;
		}
		getc(input); //eat last character
		break;
	case 'b':
		putc(BRN, output);
		getc(input); getc(input);
		break;
	case 'c':
		putc(CMP, output);
		getc(input); getc(input); //eat the other characters
		break;
	case 'd':
		putc(DIV, output);
		getc(input); getc(input); // eat other characters
		break;
	case 'e':
		putc(EXE, output);
		getc(input); getc(input); // eat other characters
		break;
	case 'i':
		putc(IOR, output);
		getc(input); getc(input); //eat
		break;
	case 'm':
		if(next == 'o') {
			next = getc(input);
			if(next == 'v') {
				putc(MOV, output);
			} else if(next == 'd') {
				putc(MOD, output);
			} else {
				goto error;
			}
		} else if(next == 'u') {
			putc(MUL, output);
			getc(input);
		} else {
			goto error;
		}
		break;
	case 'p':
		if(next == 'f') {
			putc(PFS, output);
			getc(input);
		} else if(next == 'o') {
			putc(POP, output);
			getc(input);
		} else {
			goto error;
		}
		break;
	case 'r':
		if(next == 'f') {
			putc(RFI, output);
			getc(input);
		} else if(next == 'h') {
			putc(RHD, output);
			getc(input);
		} else {
			goto error;
		}
		break;
	case 's':
		putc(SUB, output);
		getc(input); getc(input);
		break;
	case 'w':
		if(next == 't') {
			putc(WTO, output);
			getc(input);
		} else if(next == 'h') {
			putc(WHD, output);
			getc(input);
		}
		break;
	case 'x':
		putc(XOR, output);
		getc(input);
	}
error:
	fprintf(stderr, "Error occured during assembly.");
	exit(-1);
}

void output_condition(FILE *in, FILE *out) {
	switch(lowercase_char(getc(in))) {
	case 'u':
		putc(UN, out);
		break;
	case 'e':
		putc(EQ, out);
		break;
	case 'g':
		putc(GR, out);
		break;
	case 'l':
		putc(LS, out);
		break;
	case 'n':
		switch(lowercase_char(getc(in))) {
		case 'e':
			putc(NE, out);
			break;
		case 'g':
			putc(NG, out);
			break;
		case 'l':
			putc(NL, out);
			break;
		}
	}
}

void output_parameter(FILE *in, FILE *out) {
	char first = lowercase_char(getc(in));
	if(first == 'r') {
		char next = lowercase_char(getc(in));
		if(next == '$') {
			putc(REGISTER_VALUE, out);
		} else {
			putc(REGISTER, out);
			ungetc(next, in);
		}
	} else if(first == '=') {
		putc(CONSTANT, out);
	} else {
		ungetc(first, in);
	}
	char buffer[1024];
	char *current = buffer;
	char next = getc(in);
	while(!feof(in) && next != ' ' && next != '\t' && next != '\n') {
		*current = next;
		current++;
		next = getc(in);
	}
	*current = '\0';
	int value = (int)strtol(buffer, NULL, 16);
	if(value == 0) {
		for(int i = 0; i < 4; i++) putc(0, out);
	} else {
		if(value > 0) putc(0, out);
		else putc(1, out);
		int current = 32768;
		int byte = 0;
		byte += (current & value != 0) * 4;
		current /= 2;
		byte += (current & value != 0) * 2;
		current /= 2;
		byte += (current & value != 0);
		current /= 2;
		putc(byte, out);
		while(current >= 1) {
			byte += (current & value != 0) * 8;
			current /= 2;
			byte += (current & value != 0) * 4;
			current /= 2;
			byte += (current & value != 0) * 2;
			current /= 2;
			byte += (current & value != 0);
			current /= 2;
			putc(byte, out);
		}		
	}
}
