
/*
 * parseKB_main.c    A driver to print an lst generated by Logical Parsing
 *
 * Author:    Carter J. Bastian
 *            March, 2016. 16W
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "lst.h"
#include "transformations.h"

lst_node root = NULL;
lst_node query = NULL;
symlist_node *symbol_list = NULL;

extern int yyparse();
extern int yydebug;
extern FILE *yyin;

// Used in the parser (parseKB.y) for error recovery and reporting.
int parseError = 0;
FILE *error_out;  // Global conditionally used in testing mode only

int main() {
  int noRoot = 0;		/* 0 means we will have a root */

  /*   yydebug = 1; */ // Good for debugging, unnecessary for now

  /* Build the tree */
  error_out = stderr;
  noRoot = yyparse();

  if (parseError && (!noRoot))
    fprintf(stderr, "WARNING: There were %d parse errors.\nParse tree may be ill-formed.\n", parseError);

  if (noRoot != 0)
  //  print_lst(stdout, root, 0);
  // else
    fprintf(stderr, "Parsing reached fatal error. No AST was constructed\n");


//  fprintf(stdout, "SYMBOL LIST:\n");
//  print_symbol_list(stdout, symbol_list);

//  fprintf(stdout, "\n\nKNOWLEDGE BASE:\n");
//  print_lst(stdout, root, 0);

  fprintf(stdout, "\n\nQUERY (before transformation):\n");
  print_lst(stdout, query, 0);

  // Apply CNF transformations
  adoption_agency(query);
  remove_equivalence(query);
  remove_implies(query);

  move_negation_inward(query);

  fprintf(stdout, "\n\nQUERY (after transformation):\n");
  print_lst(stdout, query, 0);

  return 0;
}
