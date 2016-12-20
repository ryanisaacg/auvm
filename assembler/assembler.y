%{
	#include "instructions.h"
	#include "yacc.tab.h"
	FILE *out;

void write(FILE *out, int x) {
	putc((x >> 24) & 0xff, out);
	putc((x >> 16) & 0xff, out);
	putc((x >> 8) & 0xff, out);
	putc((x) & 0xff, out); 
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

command:
	 T_WORD parameter_list NEWLINE


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


