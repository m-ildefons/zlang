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
//	if(tlp->type == token_semi_colon)
//		pop_token(&tlp, tl, tnt);

	asn* call = make_call_exp(id, args);

	return call;
}

