/*
 * KBscan.l    A flex file for lexical analysis of sentences in 1st-order logic
 *
 * Author: Carter J. Bastian
 *          March, 2016. 16W.
 *
 * This File Contains:
 *  - A set of rules for scanning First-Order Logic Statements
 */
%{

#include <string.h>
#include "toktype.h"
#include "lst.h"


#define MAXTOKENLENGTH 201
#define YYSTYPE lst_node
%}

white [ \t\n]+
letter [A-Za-z]
digit [0-9]
id ({letter}|{digit}|_)*

%option yylineno noyywrap

%%

{white}       /* Skip white space */

  /* Declaration Lexical Symbols */
"CS"          return CONST_DEC_T;
"FS"          return FUNC_DEC_T;
"RS"          return REL_DEC_T;
"VS"          return VAR_DEC_T;
"QS"          return QUERY_DEC_T;
"=>"          return IMPLIES_T;
"<=>"         return EQUIVALENT_T;
 
  /* Single Character Lexical Symbols */
"^"           return '^';
"v"           return 'v';
"~"           return '~';
"@"           return '@';
"#"           return '#';
";"           return ';';
":"           return ':';
"("           return '(';
")"           return ')';
","           return ',';
"="           return '=';

<<EOF>>       return EOF_T;

{id}          {
              /*lst_node n = create_lst_node(0);
              n->value_string = strdup(yytext);
              yylval = n; */
              return ID_T; }

