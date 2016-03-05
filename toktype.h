/*
 * File: toktype.h
 *  A file with the definitions of tokens use in the theorem prover.
 *
 * Authors: Carter J. Bastian
 *          March, 2016. 16W.
 *
 */
#ifndef TOKTYPE_H_
#define TOKTYPE_H_

enum TokenType { ID_T=258, FUNC_DEC_T, REL_DEC_T, CONST_DEC_T,
                  QUERY_DEC_T, VAR_DEC_T, IMPLIES_T, EQUIVALENT_T, EOF_T};

static char *tokens[] =
{ "ID", "FUNCTION_DECLARATION", "RELATION_DECLARATION", "CONSTANT_DECLARATION",
  "QUERY", "VARIABLE", "IMPLIES", "EQUIVALENT", "END_OF_FILE"};

#define FIRST_BIG_TOKEN ID_T
// so that it works even if ID_T ends up nonzero

char *token_name(int token);


#endif
