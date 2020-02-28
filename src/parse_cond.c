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

    pv_root* new_symbol_map = new_trie();
    new_symbol_map->trie = symbol_map->trie;
    new_symbol_map->size = symbol_map->size;
    new_symbol_map->depth = symbol_map->depth;
    new_symbol_map->mem_offset = symbol_map->mem_offset;
    new_symbol_map->key_list = symbol_map->key_list;
    new_symbol_map->scope = scope + 1;
    symbol_map = new_symbol_map;

    parse_body(tl, tnt, &(cond_exp->op.cond_exp.if_body), &symbol_map, scope);
    cond_exp->op.cond_exp.if_symbol_map = symbol_map;

	tlp = (*tl);
	if(tlp->type == token_cond_else && tlp->level == scope){
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

