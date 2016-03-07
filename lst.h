/*
 * lst.h    Defines the data types for a Logic Syntax Tree
 *
 * Author:  Carter J. Bastian
 *          March, 2016. 16W.
 *
 * Based on code written for cs57 by myself and Quinn Stearns '16 for the
 * pccbr compilers project. This means that the code may have structural
 * similarities to examples given by Thomas H. Cormen or Sean Smith in CS57
 * materials.
 */

#ifndef LST_H_
#define LST_H_

#include <stdio.h>

#define   MAX_VARIANTS  99
#define   SKOLEM_BASE   "$F"

/* 
 * The full enumeration of types of nodes in a Logic Syntax Tree.
 */
typedef enum { 
              NULL_N, KB_ROOT_N, RELATION_N, FUNCTION_N, CONSTANT_N, VARIABLE_N,
              NEGATION_N, CONJUNCTION_N, DISJUNCTION_N, EQUALS_N,
              IMPLICATION_N, EQUIVALENCE_N, Q_EXISTS_N, Q_FORALL_N, SKOLEM_N
} lst_node_type;

/* A struct used for lst_node_type -> string value conversion table */
typedef struct {
        int val;
        char *name;
} val_name_pair;


/*
 * A table of lst_node_type -> string representation pairs
 * NOTE: These must be in the same order as the enumerated values in the
 *  ast_node_type.
 */
static val_name_pair token_table[] = {
    { NULL_N, "NULL" },
    { KB_ROOT_N, "ROOT" },
    { RELATION_N, "RELATION" },
    { FUNCTION_N, "FUNCTION" },
    { CONSTANT_N, "CONSTANT" },
    { VARIABLE_N, "VARIABLE" },
    { NEGATION_N, "~" },
    { CONJUNCTION_N, "^" },
    { DISJUNCTION_N, "v" },
    { EQUALS_N, "=" },
    { IMPLICATION_N, "=>" },
    { EQUIVALENCE_N, "<=>" },
    { Q_EXISTS_N, "#" },
    { Q_FORALL_N, "@" },
    { SKOLEM_N, "SKOLEM_FUNCTION" }
};

#define NODE_INDEX(X)    ( (X) - NULL_N)
#define NODE_NAME(X)     ( token_table[ NODE_INDEX((X)) ].name)

/* 
 * Structure for nodes of the logical syntax tree.  Uses the 
 * left-child/right-sibling representation, so that each node can have 
 * a variable number of children.
 */
typedef struct lst_node_struct *lst_node;
struct lst_node_struct {
  lst_node_type node_type;
  lst_node left_child, right_sib, parent;
  char *value_string;
};

/* Create a node with a given token type and return a pointer to the
   node. */
lst_node create_lst_node(lst_node_type type);

/* Print the contents of a subtree of an abstract syntax tree, given
   the root of the subtree and the depth of the subtree root. */
void print_lst(FILE *fp, lst_node root, int depth);


typedef struct symlist_node {
        char *name;
        lst_node_type dec_type;
        struct symlist_node *next;
        struct symlist_node *prev;
        int used;
        int current_variant;
} symlist_node;

symlist_node *create_listnode(char *name);

symlist_node *append_symlist(symlist_node *list, char *name);

int tree_equals(lst_node t1, lst_node t2);

lst_node_type lookup_symbol_type(symlist_node *list, char *name);

symlist_node *lookup_symbol(symlist_node *list, char *name);

void print_symbol_list(FILE *fp, symlist_node *list);


#endif // LST_H_
