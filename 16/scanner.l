%{
	#include "parser.tab.h"
	extern void store_token_name(std::string token_name, char *lexeme,int lineno);
%}

int yylineno=1;

digit 		[0-9]
float 		{digit}*\.{digit}+|{digit}+\.{digit}* 
integer 	{digit}+
letter 		[A-Za-z_]
id 			{letter}({letter}|{digit})*
op 			[+\-/\*]
relop 		<|<=|>|>=|==|!=
logop 		\|\||&&|!
metachar 	[\(\);,{}\?:]
assign		=
comment 	[\/][\/].*

error		.

%%
[ \t]+		;

{comment} 	;


void 		{
			store_token_name("VOID",yytext,yylineno); 
			return VOID;
			}

int 		{		
			store_token_name("INTEGER",yytext,yylineno); 
			return INTEGER;
			}

float 		{
			store_token_name("FLOAT",yytext,yylineno); 
			return FLOAT;
			}

return 		{		
			store_token_name("NAME",yytext,yylineno); 
			return RETURN;
			}

if 			{		
			store_token_name("IF",yytext,yylineno); 
			return IF;
			}

do 			{		
			store_token_name("DO",yytext,yylineno); 
			return DO;
			}

while 		{		
			store_token_name("WHILE",yytext,yylineno); 
			return WHILE;
			}

else 		{		
			store_token_name("ELSE",yytext,yylineno); 
			return ELSE;
			}

print 		{
			store_token_name("PRINT",yytext,yylineno); 
			return PRINT;
			}


{id} 		{
			store_token_name("NAME",yytext,yylineno); 
			std::string * s = new std::string(yytext); 
			yylval.string_value = &s->append("_") ; 
			return NAME;
			}

{relop} 	{
				// Done this ; 
				if 		(strcmp(yytext,"<")==0)  { 	store_token_name("LESS_THAN",yytext,yylineno);			return  L_OP ; }
				else if (strcmp(yytext,"<=")==0) { 	store_token_name("LESS_THAN_EQUAL",yytext,yylineno); 	return  LE_OP ; }
				else if (strcmp(yytext,">")==0 ) {	store_token_name("GREATER_THAN",yytext,yylineno); 		return  G_OP ; }
				else if (strcmp(yytext,">=")==0) {	store_token_name("GREATER_THAN_EQUAL",yytext,yylineno); return  GE_OP ; }
				else if (strcmp(yytext,"==")==0) {	store_token_name("EQUAL",yytext,yylineno); 				return  E_OP ; }
				else if (strcmp(yytext,"!=")==0) {	store_token_name("NOT_EQUAL",yytext,yylineno); 			return  NE_OP ; }
			}

{logop}		{	
				if 		(strcmp(yytext,"&&")==0) { store_token_name("AND",yytext,yylineno); return AND_OP;}
				else if (strcmp(yytext,"||")==0) { store_token_name("OR",yytext,yylineno) ; return OR_OP ;}
				else if (strcmp(yytext,"!")==0 ) { store_token_name("NOT",yytext,yylineno); return NOT_OP;}
			}

{op}		{		
			store_token_name("ARITHOP",yytext,yylineno); 
			return yytext[0];
			}


{assign} 	{	
			store_token_name("ASSIGN_OP",yytext,yylineno); 
			return ASSIGN;
			}

{integer} 	{
			store_token_name("NUM",yytext,yylineno); 
			yylval.integer_value = atoi(yytext); 
			return INTEGER_NUMBER;
			}

{float} 	{
			store_token_name("FNUM",yytext,yylineno); 
			yylval.double_value = atof(yytext); 
			return DOUBLE_NUMBER;
			}

{metachar} 	{
			store_token_name("META CHAR",yytext,yylineno); 
			return yytext[0];
			}

\n			{
			}
{error}		{
			printf("cs316: Error : Line: %d: scanner internal error--no action foundline\n",yylineno);
			}


%%
