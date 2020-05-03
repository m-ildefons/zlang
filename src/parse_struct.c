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
 */
asn* parse_struct_specifier(token** tl, size_t* tnt){
    printf("[%zu (%s)] parsing struct\n", (*tnt), (*tl)->str);
    token* tlp = (*tl);
    int level = tlp->level;

    int tag = -1;
    if(tlp->type == token_struct)
        tag = struct_tag;
    else if(tlp->type == token_union)
        tag = union_tag;
    else
        return NULL;


    if((tlp+1)->type != ident)
        return NULL;

    char* id = (tlp+1)->str;

    if((tlp+2)->type != token_colon)
        return NULL;

    pop_token(&tlp, tl, tnt);
    pop_token(&tlp, tl, tnt);
    pop_token(&tlp, tl, tnt);

    asn* str = make_struct_exp(tag, id, level);
    symbol_list* symbols = str->op.struct_exp.symbols;
    str->op.struct_exp.symbols = symbols;

    asn* decl;
    while(tlp->level == (level + 1)){
        printf("[%zu (%s)] parsing struct member\n", (*tnt), (*tl)->str);
        type_link* decl_spec = parse_declaration_specifier(tl, tnt);
        decl = parse_declaration(tl, tnt, decl_spec);
        tlp = (*tl);

        if(decl->tag == var_tag){


            append_exp_list(&(str->op.struct_exp.body), decl);
        } else if(decl->tag == struct_tag || decl->tag == union_tag){


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

    return str;
}

