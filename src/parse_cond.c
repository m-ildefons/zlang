/*
 * FILENAME: parse_cond.c
 *
 * DESCRIPTION:
 *     Implementation of functions for parsing conditional expressions.
 *
 * AUTHOR: Moritz Röhrich <moritzr@pool.math.tu-berlin.de>
 *
 */

#include "parse.h"

asn* parse_cond_exp(token** tl, size_t* tnt, pv_root* symbol_map){
    token* tlp = (*tl);
    int scope = tlp->level;

    if(tlp->type != token_cond_if && tlp->type != token_cond_else){
        return NULL;
	}
    printf("parsing conditional expr. %lu tokens left.\n", (*tnt));
	pop_token(&tlp, tl, tnt);

    asn* condition = parse_bin_exp(tl, tnt, symbol_map);
    asn* cond_exp = make_cond_exp(condition, NULL, NULL, scope);

    tlp = (*tl);
    if(tlp->type != token_colon){
        return NULL;
	}
	pop_token(&tlp, tl, tnt);

	symbol_map = symbol_map_copy(symbol_map);
	symbol_map->scope = scope + 1;

    parse_body(tl, tnt, &(cond_exp->op.cond_exp.if_body), &symbol_map, scope);
    cond_exp->op.cond_exp.if_symbol_map = symbol_map;

	tlp = (*tl);
	if(tlp != NULL && tlp->type == token_cond_else && tlp->level == scope){
		pop_token(&tlp, tl, tnt);

    	if(tlp->type == token_colon){
			pop_token(&tlp, tl, tnt);
		} else if(tlp->type != token_cond_if){
			return NULL;
		}

    	parse_body(tl, tnt, &(cond_exp->op.cond_exp.else_body), &symbol_map, scope);
    	cond_exp->op.cond_exp.else_symbol_map = symbol_map;
	}
    return cond_exp;
}

