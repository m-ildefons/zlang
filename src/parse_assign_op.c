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

asn* parse_assign_exp(token** tl, size_t* tnt){
    printf("[%zu (%s)] parsing assignment\n", (*tnt), (*tl)->str);
    token* tlp = (*tl);
    asn* lhs = NULL;

    lhs = parse_bin_exp(tl, tnt);
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

    asn* rhs = parse_assign_exp(tl, tnt);
    asn* as = make_assign_exp(lhs, rhs, type);

    return as;
}

