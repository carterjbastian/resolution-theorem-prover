/*
 * lst.c    Implements the data types for a Logic Syntax Tree
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
#include <assert.h>
#include <string.h>

// Local Includes
#include "lst.h"

lst_node create_lst_node(lst_node_type type) {
  lst_node new_node = calloc(1, sizeof(struct lst_node_struct));
  new_node->node_type = type;
  return new_node;
}

lst_node shallow_copy(lst_node node) {
  lst_node copy = create_lst_node(node->node_type);
  if (node->value_string)
    copy->value_string = strdup(node->value_string);

  return copy;
}
 
void print_lst(FILE *fp, lst_node root, int depth) {
  /* Print two spaces for every level of depth. */
  int i;

  assert(root);
  
  /* Syntactic sugar */
  fprintf(fp, "| ");
  for (i = 0; i < depth; i++)
    fprintf(fp, "- ");

  /* Print the node type. */
  fprintf(fp, "%s ", NODE_NAME(root->node_type));

  if (root->value_string != NULL)
    fprintf(fp, "%s", root->value_string);

  fprintf(fp, "\n");

  /* Recurse on each child of the subtree root, with a depth one
     greater than the root's depth. */
  lst_node child;
  for (child = root->left_child; child != NULL; child = child->right_sib)
    print_lst(fp, child, depth + 1);
}  


symlist_node *create_listnode(char *name) {
  symlist_node *new_node = calloc(1, sizeof(symlist_node));
  new_node->name = name;
  return new_node;
}

symlist_node *append_symlist(symlist_node *list, char *name) {
  if (!list)
    return create_listnode(name);
  
  symlist_node *new_node = create_listnode(name);
  list->prev = new_node;
  new_node->next = list;
  return new_node;
}

int tree_equals(lst_node t1, lst_node t2) {
  // Check that the current node's values are the same for both trees
  // Must check both node type and value string
  if (t1->node_type != t2->node_type)
    return 1;
  
  if (t1->value_string) { // t1 has a value string
    if (!t2->value_string) {
      return 1;
    } else {
      if (strcmp(t1->value_string, t2->value_string) != 0)
        return 1;
    }
  } else { // t1 doesn't have a value string
    if (t2->value_string)
      return 1;
  }

  lst_node t1_child, t2_child;
  // Recurse on each set of sub-trees for the two trees
  for (t1_child = t1->left_child, t2_child = t2->left_child;
        t1_child != NULL; 
        t1_child = t1_child->right_sib) {
    
    // Null guarding
    if (t2_child == NULL) // t1_child is not, so fail
      return 1;

    // recurse
    if (tree_equals(t1_child, t2_child) != 0)
      return 1;

    // change t2_child
    t2_child = t2_child->right_sib;
  }

  // IF t1_child is null and t2_child isn't, the trees are different
  if (t2_child)
    return 1;

  // No equality test failed
  return 0;
}

lst_node_type lookup_symbol_type(symlist_node *list, char *name) {
  for (symlist_node *curr = list; curr != NULL; curr = curr->next)
    if (strcmp(curr->name, name) == 0)
      return (curr->dec_type);

  return NULL_N;
}


symlist_node *lookup_symbol(symlist_node *list, char *name) {
  for (symlist_node *curr = list; curr != NULL; curr = curr->next)
    if (strcmp(curr->name, name) == 0)
      return curr;
 
  return NULL;
}

void print_symbol_list(FILE *fp, symlist_node *list) {
  for (symlist_node *curr = list; curr != NULL; curr = curr->next) {
    fprintf(fp, "%s\t", curr->name);

    switch (curr->dec_type) {
      case CONSTANT_N :
        fprintf(fp, "(constant)");
        break;

      case RELATION_N :
        fprintf(fp, "(relation)");
        break;

      case FUNCTION_N :
        fprintf(fp, "(function)");
        break;

      case VARIABLE_N :
        fprintf(fp, "(variable)");
        break;

      case SKOLEM_N :
        fprintf(fp, "(skolem function)");
        break;

      default :
        fprintf(fp, "(unrecognized syntactical usage)");
    }

    fprintf(fp, "\n");
  }
}
