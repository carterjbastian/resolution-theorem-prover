/*
 * resolution.c   functions implementing resolution-based theorem proving
 * 
 * Author:    Carter J. Bastian
 *            March 2016. 16W
 */

#include <stdlib.h>
#include <stdio.h>

#include "set.h"
#include "lst.h"
#include "resolution.h"

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

cSub *unify(lst_node c1, lst_node c2, cSub *sub) {


}
