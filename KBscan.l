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

extern YYSTYPE yylval;
%}

white [ \t\n]+
letter [A-Za-z]
digit [0-9]
id ({letter}|{digit}|_)*

%x block_comment

%option yylineno noyywrap

%%

{white}       /* Skip white space */


  /* Start Condition for block comments */
"/*"         { BEGIN(block_comment); }
<block_comment>[^*]*   /* Skip everything that's not a * */
<block_comment>"*"+[^*/]*      /* Skip *s not followed by a / */
<block_comment>"*/"  { BEGIN(INITIAL); } /* End the comment */

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

{id}          {
              lst_node n = create_lst_node(NULL_N);
              n->value_string = strdup(yytext);
              yylval = n;
              return ID_T; }

