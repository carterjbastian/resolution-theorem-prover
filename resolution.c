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
cSet *generate_clauses(lst_node node, cSet *clauses);
cSet *get_disjunctions(lst_node node, cSet *disjuncts);
lst_node disjoin_clauses(cSet *clauses);

int resolution(FILE *fp, lst_node kb) {
  cSet *clauses = generate_set();
  cSet *new = generate_set();  
  int i = 0;

  lst_node empty_clause = create_lst_node(NULL_N);

  // Create the clauses set from the knowledge base
  for (lst_node curr = kb->left_child; curr != NULL; curr = curr->right_sib)
    clauses = generate_clauses(curr, clauses);

  if (!clauses)
    fprintf(stdout, "failed to make clauses\n");

  // Separate the clauses from each other
  fprintf(stdout, "CLAUSES:\n");
  for (cSet *clause = clauses; clause != NULL; clause = clause->next) {
    if (clause->val) {
      print_lst(stdout, clause->val, 0);
      fprintf(stdout, "\n");
      clause->val->right_sib = NULL;
    } else {
      clause = NULL; // remove the trailing empty clause
    }
  }
  
  // Factor the original set of clauses
  // Loop forever
//  while (++i > 0) {
  while(++i < 4) {
    fprintf(fp, "Inside while loop iteration %d\n", i);
    
    // Loop through each clause in the clauses set
    for (cSet *c1 = clauses; c1 != NULL; c1 = c1->next) {
      // Loop through each subsequent clause in the clauses set
      for (cSet *c2 = c1->next; c2 != NULL; c2 = c2->next) {
        // create new resolvents set by resolving the pair
        cSet *resolvents = resolve(c1->val, c2->val);

        // PRINT THE RESOLVENTS!
        fprintf(stdout, "\n\tRESOLVENTS\n");
        for (cSet *resolved = resolvents; resolved != NULL; resolved = resolved->next) {
          if (resolved->val) {
            print_lst(fp, resolved->val, 0);
            fprintf(fp, "\n");
          }
          
        }

        // if resolvents contains the empty clause, return true
//        if (inSet(resolvents, empty_clause)) // DONE WITH PROOF
        /* WARNING: MAKE SURE YOU PRINT THE END OF THE PROOF */
        if (resolvents->val) {
          if (resolvents->val->node_type == NULL_N)
            return 0;
        
          // join new and resolvents sets
          new = joinSet(new, resolvents);
        }
      }
    }
    fprintf(stdout, "\n\n\tNEW CLAUSES\n");
    for (cSet *clause = clauses; clause != NULL; clause = clause->next) {
      if (clause->val) {
        print_lst(stdout, clause->val, 0);
        fprintf(stdout, "\n");
        clause->val->right_sib = NULL;
      }
    }

    // if the new set is a subset of clauses, return false
    int subset = 1;
    for (cSet *cNew = new; cNew != NULL; cNew = cNew->next)
      if (! inSet(clauses, cNew->val))
        subset = 0;

    if (subset)
      return 1;
    else  // otherwise, join clauses with the new set and reiterate
      clauses = joinSet(clauses, new);



  }


  return 1; // Failure might never be reached
}

cSet *resolve(lst_node c1, lst_node c2) {
  fprintf(stdout, "Inside resolve\n");
  cSet *resolvents = generate_set();
  
  // Make the set of all disjunctions in clause 1
  cSet *disjuncts1 = get_disjunctions(c1, generate_set());

  fprintf(stdout, "Got first disjunction set\n");
  // Make the set of all disjunctions in clause 2
  cSet *disjuncts2 = get_disjunctions(c2, generate_set());
  fprintf(stdout, "Got second disjunction set\n");  
  // For each disjunction in C1
  for (cSet *d1 = disjuncts1; d1 != NULL; d1 = d1->next) {
    // For each disjunction in C2
    for (cSet *d2 = disjuncts2; d2 != NULL; d2 = d2->next) {
      // negate d2
      lst_node d2neg = create_lst_node(NEGATION_N);
      d2neg->left_child = d2->val; // QUESTION: Do I need to move ~ inwards again?

      // If d1 and ~d2 are unifiable
      fprintf(stdout, "Starting search for unifiable nodes\n");
      if ((unify(d1->val, d2neg, empty_sub()))->fail == 0) {
        fprintf(stdout, "Found unifiable pair\n");
        // create a new set from all the disjuncts in both clauses EXCEPT these two
        // Loop forwards and backwards through disjuncts1 adding all the clauses
        cSet *newSet = generate_set();
        for (cSet *f = d1->next; f != NULL; f = f->next)
          newSet = addSet(newSet, f->val);
        for (cSet *b = d1->prev; b != NULL; b = b->prev)
          newSet = addSet(newSet, b->val);

        // Loop forwards and backwards through disjuncts2 adding all the clauses
        for (cSet *f = d2->next; f != NULL; f = f->next)
          newSet = addSet(newSet, f->val);
        for (cSet *b = d2->prev; b != NULL; b = b->prev)
          newSet = addSet(newSet, b->val);

        // Check if this new set is empty
        if (!newSet || newSet->val == NULL) {
          fprintf(stdout, "Unifiable pair has empty clause\n");
          // Give it the empty clause
          newSet->val = create_lst_node(NULL_N);
          return newSet; // Fail early - this means we have proof
        } else {
          fprintf(stdout, "Factoring new set\n");
          // Factor this new set
          newSet = factor(newSet);

          // Build the resulting set into a new node
          if (newSet) {
            lst_node rejoined = disjoin_clauses(newSet);

            fprintf(stdout, "Rejoined lst_node:\n");
            print_lst(stdout, rejoined, 0);

            // Add this new node to the resolvents set
            cSet *newNode = generate_set();
            newNode->val = rejoined;

            resolvents = addSet(resolvents, rejoined);
          }
        } 
      } else {
        fprintf(stdout, "Found non-unifiable pair\n");
      }
    }
  }

  fprintf(stdout, "RETURNING FROM RESOLVE\n");
  // Return the resolvents set
  return resolvents;
}

cSet *factor(cSet *set) {
  // For each pair of literals in the set
  // For each disjunction in C1
  for (cSet *d1 = set; d1 != NULL; d1 = d1->next) {
    // For each disjunction in C2
    for (cSet *d2 = d1->next; d2 != NULL; d2 = d2->next) {
      // If they're unifiable
      if ((unify(d1->val, d2->val, empty_sub()))->fail == 0) {
        // Remove the second one
        /* CAUTION: modifying these links is scary and might make stuff whack? */
        d2->prev->next = d2->next;
        d2->next->prev = d2->prev;
      }
    }
  }
  // Return the resulting set
  return set;
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

  if (!c1 || !c2) {
    fprintf(stdout, "Failing from unify due to nullness\n");
    sub->fail = 1;
    return sub;
  }
  fprintf(stdout, "In unify\n");
  // If substitution is failure, return failure
  if (sub->fail) {
    return sub;

  // else if x = y return sub
  } else if (c1->right_sib == NULL && c2->right_sib == NULL && tree_equals(c1, c2) == 0) {
    //fprintf(stdout, "X = Y success\n");
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
             c1->right_sib == NULL && c2->right_sib == NULL &&
             c1->left_child != NULL && c2->left_child != NULL) {
    
    fprintf(stdout, "two compounds\n");
    lst_node op1 = shallow_copy(c1);
    lst_node op2 = shallow_copy(c2);

    fprintf(stdout, "C1:\n");
    print_lst(stdout, c1, 0);
    fprintf(stdout, "C2:\n");
    print_lst(stdout, c2, 0);



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
    //fprintf(stdout, "Failing by occur check\n");
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

cSet *generate_clauses(lst_node node, cSet *clauses) {
  // If this isn't conjunction, add this node
  if (node->node_type != CONJUNCTION_N) {
    clauses = addSet(clauses, node);
  } else { // otherwise, recurse on it's children
    for (lst_node curr = node->left_child; curr != NULL; curr = curr->right_sib)
      clauses = generate_clauses(curr, clauses);
  }

  return clauses;
}

cSet *get_disjunctions(lst_node node, cSet *disjuncts) {
  // If this isn't disjunction, add this node
  if (node) {
  if (node->node_type != DISJUNCTION_N) {
    disjuncts = addSet(disjuncts, node);
  } else { // otherwise, recurse on children
    for (lst_node curr = node->left_child; curr != NULL; curr = curr->right_sib)
      disjuncts = get_disjunctions(curr, disjuncts);
  }
  }

  return disjuncts;
}

lst_node disjoin_clauses(cSet *clauses) {
  lst_node root = create_lst_node(DISJUNCTION_N);

  lst_node node = root;
  while (clauses != NULL) {
    cSet *clause = clauses;
    clauses = clauses->next;

    // Last item
    if (clauses == NULL) {
      // Replace root with myself
      lst_node addIn = deep_copy(clause->val);
      addIn->parent = node->parent;
      if (node->parent)
        node->parent->left_child->right_sib = addIn;
 
      node = addIn;
    } else {
      node->left_child = deep_copy(clause->val);
      node->left_child->parent = node;
      node->left_child->right_sib = create_lst_node(DISJUNCTION_N);
      node->left_child->right_sib->parent = node;
      node = node->left_child->right_sib;
    }
  }

  return root;
}
