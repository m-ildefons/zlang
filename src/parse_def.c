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

asn* parse_fun_def_exp(token** tl, size_t* tnt, pv_root* symbol_map, symbol_list** symbols){
    token* tlp;
    int scope;
    char* id;
    asn* f;
    pv_leaf* leaf;
	unsigned int i;
    atomic_type ty;

    tlp = *tl;

    i = 0;
    switch(tlp->type){
        case type_void_kw:
            if((tlp+1)->type == token_asterisk){
                ty = at_void_ptr;
                tlp++;
                i++;
            } else {
                ty = at_void;
            }
            break;
        case type_int_kw:
            if((tlp+1)->type == token_asterisk){
                ty = at_int_ptr;
                tlp++;
                i++;
            } else {
                ty = at_int;
            }
            break;
        case type_real_kw:
            if((tlp+1)->type == token_asterisk){
                ty = at_real_ptr;
                tlp++;
                i++;
            } else {
                ty = at_real;
            }
            break;
        case type_char_kw:
            if((tlp+1)->type == token_asterisk){
                ty = at_char_ptr;
                tlp++;
                i++;
            } else {
                ty = at_char;
            }
            break;
        default: return NULL;
    }
    tlp++;
    i++;

    if(tlp->type != ident)
        return NULL;
    id = tlp->str;
    scope = tlp->level;
    tlp++;
    i++;

    if(((*tnt) - i) <= 0 || tlp->type != open_p)
        return NULL;
    tlp++;
    i++;

    printf("[%zu (%s)] parsing function\n", (*tnt), (*tl)->str);
	(*tl) = tlp;
	(*tnt) -= i;

    symbol_map = copy_trie(symbol_map);

    symbol_list* symbols_fun = new_symbol_list((size_t) scope + 1);
    symbols_fun->bottom->next = (*symbols)->top;
    if((*symbols)->top != NULL){
        (*symbols)->top->ref_count++;
    }

	asn_list* args = NULL;
	asn* arg = NULL;
    symbol* sym_arg = NULL;
    for(i = 0; tlp->type != close_p && (tlp+1)->type != token_colon; i++){
        arg = parse_declaration(tl, tnt, symbol_map);
        arg->op.var_def_exp.scope += 1;
		if(arg == NULL)
			abort();
		tlp = (*tl);

		symbol_map_insert(&symbol_map, arg);

        sym_arg = new_symbol(arg->op.var_def_exp.ident,
                            arg->op.var_def_exp.type);
        sym_arg->scope = (size_t) arg->op.var_def_exp.scope;
        symbol_list_append(&symbols_fun, &sym_arg);
        delete_symbol(&sym_arg);

		if(i >= 6){
			symbol_map->mem_offset -= 8;
			leaf = pv_search(symbol_map, arg->op.var_def_exp.ident);
			leaf->offset = -(leaf->size) - ((((int) i) - 6) * 8 + 16);
		}

		if(tlp->type == token_comma)
			pop_token(&tlp, tl, tnt);
		append_exp_list(&args, arg);
	}

    if(tlp == NULL || tlp->type != close_p)
        return NULL;

    pop_token(&tlp, tl, tnt);
    if(tlp == NULL || tlp->type != token_colon)
        return NULL;

    pop_token(&tlp, tl, tnt);

    f = make_fun_def_exp(ty, id, args, NULL, scope);
    f->op.fun_def_exp.symbol_map = symbol_map;
    f->op.fun_def_exp.symbols = symbols_fun;
    leaf = new_pv_leaf(id, ty, -1, symbol_map->mem_offset, scope);
    pv_root* osm = symbol_map;
    symbol_map = pv_insert(symbol_map, id, leaf);
    delete_trie(osm);
    symbol_map->scope = scope + 1;

	parse_compound_statement(tl, tnt, &(f->op.fun_def_exp.body), &symbol_map, &symbols_fun, scope);
    f->op.fun_def_exp.symbol_map = symbol_map;
    return f;
}

