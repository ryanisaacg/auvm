%{
	#include "ast.h"
	#include "yacc.tab.h"
	#include <stdio.h>
	#include <string.h>
%}

%%

\".+\"						{ yylval.sval = strdup(yytext); return STRING; }
[A-za-z][A-Za-z\-0-9_]* 	{ yylval.sval = strdup(yytext); return WORD; }
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
