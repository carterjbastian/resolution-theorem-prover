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


/* 
 * The full enumeration of types of nodes in a Logic Syntax Tree.
 */
typedef enum { 
              KB_ROOT_N, PREDICATE_N, FUNCTION_N, LITERAL_N, COMPLEX_SENTENCE_N,
              ATOMIC_SENTENCE_N, NEGATION_N, CONJUNCTION_N, DISJUNCTION_N, 
              IMPLICATION_N, EQUIVALENCE_N, Q_EXISTS_N, Q_FORALL_N
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
    { KB_ROOT_N, "ROOT" },
    { PREDICATE_N, "PREDICATE" },
    { FUNCTION_N, "FUNCTION" },
    { LITERAL_N, "LITERAL" },
    { COMPLEX_SENTENCE_N, "COMPLEX_SENTENCE" },
    { ATOMIC_SENTENCE_N, "ATOMIC_SENTENCE" },
    { NEGATION_N, "~" },
    { CONJUNCTION_N, "^" },
    { DISJUNCTION_N, "v" },
    { IMPLICATION_N, "=>" },
    { EQUIVALENCE_N, "<=>" },
    { Q_EXISTS_N, "#" },
    { Q_FORALL_N, "@" }
};

#define NODE_INDEX(X)    ( (X) - ROOT_N)
#define NODE_NAME(X)     ( token_table[ NODE_INDEX((X)) ].name)

/* 
 * Structure for nodes of the logical syntax tree.  Uses the 
 * left-child/right-sibling representation, so that each node can have 
 * a variable number of children.
 */
typedef struct lst_node_struct *lst_node;
struct lst_node_struct {
  lst_node_type node_type;
  lst_node left_child, right_sib;
  char *value_string;
  int lineno;                   /* Line number for var declarations */  
};

/* Create a node with a given token type and return a pointer to the
   node. */
lst_node create_lst_node(lst_node_type node_type);

/* Print the contents of a subtree of an abstract syntax tree, given
   the root of the subtree and the depth of the subtree root. */
/*
void print_lst(FILE *fp, ast_node root, int depth);
*/
#endif // LST_H_
