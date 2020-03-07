/*
 * FILENAME: parse_unary_op.c
 *
 * DESCRIPTION:
 *     Implementation of functions for parsing unary operator expressions.
 *
 * AUTHOR: Moritz Röhrich <moritzr@pool.math.tu-berlin.de>
 *
 */

#include "parse.h"

asn* parse_unary_exp(token** tl, size_t* tnt, pv_root* symbol_map){
    printf("[%zu (%s)] parsing unary expr\n", (*tnt), (*tl)->str);

    token* tlp = *tl;
    int tok_type = tlp->type;

    asn* inner = NULL;
    if(tok_type == token_minus ||
        tok_type == not_kw ||
        tok_type == complement ||
        tok_type == token_ampersand ||
        tok_type == token_asterisk){

        pop_token(&tlp, tl, tnt);
        inner = parse_cast_exp(tl, tnt, symbol_map);
    } else if(tok_type == token_inc || tok_type == token_dec){
        pop_token(&tlp, tl, tnt);
        inner = parse_unary_exp(tl, tnt, symbol_map);
    } else {
        asn* postfix = parse_postfix_exp(tl, tnt, symbol_map);
        if(postfix != NULL)
            return postfix;

        return NULL;
    }

    if(inner == NULL){
        return NULL;
    }

    atomic_type inner_ty = get_atomic_type(inner, symbol_map);
    printf("found unary expr %d\n", tok_type);
    asn* ue = make_unary_exp(inner_ty, inner, tok_type);

    return ue;
}

