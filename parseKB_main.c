
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

  /* Add negated query to the knowledge base */
  lst_node curr;
  for (curr = root->left_child; curr->right_sib != NULL; curr = curr->right_sib)
    ;
  lst_node negation = create_lst_node(NEGATION_N);
  negation->parent = root;
  negation->left_child = query;
  query->parent = negation;
  curr->right_sib = negation;

  fprintf(stdout, "SYMBOL LIST (before):\n");
  print_symbol_list(stdout, symbol_list);

  fprintf(stdout, "\n\nKNOWLEDGE BASE: (before transformation\n");
  print_lst(stdout, root, 0);

  // Apply CNF transformations
  adoption_agency(root);
  remove_equivalence(root);
  remove_implies(root);
  move_negation_inward(root);

  standardize_variables(root, &symbol_list);
  skolemize(root, &symbol_list);

  drop_universal_quantifier(root);
  distribute_disjunction(root);

  // Clean up the NULL nodes
  adoption_agency(root);

  for (curr = root->left_child; curr != NULL; curr = curr->right_sib) {
    fprintf(stdout, "Looking at branch\n");
    curr = clean_tree(&curr);
  }

  fprintf(stdout, "\n\nKNOWLEDGE BASE (after transformation):\n");
  print_lst(stdout, root, 0);

  fprintf(stdout, "\n\nSYMBOL LIST (after):\n");
  print_symbol_list(stdout, symbol_list);
  return 0;
}
