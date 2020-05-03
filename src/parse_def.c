/*
 * FILENAME: parse_def.c
 *
 * DESCRIPTION:
 *     Implementation of functions for parsing function definitions and
 *     declarations.
 *
 * AUTHOR: Moritz Röhrich <moritzr@pool.math.tu-berlin.de>
 *
 */

#include "parse.h"


/*
 * <function-definition> ::= {<declaration-specifier>}+ <declarator> : <compound-statement>
 */
asn* parse_function_definition(token** tl,
                                size_t* tnt,
                                asn* function){
    printf("[%zu (%s)] parsing function\n", (*tnt), (*tl)->str);
    token* tlp = (*tl);
    int scope = tlp->level;

    if(function == NULL)
        return NULL;

    symbol_list* old_symbol_list_ptr = symbol_list_ptr;
    symbol_list_ptr = function->op.fun_def_exp.symbols;

    tlp = (*tl);
    if(tlp->type == token_colon){
        pop_token(&tlp, tl, tnt);
        parse_compound_statement(tl, tnt, &(function->op.fun_def_exp.body), scope);
    } else {
        return NULL;
    }

    symbol_list_ptr = old_symbol_list_ptr;
    return function;
}

