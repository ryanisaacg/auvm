%{
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "yacc.tab.h"

FILE* output;
extern FILE* yyin;

int getcommand(char *command) {
	if(strcmp(command, "DC") == 0) {
		return 0;
	} else if(strcmp(command, "LOAD") == 0) {
		return 1;
	} else if(strcmp(command, "STORE") == 0) {
		return 2;
	} else if(strcmp(command, "LABEL") == 0) {
		return 3;
	} else if(strcmp(command, "ADD") == 0) {
		return 4;
	} else if(strcmp(command, "SUB") == 0) {
		return 5;
	} else if(strcmp(command, "MULT") == 0) {
		return 6;
	} else if(strcmp(command, "DIV") == 0) {
		return 7;
	} else if(strcmp(command, "BL") == 0) {
		return 8;
	} else if(strcmp(command, "BE") == 0) {
		return 9;
	} else if(strcmp(command, "BG") == 0) {
		return 10;
	} else if(strcmp(command, "BU") == 0) {
		return 11;
	} else if(strcmp(command, "PRINT") == 0) {
		return 12;
	} else if(strcmp(command, "END") == 0) {
		return 13;
	}
}

%}

%union { char *string; int number; }

%token <sval> STRING
%token <number> NUMBER

%%
file:
	command
	| file command

command:
	STRING '\n' {
		char *command = $<string>1;
		int number = getcommand(command);
		fputc(number, output);
		fputc(0, output);
		fputc(0, output);
	} | STRING STRING '\n' {
		char *command = $<string>1;
		char *argument = $<string>2;
		fputc(getcommand(command), output);
		fputc(argument[0], output);
		fputc(0, output);
	} | STRING '=' NUMBER '\n' {
		char *command = $<string>1;
		int argument = $<number>3;
		fputc(getcommand(command), output);
		fputc('=', output);
		fputc(argument, output);
	} | STRING STRING NUMBER '\n' {
		char *command = $<string>1;
		char *argument = $<string>2;
		int value = $<number>3;
		fputc(getcommand(command), output);
		fputc(argument[0], output);
		fputc(value, output);
	}
%%

int main(int argc, char *argv[]) {
	if(argc < 3) {
		puts("Please enter input and output files.");
	} else {
		yyin = fopen(argv[1], "r");
		output = fopen(argv[2], "w");
		yyparse();
		fclose(output);
	}
	return 0;
}
