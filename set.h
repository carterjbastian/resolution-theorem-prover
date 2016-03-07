/*
 * set.h    Type definitions for a set (of clauses) datatype
 *
 * Author:  Carter J. Bastian
 *          March 2016. 16W
 */
#ifndef SET_H_
#define SET_H_

typedef struct cSub {
  lst_node var;
  lst_node replacement;
  int fail;
} cSub;

cSub *generate_sub(lst_node var, lst_node replacement);




typedef struct cSet {
  lst_node val;
  struct cSet *prev;
  struct cSet *next;
} cSet;

cSet *generate_set();

cSet *addSet(cSet *set, lst_node val);

int inSet(cSet *set, lst_node val);

cSet *joinSet(cSet *s1, cSet *s2);

#endif // SET_H_
