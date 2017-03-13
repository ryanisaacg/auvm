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
;.*\n						{ } //comments					{ } //block comments
\".+\"						{ yylval.sval = dupstring(yytext); return STRING; } //string literal
[A-za-z][A-Za-z\-0-9_]* 	{ yylval.sval = dupstring(yytext); return WORD; } //word token
[0-9]+ 						{ yylval.ival = atoi(yytext); return NUMBER; } //numbers
\(|\)	 					{ return yytext[0]; } //parens
[ \t\n]+					{ } //whitespace

%%

void yyerror(char *message) {
	fprintf(stderr, "%s\n", message);
}
int yywrap(void) {
	return 1;
}
