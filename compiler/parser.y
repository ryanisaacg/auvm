%{
	#include "ast.h"
	#include "yacc.tab.h"
	
	#include <stdio.h>
	Node *root_node;
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
	| root list { node_add_child(root_node, $2); }

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
	if(argc < 3) {
		puts("Please enter an input file and an output file.");
	} else {
		yyin = fopen(argv[1], "r");
		root_node = node_new_nil();
		yyparse();
		FILE* out = fopen(argv[2], "w");
		for(Node *child = root_node->child; child != NULL; child = child->next) {
			node_output(child, out);
			fputc('\n', out);
		}
		fclose(yyin);
		fclose(out);
	}
	return 0;
}
