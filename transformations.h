/*
 * transformations.h    Transformations to an lst that convert to CNF
 *
 * Author:    Carter J. Bastian
 *            March, 2016. 16W.
 */

#ifndef TRANSFORMATIONS_H_
#define TRANSFORMATIONS_H_

#include "lst.h"

// A series of functions to be applied in order
lst_node adoption_agency(lst_node root);
lst_node remove_equivalence(lst_node root);
lst_node remove_implies(lst_node root);
lst_node move_negation_inward(lst_node root);
lst_node standardize_variables(lst_node root);
lst_node skolemize(lst_node root);
lst_node drop_universal_quantifier(lst_node root);
lst_node distribute_disjunction(lst_node root);
lst_node clean_tree(lst_node root);
#endif // TRANSFORMATIONS_H_
