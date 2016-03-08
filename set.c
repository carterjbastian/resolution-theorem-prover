/*
 * set.c    Implementation of set and substitution datatypes
 *
 * Author:  Carter J. Bastian
 *          March 2016. 16W
 */
// System Includes
#include <stdlib.h>
#include <assert.h>

// Local Includes
#include "lst.h"
#include "set.h"

/* Implementations for the substitution datatype */
cSub *empty_sub() {
  cSub *ret = (cSub *) calloc(1, sizeof(cSub));
  assert(ret);
  return ret;
}

cSub *generate_sub(lst_node var, lst_node replacement) {
  cSub *ret = (cSub *) calloc(1, sizeof(cSub));
  assert(ret);
  ret->var = var;
  ret->replacement = replacement;
  // next, prev, and fail are zero'd out
  return ret;
}

cSub *add_sub(cSub *old, cSub *sub) {
  // Put it at the beginning of the list
  sub->next = old;

  return sub;
}

cSub *find_sub(cSub *sub, lst_node var) {
  for (cSub *curr = sub; curr != NULL; curr = curr->next)
    if (curr->var)
      if (tree_equals(var, curr->var) == 0)
        return curr;

  // We didn't find it
  return NULL;
}

/* Implementations for the set datatype */
cSet *generate_set() {
  // Zero out the memory initially
  cSet *ret = (cSet *) calloc(1, sizeof(cSet));

  return ret;
}

cSet *addSet(cSet *set, lst_node val) {
  // Create a new set with one item
  cSet *ret = generate_set();
  ret->val = val;

  // Link the new set in at beggining of old set (if it's non-null)
  ret->next = set;

  // Strip the empty set entry on the end of a set
  if (ret->next->val == NULL)
    ret->next = NULL;
  else
    ret->next->prev = ret;

  return ret;
}

int inSet(cSet *set, lst_node val) {
  for (cSet *curr = set; curr != NULL; curr = curr->next)
    if (tree_equals(val, curr->val) == 0)
      return 1;

  // We didn't find it
  return 0;
}

cSet *joinSet(cSet *s1, cSet *s2) {
  // Null guard
  if (s2 == NULL)
    return s1;
/*
  for (cSet *curr = s1; curr != NULL; curr = curr->next) {
    if (!s1->val)
      continue;

    if (inSet(curr, s1->val) == 0) {
      s2 = addSet(s2, s1->val);
    }
  }
  */
  
  // Loop to the last item in the second set
  cSet *last;
  for (last = s2; last->next != NULL; last = last->next)
    ;

  // Connect last item in second set to first item in first set
  last->next = s1;
  s2->prev = last;
  
  return s2;
}
