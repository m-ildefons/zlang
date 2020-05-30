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
asn* parse_struct_specifier(token** tl, size_t* tnt, asn* exp){
    printf("[%zu (%s)] parsing struct \'%s\'\n",
		(*tnt),
		(*tl)->str,
		exp->op.var_exp.sym->ident);
    token* tlp = (*tl);
    int level = tlp->level;

	char* id = exp->op.var_exp.sym->ident;

	delete_exp(exp);
	if(tlp->type == token_colon)
		pop_token(&tlp, tl, tnt);

    asn* str = make_struct_exp(struct_tag, id, level);
    symbol_list* old_symbol_list_ptr = symbol_list_ptr;
    symbol_list_ptr = str->op.struct_exp.symbols;

    asn* decl;
    while(tlp->level == (level + 1)){
        printf("[%zu (%s)] parsing struct member of %s\n", (*tnt), (*tl)->str, id);
        type_link* decl_spec = parse_declaration_specifier(tl, tnt);
        decl = parse_declaration(tl, tnt, decl_spec);
        tlp = (*tl);

		if(tlp->type == token_colon){
			decl = parse_struct_specifier(tl, tnt, decl);
        	tlp = (*tl);
		}

		append_exp_list(&(str->op.struct_exp.body), decl);
    }

	symbol_list_ptr = old_symbol_list_ptr;
    return str;
}

