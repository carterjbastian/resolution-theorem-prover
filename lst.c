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
  symlist_node *new_node = create_listnode(name);
  list->prev = new_node;
  new_node->next = list;
  return new_node;
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

      default :
        fprintf(fp, "(unrecognized syntactical usage)");
    }

    fprintf(fp, "\n");
  }
}
