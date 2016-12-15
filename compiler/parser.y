%{
	#include "yacc.tab.h"
	#include <stdio.h>
%}

%union { char *sval; int ival; }

%token <sval> WORD
%token <ival> NUMBER
%token <sval> STRING;

%%


list:
	'(' ')'
	| '(' members ')'

members:
	expr
	| members expr

expr:
	atom
	| list

atom:
	WORD { printf("%s\n", $1); }
	| NUMBER { printf("%d\n", $1); }
	| STRING { printf("%d\n", $1); }

%%

extern FILE* yyin;

int main(int argc, char *argv[]) {
	if(argc < 2) {
		puts("Please enter an input file");
	} else {
		yyin = fopen(argv[1], "r");
		yyparse();
		fclose(yyin);
	}
	return 0;
}
