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
#define YYSTYPE lst_node
#define YYDEBUG 1

extern YYSTYPE yylval;
extern int yylex();
int yyerror(char *s);
extern char *yytext;

extern lst_node root;
extern lst_node query;
extern symlist_node *symbol_list;

extern int parseError;
extern int yylineno;

%}

%code requires {
  #include "lst.h"
  #define YYSTYPE lst_node
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

program : declarationList lineList query {
        lst_node t = create_lst_node(KB_ROOT_N);
        t->left_child = $2;
        query = $3;
        root = $$ = t; }
;


declarationList: declarationList declaration { $$ = $1; }
| declaration { $$ = $1; }
;


declaration: CONST_DEC_T ':' ID_T ';' {
        if (symbol_list == NULL) {
          symlist_node *l = create_listnode($3->value_string);
          l->dec_type = CONSTANT_N;
          symbol_list = l;
        } else {
          symlist_node *l = append_symlist(symbol_list, $3->value_string);
          l->dec_type = CONSTANT_N;
          symbol_list = l;
        } }
| REL_DEC_T ':' ID_T ';' {
        if (symbol_list == NULL) {
          symlist_node *l = create_listnode($3->value_string);
          l->dec_type = RELATION_N;
          symbol_list = l;
        } else {
          symlist_node *l = append_symlist(symbol_list, $3->value_string);
          l->dec_type = RELATION_N;
          symbol_list = l;
        } }
| FUNC_DEC_T ':' ID_T ';' {
        if (symbol_list == NULL) {
          symlist_node *l = create_listnode($3->value_string);
          l->dec_type = FUNCTION_N;
          symbol_list = l;
        } else {
          symlist_node *l = append_symlist(symbol_list, $3->value_string);
          l->dec_type = FUNCTION_N;
          symbol_list = l;
        } }
| VAR_DEC_T ':' ID_T ';' {
        if (symbol_list == NULL) {
          symlist_node *l = create_listnode($3->value_string);
          l->dec_type = VARIABLE_N;
          symbol_list = l;
        } else {
          symlist_node *l = append_symlist(symbol_list, $3->value_string);
          l->dec_type = VARIABLE_N;
          symbol_list = l;
        } }
;


lineList: lineList line {
        lst_node t = $1;
        if (t != NULL) {
          while(t->right_sib != NULL)
            t = t->right_sib;
          t->right_sib = $2; 
          $$ = $1; 
        } else {
          $$ = $2;
        } }
| line { $$ = $1; }
;


line: sentence ';' { $$ = $1; }
;


sentence: AtomicSentence %prec LOWER_THAN_COMP { $$ = $1; }
| complexSentence %prec LOWER_THAN_COMP { $$ = $1; }
;


AtomicSentence: ID_T {
        lst_node t = create_lst_node(lookup_symbol_type(symbol_list, $1->value_string));
        if (t->node_type == NULL_N) {
          fprintf(stderr, "PARSE ERROR: Use of undeclared variable on line %d\n", yylineno);
          parseError++;
        }
        t->value_string = $1->value_string;
        $$ = t; }
| ID_T '(' termList ')' {
        lst_node t = create_lst_node(lookup_symbol_type(symbol_list, $1->value_string));
        if (t->node_type == NULL_N) {
          fprintf(stderr, "PARSE ERROR: Use of undeclared symbol on line %d\n", yylineno);
          parseError++;
        }
        t->value_string = $1->value_string;
        t->left_child = $3;
        $$ = t; }
| term '=' term {
        lst_node t = create_lst_node(EQUALS_N);
        t->left_child = $1;
        t->left_child->right_sib = $3;
        $$ = t; }
;

termList: termList ',' term {
        lst_node t = $1;
        if (t != NULL) {
          while(t->right_sib != NULL)
            t = t->right_sib;
          t->right_sib = $3; 
          $$ = $1; 
        } else {
          $$ = $3;
        } }
| term { $$ = $1; }
;


term: ID_T '(' termList ')' {
        lst_node t = create_lst_node(lookup_symbol_type(symbol_list, $1->value_string));
        if (t->node_type == NULL_N) {
          fprintf(stderr, "PARSE ERROR: Use of undeclared symbol on line %d\n", yylineno);
          parseError++;
        }
        t->value_string = $1->value_string;
        t->left_child = $3;
        $$ = t; }
| ID_T {
        lst_node t = create_lst_node(lookup_symbol_type(symbol_list, $1->value_string));
        if (t->node_type == NULL_N) {
          fprintf(stderr, "PARSE ERROR: Use of undeclared symbol on line %d\n", yylineno);
          parseError++;
        }
        t->value_string = $1->value_string;
        $$ = t; }
;


complexSentence: '(' sentence ')' { $$ = $2; }
| '[' sentence ']' {$$ = $2; }
| '~' sentence {
        lst_node t = create_lst_node(NEGATION_N);
        t->left_child = $2;
        $$ = t; }
| sentence '^' sentence {
        lst_node t = create_lst_node(CONJUNCTION_N);
        t->left_child = $1;
        t->left_child->right_sib = $3;
        $$ = t; }
| sentence 'v' sentence {
        lst_node t = create_lst_node(DISJUNCTION_N);
        t->left_child = $1;
        t->left_child->right_sib = $3;
        $$ = t; }
| sentence IMPLIES_T sentence {
        lst_node t = create_lst_node(IMPLICATION_N);
        t->left_child = $1;
        t->left_child->right_sib = $3;
        $$ = t; }
| sentence EQUIVALENT_T sentence {
        lst_node t = create_lst_node(EQUIVALENCE_N);
        t->left_child = $1;
        t->left_child->right_sib = $3; 
        $$ = t; }
| '@' ID_T sentence %prec LOWER_THAN_COMP {
        lst_node t = create_lst_node(Q_FORALL_N);
        t->value_string = $2->value_string;
        t->left_child = $3;
        $$ = t; }
| '#' ID_T sentence %prec LOWER_THAN_COMP {
        lst_node t = create_lst_node(Q_EXISTS_N);
        t->value_string = $2->value_string;
        t->left_child = $3;
        $$ = t; }
;

query: QUERY_DEC_T ':' sentence ';' { $$ = query = $3; }
;

%%

int yyerror(char *s) {
  return 0;
}
