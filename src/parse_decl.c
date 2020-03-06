/*
 * FILENAME: parse_decl.c
 *
 * DESCRIPTION:
 *     Implementation of functions for parsing declarations.
 *
 * AUTHOR: Moritz Röhrich <moritzr@pool.math.tu-berlin.de>
 *
 */


#include "parse.h"

/*
 * <external-declaration> ::= <function-definition>
 *                          | <declaration>
 */
asn* parse_external_declaration(token** tl, size_t* tnt, pv_root** symbol_map){
    printf("[%zu (%s)] parsing external declaration\n", (*tnt), (*tl)->str);

    asn* exp;

    exp = parse_fun_def_exp(tl, tnt, (*symbol_map));
    if(exp != NULL){
        if(pv_search((*symbol_map), exp->op.fun_def_exp.ident) != NULL){
            parse_error("Error, symbol already declared.", (*tl));
            abort();
        }
        pv_leaf* l = new_pv_leaf(exp->op.fun_def_exp.ident,
                            exp->op.fun_def_exp.type, -1, 0, 0);
        pv_root* osm =(*symbol_map);
        (*symbol_map)= pv_insert((*symbol_map), l->ident, l);
        delete_trie(osm);
        return exp;
    }

    exp = parse_declaration(tl, tnt,(*symbol_map));
    if(exp != NULL){
        if(pv_search((*symbol_map), exp->op.var_def_exp.ident) != NULL){
            parse_error("Error, symbol already declared.", (*tl));
            abort();
        }
        symbol_map_insert(symbol_map, exp);
        (*symbol_map)->mem_offset -= 8;
        return exp;
    }

    parse_error("Error, invalid token", (*tl));
    return NULL;
}

/*
 * <declaration> ::= {<declaration-specifier>}+ {<init-declarator>}* ;?
 */
asn* parse_declaration(token** tl, size_t* tnt, pv_root* symbol_map){
    printf("[%zu (%s)] parsing declaration\n", (*tnt), (*tl)->str);

    token* tlp = (*tl);
    atomic_type ty;

    switch(tlp->type){
        case type_void_kw:
            if((tlp+1)->type == token_asterisk){
                ty = at_void_ptr;
                pop_token(&tlp, tl, tnt);
            } else {
                ty = at_void;
            }
            break;
        case type_int_kw:
            if((tlp+1)->type == token_asterisk){
                ty = at_int_ptr;
                pop_token(&tlp, tl, tnt);
            } else {
                ty = at_int;
            }
            break;
        case type_real_kw:
            if((tlp+1)->type == token_asterisk){
                ty = at_real_ptr;
                pop_token(&tlp, tl, tnt);
            } else {
                ty = at_real;
            }
            break;
        case type_char_kw:
            if((tlp+1)->type == token_asterisk){
                ty = at_char_ptr;
                pop_token(&tlp, tl, tnt);
            } else {
                ty = at_char;
            }
            break;
        default:
            return NULL;
    }

    pop_token(&tlp, tl, tnt);
    asn* decl = parse_init_decl(tl, tnt, symbol_map, ty);

    return decl;
}

/*
 * <init-declarator> ::= <declarator>
 *                     | <declarator> = <initializer>
 */
asn* parse_init_decl(token** tl, size_t* tnt, pv_root* symbol_map, atomic_type ty){
    printf("parsing init declarator. %zu, %s\n", (*tnt), (*tl)->str);

    token* tlp = (*tl);
    int scope = tlp->level;

    if(tlp->type != ident){
        return NULL;
    }

    char* id = tlp->str;
    asn* var = make_var_def_exp(ty, id, scope);
    pop_token(&tlp, tl, tnt);

    if(tlp != NULL && tlp->type == token_semi_colon){
        pop_token(&tlp, tl, tnt);
    } else if(tlp != NULL && tlp->type == token_assign){
        pop_token(&tlp, tl, tnt);
        asn* val = parse_assign_exp(tl, tnt, symbol_map);
        var->op.var_def_exp.val = val;
    }

    return var;
}


