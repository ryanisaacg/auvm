%{
	#include "instructions.h"
	#include "asmyacc.tab.h"
	#include <stdio.h>
	#include <string.h>
	FILE *out;
	extern FILE *yyin;

void write(FILE *out, int x) {
	ubyte value[4];
	set_number(value, x);
	fwrite(value, 1, 4, out);
}

int get_value(char *input) {
	if(strcmp(input, "MOV") == 0) {
		return MOV;
	} else if(strcmp(input, "ADD") == 0) {
		return ADD;
	} else if(strcmp(input, "SUB") == 0) {
		return SUB;
	} else if(strcmp(input, "MUL") == 0) {
		return MUL;
	} else if(strcmp(input, "DIV") == 0) {
		return DIV;
	} else if(strcmp(input, "MOD") == 0) {
		return MOD;
	} else if(strcmp(input, "RFI") == 0) {
		return RFI;
	} else if(strcmp(input, "WTO") == 0) {
		return WTO;
	} else if(strcmp(input, "CMP") == 0) {
		return CMP;
	} else if(strcmp(input, "BRN") == 0) {
		return BRN;
	} else if(strcmp(input, "GTB") == 0) {
		return GTB;
	} else if(strcmp(input, "GCB") == 0) {
		return GCB;
	} else if(strcmp(input, "PFS") == 0) {
		return PFS;
	} else if(strcmp(input, "POP") == 0) {
		return POP;
	} else if(strcmp(input, "AND") == 0) {
		return AND;
	} else if(strcmp(input, "IOR") == 0) {
		return IOR;
	} else if(strcmp(input, "XOR") == 0) {
		return XOR;
	} else if(strcmp(input, "RHD") == 0) {
		return RHD;
	} else if(strcmp(input, "WHD") == 0) {
		return WHD;
	} else if(strcmp(input, "END") == 0) {
		return END;
	} else if(strcmp(input, "LBL") == 0) {
		return LBL;
	} else if(strcmp(input, "DBG") == 0) {
		return DBG;
	}
}

int get_condition(char *string) {
	if(strcmp(string, "UN") == 0) return UN;
	else if(strcmp(string, "EQ") == 0) return EQ;
	else if(strcmp(string, "GR") == 0) return GR;
	else if(strcmp(string, "LS") == 0) return LS;
	else if(strcmp(string, "NE") == 0) return NE;
	else if(strcmp(string, "NG") == 0) return NG;
	else if(strcmp(string, "NL") == 0) return NL;
}
%}

%union { int ival; char *sval; }

%token <sval> T_WORD
%token <ival> T_REGISTER
%token <ival> T_REGISTER_VALUE
%token <ival> T_CONSTANT
%token <ival> T_POINTER
%token NEWLINE

%%

root:

	| root command
	| root NEWLINE

command:
	 command_word parameter_list NEWLINE

command_word:
	T_WORD { 
		int len = strlen($1); 
		if(len == 3) { 
			putc(get_value($1), out); 
			putc(UN, out);
		} else if(len == 5) {
			int cond = get_condition($1 + 3);
			*($1 + 3) = '\0';
			int value = get_value($1);
			putc(value, out);
			putc(cond, out);
		}
	}
parameter_list:
	
	| parameter_list parameter

parameter: 
	T_REGISTER { 
		putc(REGISTER, out);
		write(out, $1); }
	| T_REGISTER_VALUE {
		putc(REGISTER_VALUE, out);
		write(out, $1); }
	| T_POINTER {
		putc(POINTER, out);
		write(out, $1); }
	| T_CONSTANT {
		putc(CONSTANT, out);
		write(out, $1); }
%%

int main(int argc, char *argv[]) {	
	if(argc < 3) {
		puts("Please enter an input file and an output file.");
	} else {
		yyin = fopen(argv[1], "r");
		out = fopen(argv[2], "w");
		yyparse();
		fclose(yyin);
		fclose(out);
	}
	return 0;
}
