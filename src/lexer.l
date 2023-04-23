%option noyywrap
%option yylineno

%{
#include <stdio.h>
#include <string.h>
#include <ctype.h>

#include "ast.h"
#include "parser.h"

int line_cnt = 1;
%}

MultilineComment	    "/*"([^\*]|(\*)*[^\*/])*(\*)*"*/"
SingleLineComment	    "//"[^\n]+
Lex_err [1-9][0-9]*[a-zA-Z]+[0-9]*|0[0-7]*[8-9a-zA-Z_]+[0-9a-zA-Z_]* 

%%
"//".* { }
"/*"([^\*]|(\*)*[^\*/])*(\*)*"*/" {
    int len = strlen(yytext);
    for (int i = 0; i < len; i++)
        if(yytext[i] == '\n')
            line_cnt++;
}
"\n" { line_cnt++; }
[ \t] { }
"int" { return INT; }
"float" { return FLOAT; }
"void" { return VOID; }
"const" { return CONST; }
"return" { return RETURN; }
"if" { return IF; }
"else" { return ELSE; }
"while" { return WHILE; }
"break" { return BREAK; }
"continue" { return CONTINUE; }
"(" { return LP; }
")" { return RP; }
"[" { return LB; }
"]" { return RB; }
"{" { return LC; }
"}" { return RC; }
"," { return COMMA; }
";" { return SEMICOLON; }
"+" { return PLUS; }
"-" { return MINUS; }
"!" { return NOT; }
"=" { return ASSIGN; }
"*" { return MUL; }
"/" { return DIV; }
"%" { return MOD; }
"&&" { return AND; }
"||" { return OR; }
"==" { return EQ; }
"!=" { return NE; }
"<" { return LT; }
"<=" { return LE; }
">" { return GT; }
">=" { return GE; }
0[xX][0-9a-fA-F]* {
    int val = 0;
    int len = strlen(yytext);
    for (int i = 2; i < len; i++) {
        val <<= 4;
        if (isdigit(yytext[i]))
            val += yytext[i] - '0';
        else
            val += yytext[i] - 'a' + 10;
    }
    yylval.int_val = val;
    return INT_LIT;
}

[a-zA-Z_][a-zA-Z0-9_]* {
    yylval.str_val = (char *)malloc(strlen(yytext) + 1);
    strcpy(yylval.str_val, yytext);
    yylval.str_val[strlen(yytext)] = '\0';
    return ID;
}
[0-9]*\.[0-9]+f?|[0-9]+e-?[0-9]+f? { yylval.float_val = atof(yytext); return FLOAT_LIT; }
{Lex_err} { return LEX_ERR; }
[1-9][0-9]*|0 { yylval.int_val = atoi(yytext); return INT_LIT; }
0[0-7]+ {
    int val = 0;
    int len = strlen(yytext);
    for (int i = 1; i < len; i++)
        val = (val << 3) + yytext[i] - '0';
    yylval.int_val = val;
    return INT_LIT;
}
. { }
%%
