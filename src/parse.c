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
    while(*tnt > 0){
        asn* e = parse_external_declaration(tl, tnt, &symbol_map);
        if(e == NULL)
            break;

        append_exp_list(&(prog->op.prog_exp.prog), e);
    }
    prog->op.prog_exp.symbol_map = symbol_map;
    return prog;
}

asn* parse_exp(token** tl, size_t* tnt, pv_root* symbol_map){
    printf("[%zu (%s)] parsing expression\n", (*tnt), (*tl)->str);

    if(*tnt <= 0){
        return NULL;
    }
    asn* expr;
    int level = (*tl)->level;
    expr = parse_ret_exp(tl, tnt, symbol_map);
    if(expr != NULL){
        printf("\033[92mfound\033[39m return expression at level %d.\n", level);
        return expr;
    }
    expr = parse_declaration(tl, tnt, symbol_map);
    if(expr != NULL){
        printf("\033[92mfound\033[39m declaration at level %d\n", level);
        return expr;
    }
    expr = parse_assign_exp(tl, tnt, symbol_map);
    if(expr != NULL){
        printf("\033[92mfound\033[39m assignment at level %d\n", level);
        return expr;
    }
    expr = parse_cond_exp(tl, tnt, symbol_map);
    if(expr != NULL){
        printf("\033[92mfound\033[39m conditional expression at level %d.\n", level);
        return expr;
    }
    expr = parse_for_loop_exp(tl, tnt, symbol_map);
    if(expr != NULL){
        printf("\033[92mfound\033[39m for loop at level %d.\n", level);
        return expr;
    }
    expr = parse_while_loop_exp(tl, tnt, symbol_map);
    if(expr != NULL){
        printf("\033[92mfound\033[39m while loop at level %d.\n", level);
        return expr;
    }
    expr = parse_fun_call_exp(tl, tnt, symbol_map);
    if(expr != NULL){
        printf("\033[92mfound\033[39m function call at level %d.\n", level);
        return expr;
    }
    expr = parse_jump_exp(tl, tnt);
    if(expr != NULL){
        printf("\033[92mfound\033[39m jump at level %d.\n", level);
        return expr;
    }

    parse_error("Invalid Expression", (*tl));
    return NULL;
}

asn* parse_cast_exp(token** tl, size_t* tnt, pv_root* symbol_map){
    printf("[%zu (%s)] parsing cast expr\n", (*tnt), (*tl)->str);
    asn* cast = parse_unary_exp(tl, tnt, symbol_map);
    return cast;
}

asn* parse_postfix_exp(token** tl, size_t* tnt, pv_root* symbol_map){
    printf("[%zu (%s)] parsing postfix expr\n", (*tnt), (*tl)->str);

    token* tlp = (*tl);

    asn* primary_exp = parse_primary_exp(tl, tnt, symbol_map);
    asn* postfix_exp = primary_exp;

    tlp = (*tl);
    if(tlp != NULL && tlp->type == token_inc){
        pop_token(&tlp, tl, tnt);
        postfix_exp = make_unary_exp(at_int, primary_exp, token_inc);
    } else if(tlp != NULL && tlp->type == token_dec){
        pop_token(&tlp, tl, tnt);
        postfix_exp = make_unary_exp(at_int, primary_exp, token_dec);
    }

    if(tlp != NULL && tlp->type == token_semi_colon)
        pop_token(&tlp, tl, tnt);

    printf("found postfix expr.\n");
    return postfix_exp;
}

asn* parse_primary_exp(token** tl, size_t* tnt, pv_root* symbol_map){
    printf("[%zu (%s)] parsing primary expr\n", (*tnt), (*tl)->str);

    token* tlp = *tl;
    int tok_type = tlp->type;
    asn* primary = NULL;

    switch(tok_type){
        case const_int: primary = parse_const_exp(tl, tnt); break;
        case const_real: primary = parse_const_exp(tl, tnt); break;
        case const_char: primary = parse_const_exp(tl, tnt); break;
        case const_string: primary = parse_const_exp(tl, tnt); break;
        case ident:
            if((*tnt) > 1 && (tlp+1)->type == open_p){
		        primary = parse_fun_call_exp(tl, tnt, symbol_map);
            } else {
                primary = parse_var_ref(tl, tnt, symbol_map);
            }
            break;
        case open_p:
		    pop_token(&tlp, tl, tnt);
            primary = parse_assign_exp(tl, tnt, symbol_map);
            tlp = (*tl);
            if(tlp->type != close_p){
                parse_error("Unbalanced Parenthesis", (*tl));
                abort();
            }
		    pop_token(&tlp, tl, tnt);
            if(tlp != NULL && tlp->type == token_semi_colon)
		    	pop_token(&tlp, tl, tnt);
            break;
        case close_p: pop_token(&tlp, tl, tnt); break;
        default:
            return NULL;
    }
    if(primary != NULL){
        printf("found primary expr\n");
    }

    return primary;
}

asn* parse_var_ref(token** tl, size_t* tnt, pv_root* symbol_map){
    token* tlp = *tl;
	asn* var = NULL;

	if(tlp == NULL || tlp->type != ident){
		return NULL;
    }

    printf("[%zu (%s)] parsing var ref expr\n", (*tnt), (*tl)->str);

    pv_leaf* leaf = pv_search(symbol_map, tlp->str);
    if(leaf == NULL){
        parse_error("Reference to undeclared symbol", (*tl));
        return NULL;
    }

	var = make_var_ref_exp(tlp->str);

	pop_token(&tlp, tl, tnt);

	if(tlp != NULL && tlp->type == token_semi_colon)
		pop_token(&tlp, tl, tnt);

    printf("found var ref expr\n");
    return var;
}

void parse_body(token** tl,
            size_t* tnt,
            asn_list** body,
            pv_root** symbol_map,
            int top_level){
    token* tlp = (*tl);
    while((*tnt) > 0 && tlp != NULL && tlp->level > top_level){
        asn* e = parse_exp(tl, tnt, (*symbol_map));
        if(e == NULL)
            break;

        if(e->tag == var_def_tag){
            if(pv_search((*symbol_map), e->op.var_def_exp.ident) != NULL)
                abort();
            symbol_map_insert(symbol_map, e);
        }

        append_exp_list(body, e);

        tlp = (*tl);
    }
}

void symbol_map_insert(pv_root** symbol_map, asn* var_exp){
    char* _id = var_exp->op.var_def_exp.ident;
    atomic_type _ty = var_exp->op.var_def_exp.type;
    int _sz = atomic_type_size[_ty];
    int offset = (*symbol_map)->mem_offset;
    int scope = var_exp->op.var_def_exp.scope;

    pv_leaf* leaf = new_pv_leaf(_id, _ty, _sz, offset, scope);

    pv_root* osm = (*symbol_map);
    (*symbol_map) = pv_insert((*symbol_map), _id, leaf);
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

