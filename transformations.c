/*
 * transformations.c    A transformations to an lst that convert to CNF
 *
 * Author:    Carter J. Bastian
 *            March, 2016. 16W.
 */
//System includes
#include <stdlib.h>
#include <assert.h>
#include <string.h>

// Local includes
#include "lst.h"
#include "transformations.h"

/* Declarations of private helper functions */
lst_node copy_tree(lst_node subtree);

char *generate_variant(symlist_node *original, symlist_node **symbol_list_p);
void replace_var(lst_node root, char *old_var, char *new_name);
char *generate_skolem_function(symlist_node **symbol_list_p);
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
    inserted_neg->right_sib = root->left_child->right_sib; // Move the sibling up
    root->left_child->right_sib = NULL; // Cancel old link
    root->left_child = inserted_neg;
  
  }

  // recurse on each child
  for (lst_node curr = root->left_child; curr != NULL; curr = curr->right_sib)
    remove_implies(curr);

  return root;
}

lst_node move_negation_inward(lst_node root) {
  if (root->node_type == NEGATION_N) { // if root is negation
    lst_node new_child;

    lst_node new_neg1;
    lst_node new_neg2;

    switch(root->left_child->node_type) {
      // if child is negation
      case NEGATION_N:
        //fprintf(stdout, "Found case 1, Negation\n");
        // remove both root and child by setting them to NULL_N
        root->node_type = NULL_N;
        root->left_child->node_type = NULL_N;
        break;

      // if child is conjunction
      case CONJUNCTION_N:
        //fprintf(stdout, "Found case 2, Conjunction\n");
        // remove self
        root->node_type = NULL_N;

        // change child to disjunction
        root->left_child->node_type = DISJUNCTION_N;

        // insert negation node before each of child's children
        new_neg1 = create_lst_node(NEGATION_N);
        new_neg2 = create_lst_node(NEGATION_N);

        new_neg1->left_child = root->left_child->left_child;
        new_neg1->parent = root->left_child; // Set up links for new node
        root->left_child->left_child->parent = new_neg1;
        root->left_child->left_child = new_neg1; // update links to new node

        new_neg2->left_child = root->left_child->left_child->left_child->right_sib; 
        new_neg2->parent = root->left_child; // Set up links for new node
        new_neg2->left_child->parent = new_neg2;
        root->left_child->left_child->right_sib = new_neg2; // Update links to new node
        root->left_child->left_child->left_child->right_sib = NULL; // Delete the old link

        break;

      // if child is disjunction
      case DISJUNCTION_N:
        //fprintf(stdout, "Found case 3, disjunction\n");
        // remove self
        root->node_type = NULL_N;

        // change child to disjunction
        root->left_child->node_type = CONJUNCTION_N;

        // insert negation node before each of child's children
        new_neg1 = create_lst_node(NEGATION_N);
        new_neg2 = create_lst_node(NEGATION_N);

        new_neg1->left_child = root->left_child->left_child;
        new_neg1->parent = root->left_child; // Set up links for new node
        root->left_child->left_child->parent = new_neg1;
        root->left_child->left_child = new_neg1; // update links to new node

        new_neg2->left_child = root->left_child->left_child->left_child->right_sib; 
        new_neg2->parent = root->left_child; // Set up links for new node
        new_neg2->left_child->parent = new_neg2;
        root->left_child->left_child->right_sib = new_neg2; // Update links to new node
        root->left_child->left_child->left_child->right_sib = NULL; // Delete the old link

        break;

      // if child is universal quantifier
      case Q_FORALL_N:
        //fprintf(stdout, "Found case 4, Universal Quantifier\n");
        // change self to existential quantifier
        root->node_type = Q_EXISTS_N;
        root->value_string = root->left_child->value_string; // Copy the variable name

        // change child to negation
        root->left_child->node_type = NEGATION_N;
        root->left_child->value_string = NULL;

        break;

      // if child is existential quantifier
      case Q_EXISTS_N:
        //fprintf(stdout, "Found case 5, Existential Quantifier\n");
        // change self to universal quantifier
        root->node_type = Q_FORALL_N;
        root->value_string = root->left_child->value_string; // Copy the variable name

        // change child to negation
        root->left_child->node_type = NEGATION_N;
        root->left_child->value_string = NULL;

        break;

      default :
        ; //fprintf(stdout, "hit the default\n"); // Do nothing
    }
  }
    
  // recurse on each child
  for (lst_node curr = root->left_child; curr != NULL; curr = curr->right_sib)
    move_negation_inward(curr);

  return root;
}

lst_node standardize_variables(lst_node root, symlist_node **list_p) {
  // if root is quantifier
  if (root->node_type == Q_FORALL_N || root->node_type == Q_EXISTS_N) {
    char *original_name = root->value_string;
    assert(original_name);
    symlist_node *original = lookup_symbol(*list_p, original_name);
    assert(original);
    
    // if var name is taken
    if (original->used) {
      // Create a new variable to replace the taken one
      char *new_var = generate_variant(original, list_p);

      // Quantifiers have only one child each
      root->value_string = new_var; // Update the quantifier
      replace_var(root->left_child, original_name, new_var);
    
    } else {
      original->used = 1; // Mark this var as having been used
    }
  }

  // recurse on each child
  for (lst_node curr = root->left_child; curr != NULL; curr = curr->right_sib)
    standardize_variables(curr, list_p);

  return root;
}

lst_node skolemize(lst_node root, symlist_node **list_p) {
  // if root is existential quantifier
  if (root->node_type == Q_EXISTS_N) {
    symlist_node *UQVlist = NULL;

    // loop backwards to create list of universally quantified variables in scope
    for (lst_node curr = root->parent; curr != NULL; curr = curr->parent)
      if (curr->node_type == Q_FORALL_N)
        UQVlist = append_symlist(UQVlist, curr->value_string);

    // Make a skolem function
    char *skolem_name = generate_skolem_function(list_p);

    // Replace every instance of the quantified variable with the skolem function
    skolem_replace(root->left_child, root->value_string, skolem_name, UQVlist);
     
    // Delete the root
    root->value_string = NULL;
    root->node_type = NULL_N;
  }
  // recurse on each child
  for (lst_node curr = root->left_child; curr != NULL; curr = curr->right_sib)
    skolemize(curr, list_p);

  return root;
}

lst_node drop_universal_quantifier(lst_node root) {
  // If root is universal quantifier
  if (root->node_type == Q_FORALL_N) {
    // Delete self
    root->node_type = NULL_N;
    root->value_string = NULL;
  }

  // Recurse on each child
  for (lst_node curr = root->left_child; curr != NULL; curr = curr->right_sib)
    drop_universal_quantifier(curr);

  return root;
}

lst_node distribute_disjunction(lst_node root) {
  // if root is disjunction
  if (root->node_type == DISJUNCTION_N) {

    // if right child is conjunction (CUTE HACK)
    if (root->left_child->right_sib->node_type == CONJUNCTION_N) {
      // switch left and right child to avoid having two cases.
      // This algorithm works the same on cases where both nodes are conjunction
      root->left_child->right_sib->right_sib = root->left_child;
      root->left_child = root->left_child->right_sib;
      root->left_child->right_sib->right_sib = NULL;
    }

    // if left child is conjunction
    if (root->left_child->node_type == CONJUNCTION_N) {
      // Make two copies of the right child (to be distributed)
        // distributed1 = subtreeCopy(right child)
        // distributed2 = subtreeCopy(right child)
        // grandchild1 = left_child->left_child
        // grandchild2 = left_child->right_child

      lst_node distrib1 = copy_tree(root->left_child->right_sib);
      distrib1->right_sib = NULL;
      lst_node distrib2 = copy_tree(root->left_child->right_sib);
      distrib2->right_sib = NULL;

      lst_node grandchild1 = root->left_child->left_child;
      lst_node grandchild2 = root->left_child->left_child->right_sib;

      lst_node disjunct1 = create_lst_node(DISJUNCTION_N);
      lst_node disjunct2 = create_lst_node(DISJUNCTION_N);


      // Change self to conjunction with two child disjunctions
        // left child <- disjunction
        // right child <- disjunction
        // left child->left child <- grandchild1
        // left child->right child <- distributed1
        // right child->left child <- grandchild2
        // right child->right child <- distributed2

      root->node_type = CONJUNCTION_N;
      root->left_child = disjunct1;
      root->left_child->right_sib = disjunct2;

      root->left_child->left_child = grandchild1;
      root->left_child->left_child->right_sib = distrib1;
      
      root->left_child->right_sib->left_child = grandchild2;
      root->left_child->right_sib->left_child->right_sib = distrib2;

      // Update parent links
      grandchild2->parent = root->left_child->right_sib;
      distrib2->parent = root->left_child->right_sib;

      grandchild1->parent = root->left_child;
      distrib1->parent = root->left_child;

      root->left_child->right_sib->parent = root;
      root->left_child->parent = root;
    }
  }

  // Recurse on all children
  for (lst_node curr = root->left_child; curr != NULL; curr = curr->right_sib)
    distribute_disjunction(curr);

  return root;
}

lst_node clean_tree(lst_node *root) {
  // If the node is a NULL_N
  if ((*root)->node_type == NULL_N) {  
    fprintf(stdout, "Found a null node\n");
    // Remove it from the tree
    
    // Find where it is in relation to its siblings
    lst_node lSib = NULL;
    if ((*root)->parent->left_child == (*root)) {
      fprintf(stdout, "It's the leftmost child\n");
      (*root)->parent->left_child = (*root)->left_child;
      (*root)->left_child->right_sib = (*root)->right_sib;
      (*root)->left_child->parent = (*root)->parent;
      *root = (*root)->parent;

    } else {
      fprintf(stdout, "It's not the leftmost child\n");
      for (lSib = (*root)->parent->left_child; lSib->right_sib != NULL && lSib->right_sib != *root; lSib = lSib->right_sib)
        ;

      lSib->right_sib = (*root)->left_child;
      (*root)->left_child->right_sib = (*root)->right_sib;
      (*root)->left_child->parent = (*root)->parent;
      *root = (*root)->parent;      
    }
  }
    
  // Recurse on children
  for (lst_node curr = (*root)->left_child; curr != NULL; curr = curr->right_sib)
    curr = clean_tree(&curr);

  return *root;
}

/* Helper functions implementations */

/*
 * generate_variant - creates a new variable as a variant of an original
 *
 * returns the new variable's name
 */
char *generate_variant(symlist_node *original, symlist_node **symbol_list_p) {
  // Get the original name
  char *original_name = original->name;
  assert(original_name);
  int original_len = strlen(original_name);

  // We want null termination => calloc
  // Allocate space for original name + up to 2 digits + null terminator
  char *variant_name = (char *) calloc(1, original_len + 2 + 1);

  int variant_number = ++(original->current_variant); // Increment before using
  assert(variant_number <= MAX_VARIANTS); // Edgecase guarding
  assert(variant_number > 0);             // No namespace overlap

  // Create the new variant's name
  sprintf(variant_name, "%s%d", original_name, variant_number);

  // Add the new variant to the symbol table
  *symbol_list_p = append_symlist(*symbol_list_p, variant_name);
  assert(*symbol_list_p);
  (*symbol_list_p)->used = 1;
  (*symbol_list_p)->current_variant = -1;
  (*symbol_list_p)->dec_type = VARIABLE_N;
  return variant_name;
}

/*
 * generate_skolem_function - creates a new skolem function
 *
 * returns its name
 */
char *generate_skolem_function(symlist_node **symbol_list_p) {
  // Check if this is the first skolem function
  char *skolem_name = SKOLEM_BASE;

  if (lookup_symbol(*symbol_list_p, skolem_name)) {
    // We need a skolem variant
    symlist_node *skolem_node = lookup_symbol(*symbol_list_p, skolem_name);
    skolem_name = generate_variant(skolem_node, symbol_list_p);
    skolem_node = lookup_symbol(*symbol_list_p, skolem_name);
    skolem_node->dec_type = SKOLEM_N;
    return skolem_name;
  } else {
    // We need to make the first skolem function
    *symbol_list_p = append_symlist(*symbol_list_p, skolem_name);
    assert(*symbol_list_p);
  
    (*symbol_list_p)->used = 1;
    (*symbol_list_p)->dec_type = SKOLEM_N;
    return skolem_name;
  }
}
/*
 * replace_var - recursively replaces one variable with another in a subtree
 *
 */
void replace_var(lst_node root, char *old_var, char *new_name) {
  // if root is an id and it's name is that of a variable
  if (root->node_type == VARIABLE_N)
    if (strcmp(root->value_string, old_var) == 0) 
      root->value_string = new_name;

  // Recurse on each child
  for (lst_node curr = root->left_child; curr != NULL; curr = curr->right_sib)
    replace_var(curr, old_var, new_name);
}

/*
 * skolem_replace - recursively replaces a variabe with its skolem function
 *
 */
void skolem_replace(lst_node root, char *old_var, char *skolem_function, symlist_node *UQV_list) {
  // if root is a variable id
  if (root->node_type == VARIABLE_N) {
    if (strcmp(root->value_string, old_var) == 0) {
      root->node_type = SKOLEM_N;
      root->value_string = skolem_function;

      // For each item in the UQV_list, make it a child of the skolem function
      for (symlist_node *UQV = UQV_list; UQV != NULL; UQV = UQV->next) {
        // Add the child in the right place
        if (root->left_child) {
          // Loop to the last child
          lst_node child;
          for (child = root->left_child; child->right_sib != NULL; child = child->right_sib)
            ;
          
          // Give it a new brother
          child->right_sib = create_lst_node(VARIABLE_N);
          child->right_sib->value_string = UQV->name;
          child->right_sib->parent = root;

        } else {
          // It will be the first child
          root->left_child = create_lst_node(VARIABLE_N);
          root->left_child->value_string = UQV->name;
          root->left_child->parent = root;
        }
      }

    }
  }

  // Recurse on all children
  for (lst_node curr = root->left_child; curr != NULL; curr = curr->right_sib)
    skolem_replace(curr, old_var, skolem_function, UQV_list);
}

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
