/*
 * resolution.h   function prototypes for resolution theorem proving
 * 
 * Author:    Carter J. Bastian
 *            March 2016. 16W
 */

#ifndef RESOLUTION_H_
#define RESOLUTION_H_

#include "set.h"
#include "lst.h"

int resolution(FILE *fp, lst_node kb);

cSet *resolve(lst_node c1, lst_node c2);

cSet *factor(lst_node clause);

cSub *unify(lst_node c1, lst_node c2, cSub *sub);

#endif // RESOLUTION_H_
