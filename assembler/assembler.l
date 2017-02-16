%{
	#include "asmyacc.tab.h"
	#include <stdio.h>
	#include <stdlib.h>
	#include <string.h>

char *dupstr(char *text) {
	int len = strlen(text);
	char *new = malloc(len + 1);
	memcpy(new, text, len + 1);
	return new;
}
char *upcase(char *text) {
	char *start = text;
	while(*text != '\0') {
		if(*text >= 'a' && *text <= 'z') *text += 'A' - 'a';
		text++;
	}
	return start;
}


%}

%option yylineno

%%

;.*\n 						{ } /* handle comments */
R$[0-9]+					{ yylval.ival = atoi(yytext + 2); 		return T_REGISTER_VALUE;}
R[0-9]+						{ yylval.ival = atoi(yytext + 1);		return T_REGISTER; 		}
=[0-9]+						{ yylval.ival = atoi(yytext + 1);  		return T_CONSTANT; 		}
%[0-9]+						{ yylval.ival = atoi(yytext + 1);       return T_POINTER; 		}
[A-Za-z]+					{ yylval.sval = upcase(dupstr(yytext)); 	return T_WORD; 			}
\n							{ 											return NEWLINE; 		}
[ \t\r]+					{ } //whitespace
.							{ }
%%

void yyerror(char *message) {
	fprintf(stderr, "%s at line %d\n", message, yylineno);
}
int yywrap(void) {
	return 1;
}
