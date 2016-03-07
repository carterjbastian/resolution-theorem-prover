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

}

cSet *factor(lst_node cSet *) {
  // For each pair of literals in the set
    // If they're unifiable
      // Remove the second one

  // Return the resulting set
}

/*
 * NOTES:
 *  All of the roots will be disjunctions.
 *    THE FOLLOWING ARE TREATED AS COMPOUNDS:
 *      - DISJUNCTION_N
 *      - NEGATTION_N
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
  // If substitution is failure, return failure
  if (sub->fail) {
    return sub;

  // else if x = y return sub
  } else if (tree_equals(c1, c2) == 0) {
    return sub;
  
  // else if x is a variable, recurse on unify var
  } else if (c1->node_type == VARIABLE_N) {

  // else if y is a variable, recurse on unify var
  } else if (c2->node_type == VARIABLE_N) {

  // else if both are compounds, recurse on their arguments
  } else if (c1->node_type != VARIABLE_N && c1->node_type != CONSTANT_N &&
             c2->node_type != VARIABLE_N && c2->node_type != CONSTANT_N &&
             c1->right_sib == NULL && c2->right_sib == NULL) {
 
  // else if both are lists, recurse on on their second+ arguments
  } else if (c1->node_type != VARIABLE_N && c1->node_type != CONSTANT_N &&
             c2->node_type != VARIABLE_N && c2->node_type != CONSTANT_N &&
             c1->right_sib != NULL && c2->right_sib != NULL) {

  // else return failure
  } else {
    sub->fail = 1;
    return sub;
  }
}

cSub *unify_var(lst_node c1, lst_node c2, cSub *sub) {
  // If c1 has been substituted, recurse with said substitution
  if (findSubstitution(sub, c1)) {

  // else if c2 has been substituted, recurse with substitution
  } else if (findSubstitution(sub, c2)) {

  // else if OCCUR-CHECK (var, x) return failure
  } else if (occur_check(var, x)) {

  // else add {c1, c2} to substitution and return
  } else {

  }
}

int occur_check(lst_node var, lst_node x) {
  // Loop through each node in x
    // if this node equal to var, return 1 (it occurs)
  

  // Return 0 (it doesn't occur).
  return 0;
}
