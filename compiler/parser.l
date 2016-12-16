%{
	#include "ast.h"
	#include "yacc.tab.h"
	#include <stdio.h>
	#include <stdlib.h>
	#include <string.h>

char *dupstring(char *text) {
	int len = strlen(text);
	char *new = malloc(len + 1);
	memcpy(new, text, len + 1);
	return new;
}
%}

%%

\".+\"						{ yylval.sval = dupstring(yytext); return STRING; }
[A-za-z][A-Za-z\-0-9_]* 	{ yylval.sval = dupstring(yytext); return WORD; }
[0-9]+ 						{ yylval.ival = atoi(yytext); return NUMBER; }
\(|\)	 					{ return yytext[0]; }
[ \t\n]+					{ }

%%

void yyerror(char *message) {
	fprintf(stderr, "%s\n", message);
}
int yywrap(void) {
	return 1;
}
