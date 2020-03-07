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

asn* parse_fun_def_exp(token** tl, size_t* tnt, pv_root* symbol_map){
    token* tlp;
    int level;
    int scope;
    char* id;
    asn* f;
    asn* e;
    pv_leaf* leaf;
	unsigned int i;
    atomic_type ty;

    tlp = *tl;
    level = tlp->level;

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

	asn_list* args = NULL;
	asn* arg = NULL;
    for(i = 0; tlp->type != close_p && (tlp+1)->type != token_colon; i++){
        arg = parse_declaration(tl, tnt, symbol_map);
        arg->op.var_def_exp.scope += 1;
		if(arg == NULL)
			abort();
		tlp = (*tl);

		symbol_map_insert(&symbol_map, arg);
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
    leaf = new_pv_leaf(id, ty, -1, symbol_map->mem_offset, scope);
    pv_root* osm = symbol_map;
    symbol_map = pv_insert(symbol_map, id, leaf);
    delete_trie(osm);
    symbol_map->scope = scope + 1;

    while(*tnt > 0 && tlp->level > level){
        e = parse_exp(tl, tnt, symbol_map);
        if(e == NULL)
            break;

        if(e->tag == var_def_tag){
            if(pv_search(symbol_map, e->op.var_def_exp.ident) != NULL)
                abort();
            symbol_map_insert(&symbol_map, e);
        }
        append_exp_list(&(f->op.fun_def_exp.body), e);
        if(*tnt > 0)
            tlp = *tl;
    }
    f->op.fun_def_exp.symbol_map = symbol_map;
    return f;
}

