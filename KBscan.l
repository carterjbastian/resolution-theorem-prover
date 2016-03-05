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

#define MAXTOKENLENGTH 201

%}

white [ \t\n]+
letter [A-Za-z]
digit [0-9]
id ({letter}|_)({letter}|{digit}|_)*

%option yylineno noyywrap

%%

{white}       /* Skip white space */

{id}          {

