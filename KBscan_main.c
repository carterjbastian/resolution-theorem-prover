
/*
 * File: KBscan_main.c
 *  A file containing the main function for scanning 1st-order logic via flex
 *  This is just meant for testing lexical analysis.
 *
 * Author:  Carter J. Bastian
 *          March, 2016. 16W. 
 *        
 */

/* System Includes */
#include <stdio.h>
#include <string.h>

/* Local Includes */
#include "toktype.h"
#include "lst.h"

/* Public Variable Declarations */
extern FILE *yyin;
extern char *yytext;
extern int yyleng;
extern int yylex (void);
extern int yylineno;
extern int yyval;

// Utility function courtesy of Thomas H. Cormen
char *token_name(int token) {
  char tiny_buf[2];

  if (token > 256) // non-single-char
    return (tokens[token - FIRST_BIG_TOKEN]);

  tiny_buf[1] = 0x00;
  tiny_buf[0] = token;
  return strdup(tiny_buf);
}

/* The main function to do the scanning via flex */
int main() {
  int token;
  while ((token = yylex()) != EOF_T)
    printf("[%d]: %s \"%s\"\n", yylineno, token_name(token), yytext);
  
  return 0;
}
