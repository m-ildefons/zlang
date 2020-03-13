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

asn* parse_for_loop_exp(token** tl, size_t* tnt, pv_root* symbol_map){
	token* tlp = (*tl);
	int scope = tlp->level;

	if(tlp->type != token_loop_for)
		return NULL;

	printf("parsing for loop expr. %lu tokens left.\n", (*tnt));
	pop_token(&tlp, tl, tnt);

	symbol_map = symbol_map_copy(symbol_map);
	symbol_map->scope = scope + 1;

	asn* init = parse_exp(tl, tnt, symbol_map);
	if(init->tag == var_def_tag)
		symbol_map_insert(&symbol_map, init);
	asn* cond = parse_exp(tl, tnt, symbol_map);
	asn* move = parse_exp(tl, tnt, symbol_map);

	asn* for_exp = make_for_exp(init, cond, move, NULL, scope);
	tlp = (*tl);

	if(tlp->type != token_colon)
		return NULL;
	pop_token(&tlp, tl, tnt);

	parse_compound_statement(tl,
                            tnt,
                            &(for_exp->op.for_loop_exp.body),
                            &symbol_map,
                            scope);
	for_exp->op.for_loop_exp.symbol_map = symbol_map;

	return for_exp;
}

asn* parse_while_loop_exp(token** tl, size_t* tnt, pv_root* symbol_map){
	token* tlp = (*tl);
	int scope = tlp->level;

	if(tlp->type != token_loop_while)
		return NULL;

	printf("parsing while loop expr. %lu tokens left\n", (*tnt));
	pop_token(&tlp, tl, tnt);

	asn* condition = parse_bin_exp(tl, tnt, symbol_map);
	asn* while_exp = make_while_exp(condition, NULL, scope);
	tlp = (*tl);

	if(tlp->type != token_colon)
		return NULL;

	pop_token(&tlp, tl, tnt);

	symbol_map = symbol_map_copy(symbol_map);
	symbol_map->scope = scope + 1;

	parse_compound_statement(tl,
                            tnt,
                            &(while_exp->op.while_loop_exp.body),
                            &symbol_map,
                            scope);
	while_exp->op.while_loop_exp.symbol_map = symbol_map;

	return while_exp;
}

