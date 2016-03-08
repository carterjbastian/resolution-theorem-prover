/*
 * set.h    Type definitions for set and substitution datatypes
 *
 * Author:  Carter J. Bastian
 *          March 2016. 16W
 */
#ifndef SET_H_
#define SET_H_

#include "lst.h"

/* Declarations for the clause substitution (cSub) datatype */
typedef struct cSub {
  lst_node var;
  lst_node replacement;
  struct cSub *next;
//  struct cSub *prev;
  int fail;
} cSub;

cSub *empty_sub();

cSub *generate_sub(lst_node var, lst_node replacement);

cSub *add_sub(cSub *old, cSub *sub);

cSub *find_sub(cSub *sub, lst_node var);


/* Declarations for the clause set (cSet) datatype */
typedef struct cSet {
  lst_node val;
  struct cSet *next;
  struct cSet *prev;
} cSet;

void *print_set();

cSet *generate_set();

cSet *addSet(cSet *set, lst_node val);

int inSet(cSet *set, lst_node val);

cSet *joinSet(cSet *s1, cSet *s2);

#endif // SET_H_
