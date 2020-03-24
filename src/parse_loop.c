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

asn* parse_for_loop_exp(token** tl, size_t* tnt, pv_root* symbol_map, symbol_list** symbols){
	token* tlp = (*tl);
	int scope = tlp->level;

	if(tlp->type != token_loop_for)
		return NULL;

	printf("[%zu (%s)] parsing for loop\n", (*tnt), (*tl)->str);
	pop_token(&tlp, tl, tnt);

	symbol_map = symbol_map_copy(symbol_map);
	symbol_map->scope = scope + 1;

    symbol_list* for_symbols = new_symbol_list((size_t) scope + 1);
    symbol_list_attach(symbols, &for_symbols);

	asn* init = parse_exp(tl, tnt, symbol_map);
	if(init != NULL && init->tag == var_def_tag){
		symbol_map_insert(&symbol_map, init);
        symbol* init_sym = new_symbol(init->op.var_def_exp.ident,
                                    init->op.var_def_exp.type);
        init_sym->scope = (size_t) init->op.var_def_exp.scope;
        symbol_list_append(&for_symbols, &init_sym);
        delete_symbol(&init_sym);
    }

	tlp = (*tl);
	if(tlp != NULL && tlp->type == token_semi_colon)
		pop_token(&tlp, tl, tnt);

	asn* cond = parse_expression(tl, tnt, symbol_map);

	tlp = (*tl);
	if(tlp != NULL && tlp->type == token_semi_colon)
		pop_token(&tlp, tl, tnt);

	asn* move = parse_expression(tl, tnt, symbol_map);

	asn* for_exp = make_for_exp(init, cond, move, NULL, scope);
	tlp = (*tl);

	if(tlp->type != token_colon)
		return NULL;
	pop_token(&tlp, tl, tnt);

	parse_compound_statement(tl,
                            tnt,
                            &(for_exp->op.for_loop_exp.body),
                            &symbol_map,
                            &for_symbols,
                            scope);
	for_exp->op.for_loop_exp.symbol_map = symbol_map;
    for_exp->op.for_loop_exp.symbols = for_symbols;

	return for_exp;
}

asn* parse_while_loop_exp(token** tl, size_t* tnt, pv_root* symbol_map, symbol_list** symbols){
	token* tlp = (*tl);
	int scope = tlp->level;

	if(tlp->type != token_loop_while)
		return NULL;

	printf("[%zu (%s)] parsing while loop\n", (*tnt), (*tl)->str);
	pop_token(&tlp, tl, tnt);

	asn* condition = parse_expression(tl, tnt, symbol_map);
	asn* while_exp = make_while_exp(condition, NULL, scope);
	tlp = (*tl);

	if(tlp->type != token_colon)
		return NULL;

	pop_token(&tlp, tl, tnt);

	symbol_map = symbol_map_copy(symbol_map);
	symbol_map->scope = scope + 1;

    symbol_list* while_symbols = new_symbol_list((size_t) scope + 1);
    symbol_list_attach(symbols, &while_symbols);

	parse_compound_statement(tl,
                            tnt,
                            &(while_exp->op.while_loop_exp.body),
                            &symbol_map,
                            &while_symbols,
                            scope);
	while_exp->op.while_loop_exp.symbol_map = symbol_map;
    while_exp->op.while_loop_exp.symbols = while_symbols;

	return while_exp;
}

