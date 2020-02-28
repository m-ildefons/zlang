#include "parse.h"

asn* parse_fun_def_exp(token** tl, size_t* tnt, pv_root* symbol_map){
    token* tlp;
    int level;
    int scope;
    char* id;
    asn* r;
    asn* f;
    asn* e;
    pv_leaf* leaf;
	int i;

    tlp = *tl;
    level = tlp->level;
    if(tlp->type != type_int_kw)
        return NULL;
    tlp++;
    if(tlp->type != ident)
        return NULL;
    id = tlp->str;
    scope = tlp->level;
    tlp++;
    if(tlp->type != open_p)
        return NULL;
    tlp++;

    printf("parsing function expr. %zu tokens\n", (*tnt));
	(*tl) = tlp;
	(*tnt) -= 3;

	asn_list* args = NULL;
	asn* arg = NULL;
	//for(i = 0; tlp->type == type_int_kw && (tlp+1)->type == ident; i++){
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
			leaf->offset = -(leaf->size) - ((i - 6) * 8 + 16);
		}

		if(tlp->type == token_comma)
			pop_token(&tlp, tl, tnt);
		append_exp_list(&args, arg);
	}

    if(tlp->type != close_p)
        return NULL;
    pop_token(&tlp, tl, tnt);
    if(tlp->type != token_colon)
        return NULL;
    pop_token(&tlp, tl, tnt);

    r = make_int_exp(0);
    f = make_fun_def_exp(r, id, args, NULL, scope);
    f->op.fun_def_exp.symbol_map = symbol_map;
    leaf = new_pv_leaf(id, at_func, -1, symbol_map->mem_offset, scope);
    symbol_map = pv_insert(symbol_map, id, leaf);
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

