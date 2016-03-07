/*
 * resolution.c   functions implementing resolution-based theorem proving
 * 
 * Author:    Carter J. Bastian
 *            March 2016. 16W
 */

// System includes
#include <stdlib.h>
#include <stdio.h>

// Local includes
#include "set.h"
#include "lst.h"
#include "resolution.h"

// Helper function declarations
cSub *unify_var(lst_node c1, lst_node c2, cSub *sub);
int occur_check(lst_node var, lst_node x);

int resolution(FILE *fp, lst_node kb) {
  cSet *clauses = generate_set();
  cSet *new = generate_set();  
  int i = 0;


  // Create the clauses set from the knowledge base

  // Loop forever
  while (++i > 0) {
    fprintf(fp, "Inside while loop iteration %d\n", i);
    
    // Loop through each clause in the clauses set
      // Loop through each subsequent clause in the clauses set
        // create new resolvents set
        // resolve the pair
        // if resolvents contains the empty clause, return true
        // join new and resolvents sets
    // if the new set is a subset of clauses, return false
    // otherwise, join clauses with the new set and reiterate

  }

  return 1; // Failure might never be reached
}

cSet *resolve(lst_node c1, lst_node c2) {
  cSet *resolvents = generate_set();
  
  cSet *disjuncts1 = generate_set();
  cSet *desjuncts2 = generate_set();

  // Make the set of all disjunctions in clause 1

  // Make the set of all disjunctions in clause 2
  
  // For each disjunction in C1
    // For each disjunction in C2
    
      // If they're unifiable
        // create a new set from all the disjuncts in both clauses EXCEPT these two
        // Factor this new set
        // Build the resulting set into a new node
        // Add this new node to the resolvents set

  // Return the resolvents set
  return NULL;
}

cSet *factor(lst_node clause) {
  // For each pair of literals in the set
    // If they're unifiable
      // Remove the second one

  // Return the resulting set
  return NULL;
}

/*
 * NOTES:
 *  All of the roots will be disjunctions.
 *    THE FOLLOWING ARE TREATED AS COMPOUNDS:
 *      - DISJUNCTION_N
 *      - NEGATION_N
 *      - SKOLEM_N
 *      - FUNCTION_N
 *      - EQUALS_N
 *      - RELATION_N
 *
 *    UNLESS they have a right sibling, in which case they are a list
 *
 *  To my belief, this algorithm (from AIMA textbook) doesn't require
 *    modifying the actual trees. EXCEPT, when dealing with recursing on
 *    lists, where the link to the right sibling is nullified and then
 *    restored before returning.
 */
cSub *unify(lst_node c1, lst_node c2, cSub *sub) {
  fprintf(stdout, "In unify\n");
  // If substitution is failure, return failure
  if (sub->fail) {
    return sub;

  // else if x = y return sub
  } else if (c1->right_sib == NULL && c2->right_sib == NULL && tree_equals(c1, c2) == 0) {
    fprintf(stdout, "X = Y success\n");
    return sub;
  
  // else if x is a variable, recurse on unify var
  } else if (c1->node_type == VARIABLE_N) {
    fprintf(stdout, "var 1 is variable, => unify var\n");
    return unify_var(c1, c2, sub);

  // else if y is a variable, recurse on unify var
  } else if (c2->node_type == VARIABLE_N) {
    fprintf(stdout, "var 2 is variable, => unify var\n");
    return unify_var(c2, c1, sub);

  // else if both are compounds, recurse on their arguments
  } else if (c1->node_type != VARIABLE_N && c1->node_type != CONSTANT_N &&
             c2->node_type != VARIABLE_N && c2->node_type != CONSTANT_N &&
             c1->right_sib == NULL && c2->right_sib == NULL) {
    
    fprintf(stdout, "two compounds\n");
    lst_node op1 = shallow_copy(c1);
    lst_node op2 = shallow_copy(c2);

    // The list of arguments for a complex statment is its left child
    return unify(c1->left_child, c2->left_child, unify(op1, op2, sub));

  // else if both are lists, recurse on on their second+ arguments
  } else if (/*c1->node_type != VARIABLE_N && c1->node_type != CONSTANT_N &&
             c2->node_type != VARIABLE_N && c2->node_type != CONSTANT_N && */
             c1->right_sib != NULL && c2->right_sib != NULL) {

    // Make shallow copies of the arguments we're popping off
    fprintf(stdout, "Two lists\n");
    lst_node arg1 = shallow_copy(c1);
    lst_node arg2 = shallow_copy(c2);

    return unify(c1->right_sib, c2->right_sib, unify(arg1, arg2, sub));

  // else return failure
  } else {
    fprintf(stdout, "Failing by default\n");
    fprintf(stdout, "var 1 = %s, var 2 = %s\n", NODE_NAME(c1->node_type), NODE_NAME(c2->node_type));
    fprintf(stdout, "Var 1: \n");
    print_lst(stdout, c1, 0);
    fprintf(stdout, "Var 2: \n");
    print_lst(stdout, c2, 0);
    sub->fail = 1;
    return sub;
  }
}

cSub *unify_var(lst_node c1, lst_node c2, cSub *sub) {
  cSub *c1_found = find_sub(sub, c1);
  cSub *c2_found = find_sub(sub, c2);

  // If c1 has been substituted, recurse with said substitution
  if (c1_found) {
    return unify(c1_found->replacement, c1, sub);
  
  // else if c2 has been substituted, recurse with substitution
  } else if (c2_found) {
    return unify(c1, c2_found->replacement, sub);

  // else if OCCUR-CHECK (var, x) return failure
  } else if (occur_check(c1, c2)) {
    fprintf(stdout, "Failing by occur check\n");
    sub->fail = 1;
    return sub;

  // else add {c1, c2} to substitution and return
  } else {
    cSub *added = generate_sub(c1, c2);
    sub = add_sub(sub, added);
    return sub;
  }
}

int occur_check(lst_node var, lst_node x) {
  // if this node equal to var, return 1 (it occurs)
  if (tree_equals(var, x) == 0)
    return 1;

  // Recurse on each child of x
  for (lst_node curr = x->left_child; curr != NULL; curr = curr->right_sib)
    if (occur_check(var, curr))
      return 1;

  // Return 0 (it doesn't occur).
  return 0;
}
