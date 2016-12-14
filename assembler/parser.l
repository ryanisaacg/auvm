/*-*-C-*-*/

%{ 
#include "yacc.tab.h"
#include <stdio.h>
#include <string.h>
%}

%option yylineno

%%

[A-Za-z]+ { yylval.string = strdup(yytext); return STRING; }
[0-9]+ { yylval.number = atoi(yytext); return NUMBER; }
[ \t\r]+ {}
[\n]|. { return yytext[0]; }

%%
void yyerror(char *message) {
	fprintf(stderr, "%s at line %d\n", message, yylineno);
}
int yywrap(void) {
	return 1;
}
