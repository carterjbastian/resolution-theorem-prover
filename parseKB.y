/*
 * parseKB.y    A bison file to parse a knowledge base into an lst
 *
 * Author:  Carter J. Bastian
 *          March, 2016. 16W.
 */

%{
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "lst.h"


extern int yylex();
int yyerror(char *s);
extern char *yytext;

extern ast_node root;
extern int parseError;
extern int yylineno;

%}

%code requires {
  #include "../util/ast.h"
  #include "../util/symtab.h"
  #define YYSTYPE ast_node
}

/* Declarations of tokens from lexer */
%token ID_T FUNC_DEC_T REL_DEC_T CONST_DEC_T QUERY_DEC_T VAR_DEC_T
%token IMPLIES_T EQUIVALENT_T EOF_T

/* Operator associativity and precedence */
%nonassoc LOWER_THAN_COMP
%right '(' ')'
%right '[' ']'

%left EQUIVALENT_T
%left IMPLIES_T
%left 'v'
%left '^'
%nonassoc '='
%left '~'



%%

program: declarationList lineList query {}
;


declarationList: declarationList declaration {}
| declaration {}
;


declaration: CONST_DEC_T ':' ID_T ';' {}
| REL_DEC_T ':' ID_T ';' {}
| FUNC_DEC_T ':' ID_T ';' {}
| VAR_DEC_T ':' ID_T ';' {}
;


lineList: lineList line {}
| line {}
;


line: sentence ';' {}
;


sentence: AtomicSentence %prec LOWER_THAN_COMP {}
| complexSentence %prec LOWER_THAN_COMP {}
;


AtomicSentence: ID_T {}
| ID_T '(' termList ')' {}
| term '=' term {}
;

termList: termList ',' term {}
| term {}
;


term: ID_T '(' termList ')' {}
| ID_T {}
;


complexSentence: '(' sentence ')' {}
| '[' sentence ']' {}
| '~' sentence {}
| sentence '^' sentence {}
| sentence 'v' sentence {}
| sentence IMPLIES_T sentence {}
| sentence EQUIVALENT_T sentence {}
| '@' ID_T sentence %prec LOWER_THAN_COMP {}
| '#' ID_T sentence %prec LOWER_THAN_COMP {}
;

query: QUERY_DEC_T ':' sentence ';' {}
;

%%

int yyerror(char *s) {
  return 0;
}
