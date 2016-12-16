%{
	#include "ast.h"
	#include "yacc.tab.h"
	
	#include <stdio.h>
%}

%union { char *sval; int ival; Node *nval; }

%token <sval> WORD
%token <ival> NUMBER
%token <sval> STRING

%type <nval> atom
%type <nval> expr
%type <nval> members
%type <nval> list

%%

root:
	| root list { node_print($2); }

list:
	'(' ')' { $$ = node_new_nil(); }
	| '(' members ')' { $$ = $2; }

members:
	expr
	| members expr { node_add_child($1, $2); $$ = $1; }

expr:
	atom
	| list

atom:
	WORD { $$ = node_new_str($1, WORD_NODE); }
	| NUMBER { $$ = node_new_int($1, NUMBER_NODE); }
	| STRING { $$ = node_new_str($1, STRING_NODE); }

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
