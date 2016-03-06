/*
 * transformations.c    A transformations to an lst that convert to CNF
 *
 * Author:    Carter J. Bastian
 *            March, 2016. 16W.
 */


// A series of functions to be applied in order
lst_node remove_equivalence(lst_node root) {
  // if root is EQUIVALENCE_N
    // Replace with conjunction of two implications
  
  // recurse on each child
}

lst_node remove_implies(lst_node root) {
  // if root is implication
    // replace with conjunction
    // insert negation before left child

  // recurse on each child
}

lst_node move_negation_inward(lst_node root) {
  // if root is negation
    // if child is negation
      // remove both root and child

    // if child is conjunction
      // remove self
      // change child to disjunction
      // insert negation node before each of child's children

    // if child is disjunction
      // remove self
      // change child to conjunction
      // insert negation node before each of child's children

    // if child is universal quantifier
      // change self to existential quantifier
      // change child to negation

    // if child is existential quantifier
      // change self to universal quantifier
      // change child to negation
    
  // recurse on each child
}

lst_node standardize_variables(lst_node root) {
  // if root is quantifier
    // if var name is taken
      // new_var = generate_new_var()
      // replace_var(left_child, old_var_name, new_var_name);

  // recurse on each child
}

lst_node skolemize(lst_node root) {
  // if root is existential quantifier
    // loop backwards to create list of universally quantified variables in scope
    // F <- generate_skolem_func()
    // skolem_replace(child, root->value_string, F, UQVlist)

  // recurse on each child
}

lst_node drop_universal_quantifier(lst_node root) {
  // If root is universal quantifier
    // Delete self

  // Recurse on each child
}

lst_node distribute_conjunction(lst_node root) {
  // if root is disjunction
    // if right child is conjunction (CUTE HACK)
      // switch left and right child

    // if left child is conjunction
      // distributed1 = subtreeCopy(right child)
      // distributed2 = subtreeCopy(right child)
      // grandchild1 = left_child->left_child
      // grandchild2 = left_child->right_child

      // Change self to conjunction
      // left child <- disjunction
      // right child <- disjunction
      // left child->left child <- grandchild1
      // left child->right child <- distributed1
      // right child->left child <- grandchild2
      // right child->right child <- distributed2

  // Recurse on all children
}

/* Helper functions */
char *generate_new_var();
void replace_var(lst_node root, char *old_var, char *new_name);
char *generate_skolem_func();
void skolem_replace(lst_node root, char *old_var, char *skolem_function, symlist_node *UQV_list);
