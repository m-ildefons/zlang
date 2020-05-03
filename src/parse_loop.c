/*
 * FILENAME: parse_loop.c
 *
 * DESCRIPTION:
 *     Implementation of functions for parsing loop expressions.
 *
 * AUTHOR: Moritz Röhrich <moritzr@pool.math.tu-berlin.de>
 *
 */

#include "parse.h"

asn* parse_for_loop_exp(token** tl, size_t* tnt){
	token* tlp = (*tl);
	int scope = tlp->level;

	if(tlp->type != token_loop_for)
		return NULL;

	printf("[%zu (%s)] parsing for loop\n", (*tnt), (*tl)->str);
	pop_token(&tlp, tl, tnt);

    symbol_list* for_symbols = new_symbol_list((size_t) scope + 1);
    symbol_list_attach(&symbol_list_ptr, &for_symbols);
    symbol_list* old_symbol_list_ptr = symbol_list_ptr;
    symbol_list_ptr = for_symbols;

	asn* init = parse_exp(tl, tnt);

	tlp = (*tl);
	if(tlp != NULL && tlp->type == token_semi_colon)
		pop_token(&tlp, tl, tnt);

	asn* cond = parse_expression(tl, tnt);

	tlp = (*tl);
	if(tlp != NULL && tlp->type == token_semi_colon)
		pop_token(&tlp, tl, tnt);

	asn* move = parse_expression(tl, tnt);

	asn* for_exp = make_for_exp(init, cond, move, NULL, scope);
	tlp = (*tl);

	if(tlp->type != token_colon)
		return NULL;
	pop_token(&tlp, tl, tnt);

	parse_compound_statement(tl,
                            tnt,
                            &(for_exp->op.for_loop_exp.body),
                            scope);
    for_exp->op.for_loop_exp.symbols = for_symbols;

    symbol_list_ptr = old_symbol_list_ptr;
	return for_exp;
}

asn* parse_while_loop_exp(token** tl, size_t* tnt){
	token* tlp = (*tl);
	int scope = tlp->level;

	if(tlp->type != token_loop_while)
		return NULL;

	printf("[%zu (%s)] parsing while loop\n", (*tnt), (*tl)->str);
	pop_token(&tlp, tl, tnt);

	asn* condition = parse_expression(tl, tnt);
	asn* while_exp = make_while_exp(condition, NULL, scope);
	tlp = (*tl);

	if(tlp->type != token_colon)
		return NULL;

	pop_token(&tlp, tl, tnt);

    symbol_list* while_symbols = new_symbol_list((size_t) scope + 1);
    symbol_list_attach(&symbol_list_ptr, &while_symbols);

	parse_compound_statement(tl,
                            tnt,
                            &(while_exp->op.while_loop_exp.body),
                            scope);
    while_exp->op.while_loop_exp.symbols = while_symbols;

	return while_exp;
}

