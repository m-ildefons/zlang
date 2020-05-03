/*
 * FILENAME: parse_declaration.c
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
 *
 * This function works a bit different, since it can only be determined very
 * late whether it is parsing a declaration (of a global varible or a function)
 * or a function definition. This is because a lot of tokens make up either.
 *
 * So it parses the beginning components like the declaration specifier that is
 * common and passes these on to the following function. Finally, when it
 * notices a colon, it decides that this is actually a definition a parses the
 * definition body with the appropriate function.
 */
asn* parse_external_declaration(token** tl, size_t* tnt){
    printf("[%zu (%s)] parsing external declaration\n", (*tnt), (*tl)->str);
    token* tlp = (*tl);
    asn* exp = NULL;
    type_link* decl_spec = NULL;

    decl_spec = parse_declaration_specifier(tl, tnt);

    if(decl_spec == NULL)
        return NULL;

    exp = parse_declaration(tl, tnt, decl_spec);
    tlp = (*tl);
    if(tlp->type == token_colon)
        exp = parse_function_definition(tl, tnt, exp);

    if(exp != NULL){
        return exp;
    }

    parse_error("invalid token", (*tl));
    return NULL;
}

/*
 * <declaration> ::= {<declaration-specifier>}+ {<init-declarator>}* ;?
 */
asn* parse_declaration(token** tl, size_t* tnt, type_link* decl_spec){
    printf("[%zu (%s)] parsing declaration\n", (*tnt), (*tl)->str);

    token* tlp = (*tl);

    if(decl_spec == NULL)
        return NULL;

    asn* init_decl = parse_init_decl(tl, tnt);
    if(init_decl == NULL)
        return NULL;

    if(init_decl->tag == assign_tag && init_decl->op.assign_exp.lval->tag == var_tag){
        asn* var = init_decl->op.assign_exp.lval;
        type_link_attach(&(var->op.var_exp.sym), decl_spec);
    } else if(init_decl->tag == var_tag){
        type_link_attach(&(init_decl->op.var_exp.sym), decl_spec);
    } else if(init_decl->tag == fun_def_tag){
        type_link_attach(&(init_decl->op.fun_def_exp.sym), decl_spec);
    }

    tlp = (*tl);
    if(tlp != NULL && tlp->type == token_semi_colon)
        pop_token(&tlp, tl, tnt);

    return init_decl;
}

/*
 * <init-declarator> ::= <declarator>
 *                     | <declarator> = <initializer>
 */
asn* parse_init_decl(token** tl, size_t* tnt){
    printf("[%zu (%s)] parsing init declarator\n", (*tnt), (*tl)->str);

    asn* decl = parse_declarator(tl, tnt);
    token* tlp = (*tl);

    if(tlp != NULL && tlp->type == token_assign){
        pop_token(&tlp, tl, tnt);
        asn* val = parse_assign_exp(tl, tnt);
        decl = make_assign_exp(decl, val, token_assign);
    }

    return decl;
}

/*
 * <declarator> ::= {<pointer>}? <direct-declarator>
 */
asn* parse_declarator(token** tl, size_t* tnt){
    printf("[%zu (%s)] parsing declarator\n", (*tnt), (*tl)->str);

    type_link* ptr = parse_pointer(tl, tnt);

    asn* dir_decl = parse_direct_declarator(tl, tnt);

    if(dir_decl->tag == var_tag){
        type_link_attach(&(dir_decl->op.var_exp.sym), ptr);
    } else if(dir_decl->tag == fun_def_tag){
        type_link_attach(&(dir_decl->op.fun_def_exp.sym), ptr);
    }

    return dir_decl;
}

/*
 * <pointer> ::= * {<type-qualifier>}* {<pointer>}?
 */
type_link* parse_pointer(token** tl, size_t* tnt){
    printf("[%zu (%s)] parsing pointer\n", (*tnt), (*tl)->str);
    token* tlp = (*tl);
    type_link* link = NULL;
    while(tlp->type == token_asterisk){
        pop_token(&tlp, tl, tnt);
        declarator* decl = new_declarator();
        decl->type = decl_pointer;

        type_link* l = new_type_link();
        l->cls = cls_decl;
        l->type.decl = decl;
        if(link == NULL){
            link = l;
        } else {
            l->next = link;
            link = l;
        }
//        type_link* qualifier = parse_type_qualifier(tl, tnt);
    }
    return link;
}

/*
 *
 * <declaration-specifier> ::= <storage-class-specifier>
 *                           | <type-specifier>
 *                           | <type-qualifier>
 */
type_link* parse_declaration_specifier(token** tl, size_t* tnt){
    printf("[%zu (%s)] parsing declaration specifier\n", (*tnt), (*tl)->str);
    type_link* scs = parse_storage_class_specifier(tl, tnt);
	if(scs != NULL)
        return scs;

    type_link* ts = parse_type_specifier(tl, tnt);
    if(ts != NULL)
        return ts;

    type_link* tq = parse_type_qualifier(tl, tnt);
    if(tq != NULL)
        return tq;

    return NULL;
}

/*
 * <storage-class-specifier> ::= auto
 *                             | register
 *                             | static
 *                             | extern
 *                             | inline
 */
type_link* parse_storage_class_specifier(token** tl, size_t* tnt){
    printf("[%zu (%s)] parsing storage-class specifier\n", (*tnt), (*tl)->str);
    return NULL;
}

/*
 * <type-specifier> ::= void
 *                    | bool
 *                    | char
 *                    | int
 *                    | real
 *                    | signed
 *                    | unsigned
 *                    | complex
 *                    | string
 *                    | <struct-or-union-specifier>
 *                    | <enum-specifier>
 *                    | <typedef-name>
 */
type_link* parse_type_specifier(token** tl, size_t* tnt){
    printf("[%zu (%s)] parsing type specifier\n", (*tnt), (*tl)->str);

    int type;
    token* tlp = (*tl);
    switch(tlp->type){
        case token_void_kw: type = type_void; break;
        case token_bool_kw: type = type_bool; break;
        case token_char_kw: type = type_char; break;
    	case token_int_kw: type = type_int; break;
        case token_real_kw: type = type_real; break;
        case token_signed_kw: type = type_signed; break;
        case token_unsigned_kw: type = type_unsigned; break;
        case token_complex_kw: type = type_complex; break;
        case token_string_kw: type = type_string; break;
        case token_struct: type = type_struct; break;
        default:
            return NULL;
    }

    pop_token(&tlp, tl, tnt);

    specifier* spec = new_specifier();
    spec->storage_class = sclass_auto;
    spec->output_class = oclass_auto;
    spec->type = type;

    type_link* t = new_type_link();
    t->cls = cls_spec;
    t->type.spec = spec;
    return t;
}

/*
 * <type-qualifier> ::= const
 *                    | volatile
 *                    | restrict
 */
type_link* parse_type_qualifier(token** tl, size_t* tnt){
    printf("[%zu (%s)] parsing type qualifier\n", (*tnt), (*tl)->str);
    return NULL;
}

/*
 * <direct-declarator> ::= <identifier>
 *                       | ( <declarator> )
 *                       | <direct-declarator> [ {<constant-expression>}? ]
 *                       | <direct-declarator> ( <parameter-type-list> )
 *                       | <direct-declarator> ( {<identifier>}* )
 */
asn* parse_direct_declarator(token** tl, size_t* tnt){
    printf("[%zu (%s)] parsing direct declarator\n", (*tnt), (*tl)->str);
    token* tlp = (*tl);
    int scope = tlp->level;

    asn* var = NULL;
    symbol* sym;
    if(tlp->type == ident){
        /* <identifier> */
        sym = new_symbol(tlp->str);
        symbol_list_append(&symbol_list_ptr, &sym);
        var = make_var_exp(sym);
        pop_token(&tlp, tl, tnt);
    } else if(tlp->type == open_p){
        /* ( <declarator> ) */
        pop_token(&tlp, tl, tnt);
        var = parse_declarator(tl, tnt);
        if(tlp->type != close_p){
            parse_error("Unbalanced Parenthesis", (*tl));
            abort();
        }
        pop_token(&tlp, tl, tnt);
    }

    /* <direct-declarator> ( <parameter-type-list> ) */
    while(tlp != NULL && tlp->type == open_p){
        pop_token(&tlp, tl, tnt);

        delete_var_exp(var);
        var = make_fun_def_exp(sym,
                                NULL,
                                NULL,
                                scope);

        declarator* func_decl = new_declarator();
        func_decl->type = decl_function;

        type_link* link = new_type_link();
        link->cls = cls_decl;
        link->type.decl = func_decl;
        type_link_attach(&(sym), link);

        symbol_list* symbols_fun = new_symbol_list((size_t) scope + 1);
        symbol_list_attach(&symbol_list_ptr, &symbols_fun);
        symbol_list* old_symbol_list_ptr = symbol_list_ptr;
        symbol_list_ptr = symbols_fun;
        var->op.fun_def_exp.symbols = symbols_fun;

        asn_list* parameter_list = parse_parameter_type_list(tl, tnt);
        var->op.fun_def_exp.args = parameter_list;
        symbol_list_ptr = old_symbol_list_ptr;

        tlp = (*tl);
        if(tlp->type != close_p){
            parse_error("Unbalanced Parenthesis", (*tl));
            abort();
        }
        pop_token(&tlp, tl, tnt);
    }
    delete_symbol(&sym);
    return var;
}

/*
 * <parameter-type-list> ::= <parameter-list>
 *                         | <parameter-list> , ...
 */
asn_list* parse_parameter_type_list(token** tl, size_t* tnt){
    printf("[%zu (%s)] parsing parameter type list\n", (*tnt), (*tl)->str);
    return parse_parameter_list(tl, tnt);
}

/*
 * <parameter-list> ::= <parameter-declaration>
 *                    | <parameter-list> , <parameter-declaration>
 */
asn_list* parse_parameter_list(token** tl, size_t* tnt){
    printf("[%zu (%s)] parsing parameter list\n", (*tnt), (*tl)->str);
    asn_list* param_list = NULL;

    asn* param = parse_parameter_declaration(tl, tnt);
    while(param != NULL){
        append_exp_list(&param_list, param);
        param = parse_parameter_declaration(tl, tnt);
    }
    return param_list;
}

/*
 * <parameter-declaration> ::= {<declaration-specifier>}+ <declarator>
 *                           | {<declaration-specifier>}+ <abstract-declarator>
 *                           | {<declaration-specifier>}+
 */
asn* parse_parameter_declaration(token** tl, size_t* tnt){
    printf("[%zu (%s)] parsing declaration\n", (*tnt), (*tl)->str);
    type_link* decl_spec = parse_declaration_specifier(tl, tnt);
    if(decl_spec == NULL)
        return NULL;

    asn* decl = parse_declarator(tl, tnt);
    type_link_attach(&(decl->op.var_exp.sym), decl_spec);
    return decl;
}

