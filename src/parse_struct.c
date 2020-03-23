/*
 * FILENAME: parse_struct.c
 *
 * DESCRIPTION:
 *     Function definitions for parsing struct declarations.
 *
 * AUTHOR: Moritz Röhrich <moritzr@pool.math.tu-berlin.de>
 *
 */

#include "parse.h"

/*
 * <struct-specifier> ::= struct <identifier> : {<declaration>}+
 *                      | union <identifier> : {<declaration>}+
 */
asn* parse_struct_specifier(token** tl, size_t* tnt){
    printf("[%zu (%s)] parsing struct\n", (*tnt), (*tl)->str);
    token* tlp = (*tl);

    int tag = -1;
    if(tlp->type == token_struct)
        tag = struct_tag;
    else if(tlp->type == token_union)
        tag = union_tag;
    else
        return NULL;

    int level = tlp->level;

    if((tlp+1)->type != ident)
        return NULL;

    char* id = (tlp+1)->str;

    if((tlp+2)->type != token_colon)
        return NULL;

    pop_token(&tlp, tl, tnt);
    pop_token(&tlp, tl, tnt);
    pop_token(&tlp, tl, tnt);

    asn* str = make_struct_exp(tag, id, level);
    pv_root* symbol_map = str->op.struct_exp.symbol_map;
    symbol_list* symbols = str->op.struct_exp.symbols;
    str->op.struct_exp.symbols = symbols;

    asn* decl;
    symbol* sym = NULL;
    while(tlp->level == (level + 1)){
        printf("[%zu (%s)] parsing struct member\n", (*tnt), (*tl)->str);
        decl = parse_declaration(tl, tnt, symbol_map);
        tlp = (*tl);

        if(decl->tag == var_def_tag){
            if(pv_search(symbol_map, decl->op.var_def_exp.ident) != NULL){
                parse_error("struct member already declared.", (*tl));
                abort();
            }

            sym = new_symbol(decl->op.var_def_exp.ident,
                            decl->op.var_def_exp.type);
            sym->scope = (size_t) level + 1;
            symbol_list_append(&symbols, &sym);
            delete_symbol(&sym);

            symbol_map_insert(&symbol_map, decl);
            append_exp_list(&(str->op.struct_exp.body), decl);
            if(tag == union_tag)
                str->op.struct_exp.size = max(str->op.struct_exp.size,
                            atomic_type_size[decl->op.var_def_exp.type]);
            else if(tag == struct_tag)
                str->op.struct_exp.size += atomic_type_size[decl->op.var_def_exp.type];

        } else if(decl->tag == struct_tag || decl->tag == union_tag){
            if(pv_search(symbol_map, decl->op.struct_exp.ident) != NULL){
                parse_error("struct member already declared.", (*tl));
                abort();
            }

            sym = new_symbol(decl->op.struct_exp.ident,
                            at_struct);
            sym->scope = (size_t) level + 1;
            symbol_list_append(&symbols, &sym);
            delete_symbol(&sym);

            symbol_map_insert(&symbol_map, decl);
            append_exp_list(&(str->op.struct_exp.body), decl);
            if(tag == union_tag)
                str->op.struct_exp.size = max(str->op.struct_exp.size,
                                            decl->op.struct_exp.size);
            else if(tag == struct_tag)
                str->op.struct_exp.size += decl->op.struct_exp.size;
        } else {
            parse_error("non declaration in struct.", (*tl));
            abort();
        }
    }

    str->op.struct_exp.symbol_map = symbol_map;
    return str;
}

