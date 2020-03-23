/*
 * FILENAME: parse.c
 *
 * DESCRIPTION:
 *     Implementation of general parsing functions. These are general
 *     purpose functions for generating the abstract syntax tree.
 *
 * AUTHOR: Moritz Röhrich <moritzr@pool.math.tu-berlin.de>
 *
 */

#include "parse.h"

/*
 * <translation-unit> ::= {<external-declaration>}*
 */
asn* parse_translation_unit(token** tl, size_t* tnt, const char* filename){
    printf("[%zu (%s)] parsing translation unit\n", (*tnt), (*tl)->str);

    asn* prog = make_prog_exp(filename, NULL);
    pv_root* symbol_map = prog->op.prog_exp.symbol_map;

    symbol_list* symbols = prog->op.prog_exp.symbols;

    while(*tnt > 0){
        asn* e = parse_external_declaration(tl, tnt, &symbol_map, &symbols);
        if(e == NULL)
            break;

        append_exp_list(&(prog->op.prog_exp.prog), e);
    }
    prog->op.prog_exp.symbol_map = symbol_map;
    return prog;
}

asn* parse_var_ref(token** tl, size_t* tnt, pv_root* symbol_map){
    token* tlp = *tl;
	asn* var = NULL;

	if(tlp == NULL || tlp->type != ident){
		return NULL;
    }

    printf("[%zu (%s)] parsing var ref expr\n", (*tnt), (*tl)->str);

    pv_leaf* leaf = pv_search(symbol_map, tlp->str);
    //if(leaf == NULL){
    //    parse_error("Reference to undeclared symbol", (*tl));
	//	pv_pretty_print(symbol_map);
    //    return NULL;
    //}

	var = make_var_ref_exp(tlp->str);

	pop_token(&tlp, tl, tnt);

    printf("found var ref expr\n");
    return var;
}

void symbol_map_insert(pv_root** symbol_map, asn* var_exp){
    char* id;
    int sz;
    int offset;
    int scope;
    atomic_type ty;

    pv_leaf* leaf;
    pv_root* osm;
    if(var_exp->tag == var_def_tag){
        id = var_exp->op.var_def_exp.ident;
        ty = var_exp->op.var_def_exp.type;
        sz = atomic_type_size[ty];
        offset = (*symbol_map)->mem_offset;
        scope = var_exp->op.var_def_exp.scope;
    } else if(var_exp->tag == struct_tag || var_exp->tag == union_tag){
        id = var_exp->op.struct_exp.ident;
        ty = at_struct;
        sz = var_exp->op.struct_exp.size;
        offset = 0;
        scope = var_exp->op.struct_exp.scope;
    }

    leaf = new_pv_leaf(id, ty, sz, offset, scope);
    osm = (*symbol_map);
    (*symbol_map) = pv_insert((*symbol_map), id, leaf);
    delete_trie(osm);
}

pv_root* symbol_map_copy(pv_root* symbol_map){
    pv_root* new_symbol_map = copy_trie(symbol_map);
	return new_symbol_map;
}

void pop_token(token** tlp, token** tl, size_t* tnt){
	(*tlp)++;
	(*tnt)--;
	if((*tnt) == 0)
		(*tlp) = NULL;
	(*tl) = (*tlp);
}

void parse_error(const char* err, token* tok){
    fprintf(stderr,
            "Line %d: \033[91mError\033[39m: %s: %s\n",
            tok->line,
            err,
            tok->str);
}

