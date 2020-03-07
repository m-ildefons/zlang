/*
 * FILENAME: parse_jump.c
 *
 * DESCRIPTION:
 *     Implementation of functions for parsing jump, return and function call
 *     expressions.
 *
 * AUTHOR: Moritz Röhrich <moritzr@pool.math.tu-berlin.de>
 *
 */

#include "parse.h"

asn* parse_ret_exp(token** tl, size_t* tnt, pv_root* symbol_map){
    token* tlp = *tl;
    if(tlp->type != return_kw){
        return NULL;
    }

    printf("[%zu (%s)] parsing return\n", (*tnt), (*tl)->str);

	pop_token(&tlp, tl, tnt);

    if(tlp->type == token_semi_colon){
		pop_token(&tlp, tl, tnt);
    }

    asn* inner = parse_bin_exp(tl, tnt, symbol_map);
    if(inner == NULL)
        return NULL;
    asn* s = make_ret_exp(inner);

    return s;
}

asn* parse_fun_call_exp(token** tl, size_t* tnt, pv_root* symbol_map){
	token* tlp = (*tl);

	if(tlp->type != ident)
		return NULL;

	char* id = tlp->str;

	if((tlp+1)->type != open_p)
		return NULL;

    printf("[%zu (%s)] parsing function call\n", (*tnt), (*tl)->str);

	pop_token(&tlp, tl, tnt);
	pop_token(&tlp, tl, tnt);

	asn_list* args = NULL;
	asn* arg = NULL;
    for(; tlp->type != close_p; ){
		arg = parse_exp(tl, tnt, symbol_map);
		tlp = (*tl);
		if(tlp->type == token_comma)
			pop_token(&tlp, tl, tnt);
		append_exp_list(&args, arg);
	}
	if(tlp->type != close_p){
		abort();
    }

	pop_token(&tlp, tl, tnt);
	if(tlp->type == token_semi_colon)
		pop_token(&tlp, tl, tnt);

	asn* call = make_call_exp(id, args);

	return call;
}

asn* parse_jump_exp(token** tl, size_t* tnt){
	token* tlp = (*tl);
	asn* jump = NULL;
	if(tlp->type == break_kw || tlp->type == continue_kw){
        printf("[%zu (%s)] parsing jump\n", (*tnt), (*tl)->str);
		jump = make_jump_exp(tlp->type);
		pop_token(&tlp, tl, tnt);
		if(tlp->type == token_semi_colon)
			pop_token(&tlp, tl, tnt);
	}
	return jump;
}

