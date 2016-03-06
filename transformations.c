/*
 * transformations.c    A transformations to an lst that convert to CNF
 *
 * Author:    Carter J. Bastian
 *            March, 2016. 16W.
 */
//System includes
#include <stdlib.h>

// Local includes
#include "lst.h"
#include "transformations.h"

/* Declarations of private helper functions */
lst_node copy_tree(lst_node subtree);

char *generate_new_var();
void replace_var(lst_node root, char *old_var, char *new_name);
char *generate_skolem_func();
void skolem_replace(lst_node root, char *old_var, char *skolem_function, symlist_node *UQV_list);

// A series of functions to be applied in order

/*
 * A function to initialize all of the parent links for the nodes.
 * AKA, we're giving all the nodes parents
 *
 */
lst_node adoption_agency(lst_node root) {
  // For each child of mine, it's parent is me! And recurse
  for (lst_node child = root->left_child; child != NULL; child = child->right_sib) {
    child->parent = root;
    adoption_agency(child);
  }

  return root;
}

lst_node remove_equivalence(lst_node root) {
  if (root->node_type == EQUIVALENCE_N) { // if root is EQUIVALENCE_N
    /* Replace equivalence with a conjunction of two implications */
    // Convert to conjunction
    root->node_type = CONJUNCTION_N;

    // Make a copy of each child (for the second imply node)
    lst_node lChild_copy = copy_tree(root->left_child);
    lst_node rChild_copy = copy_tree(root->left_child->right_sib);

    // Create two imply nodes
    lst_node left_imply = create_lst_node(IMPLICATION_N);
    lst_node right_imply = create_lst_node(IMPLICATION_N);

    // Insert the left imply
    left_imply->parent = root;
    left_imply->left_child = root->left_child;
    root->left_child->parent = left_imply;
    root->left_child->right_sib->parent = left_imply;
    root->left_child = left_imply;

    // Insert the right imply
    right_imply->parent = root;
    rChild_copy->parent = right_imply;
    lChild_copy->parent = right_imply;

    right_imply->left_child = rChild_copy;
    right_imply->left_child->right_sib = lChild_copy;

    root->left_child->right_sib = right_imply;

  }
  
  // recurse on each child
  for (lst_node curr = root->left_child; curr != NULL; curr = curr->right_sib)
    remove_equivalence(curr);

  return root;
}

lst_node remove_implies(lst_node root) {
  
  if (root->node_type == IMPLICATION_N) { // if root is implication 
    // replace with disjunction
    root->node_type = DISJUNCTION_N;

    // insert negation before left child
    lst_node inserted_neg = create_lst_node(NEGATION_N);
    
    inserted_neg->parent = root;
    root->left_child->parent = inserted_neg;
    
    inserted_neg->left_child = root->left_child;
    root->left_child = inserted_neg;
  
  }

  // recurse on each child
  for (lst_node curr = root->left_child; curr != NULL; curr = curr->right_sib)
    remove_implies(curr);

  return root;
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
  for (lst_node curr = root->left_child; curr != NULL; curr = curr->right_sib)
    move_negation_inward(curr);

  return root;
}

lst_node standardize_variables(lst_node root) {
  // if root is quantifier
    // if var name is taken
      // new_var = generate_new_var()
      // replace_var(left_child, old_var_name, new_var_name);

  // recurse on each child
  for (lst_node curr = root->left_child; curr != NULL; curr = curr->right_sib)
    standardize_variables(curr);

  return root;
}

lst_node skolemize(lst_node root) {
  // if root is existential quantifier
    // loop backwards to create list of universally quantified variables in scope
    // F <- generate_skolem_func()
    // skolem_replace(child, root->value_string, F, UQVlist)

  // recurse on each child
  for (lst_node curr = root->left_child; curr != NULL; curr = curr->right_sib)
    skolemize(curr);

  return root;
}

lst_node drop_universal_quantifier(lst_node root) {
  // If root is universal quantifier
    // Delete self

  // Recurse on each child
  for (lst_node curr = root->left_child; curr != NULL; curr = curr->right_sib)
    drop_universal_quantifier(curr);

  return root;
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
  for (lst_node curr = root->left_child; curr != NULL; curr = curr->right_sib)
    distribute_conjunction(curr);

  return root;
}

/* Helper functions implementations */

/*
 * copy_tree  - returns an identical copy of an lst subtree starting at the root
 *
 * NOTE: it will return having the same parent as the node it copied
 */
lst_node copy_tree(lst_node original) {
  if (!original)
    return NULL;

  lst_node copy = create_lst_node(original->node_type);

  // Copy the rest of the node's attributes
  copy->parent = original->parent;
  copy->value_string = original->value_string;

  // Recurse on children
  for (lst_node curr = original->left_child; curr != NULL; curr = curr->right_sib) {
    // Is this the first child?
    if (copy->left_child == NULL) {
      copy->left_child = copy_tree(curr);
    } else {
      // Loop to the copy's last child and give it a right sibling
      lst_node temp;
      for (temp = copy->left_child; temp->right_sib != NULL; temp = temp->right_sib)
        ;
      temp->right_sib = copy_tree(curr);
    }
  }

  return copy;
}
