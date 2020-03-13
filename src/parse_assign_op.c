/*
 * FILENAME: parse_assign.c
 *
 * DESCRIPTION:
 *     Implementation of functions for parsing assignment expressions.
 *
 * AUTHOR: Moritz Röhrich <moritzr@pool.math.tu-berlin.de>
 *
 */

#include "parse.h"

/*
 * <assignment-expression> ::= <conditional-expression>
 *                           | <unary-expression> <assignment-operator> <assignment-expression>
 *
 * <assignment-operator> ::= =
 *                         | *=
 *                         | /=
 *                         | %=
 *                         | +=
 *                         | -=
 *                         | <<=
 *                         | >>=
 *                         | &=
 *                         | ^=
 *                         | |=
 */

asn* parse_assign_exp(token** tl, size_t* tnt, pv_root* symbol_map){
    printf("[%zu (%s)] parsing assignment\n", (*tnt), (*tl)->str);
    token* tlp = (*tl);
    asn* lhs = NULL;

    lhs = parse_bin_exp(tl, tnt, symbol_map);
    tlp = (*tl);

	if((*tnt) < 1){
		return lhs;
	}

	int type = tlp->type;
    switch(tlp->type){
        case token_assign: break;
        case token_add_assign: break;
        case token_sub_assign: break;
        case token_mul_assign: break;
        case token_div_assign: break;
        case token_mod_assign: break;
        case token_and_assign: break;
        case token_xor_assign: break;
        case token_or_assign: break;
        case token_bsl_assign: break;
        case token_bsr_assign: break;
        default:
			return lhs;
    }

    pop_token(&tlp, tl, tnt);

    if(*tnt < 1)
		abort();

	if(lhs->tag == var_ref_tag &&
		pv_search(symbol_map, lhs->op.var_ref_exp.ident) == NULL){
		fprintf(stderr, "\033[91mError\033[39m: Reference to undeclared variable.\n");
        abort();
	}

    asn* rhs = parse_assign_exp(tl, tnt, symbol_map);
    asn* as = make_assign_exp(lhs, rhs, type);

    return as;
}

