/*
 * lst.h    Implements the data types for a Logic Syntax Tree
 *
 * Author:  Carter J. Bastian
 *          March, 2016. 16W.
 *
 * Based on code written for cs57 by myself and Quinn Stearns '16 for the
 * pccbr compilers project. This means that the code may have structural
 * similarities to examples given by Thomas H. Cormen or Sean Smith in CS57
 * materials.
 */

// System Includes
#include <stdlib.h>

// Local Includes
#include "lst.h"

lst_node create_lst_node(lst_node_type type) {
  lst_node new_node = calloc(1, sizeof(struct lst_node_struct));
  new_node->node_type = type;
  return new_node;
}
