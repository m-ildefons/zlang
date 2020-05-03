/*
 * FILENAME: parse_binary_op.c
 *
 * DESCRIPTION:
 *     Implementation of functions for parsing binary operation expressions.
 *
 * AUTHOR: Moritz Röhrich <moritzr@pool.math.tu-berlin.de>
 *
 */

#include "parse.h"

/*
 * <multiplicative-expression> ::= <cast-expression>
 *                               | <multiplicative-expression> * <cast-expression>
 *                               | <multiplicative-expression> / <cast-expression>
 *                               | <multiplicative-expression> % <cast-expression>
 */
asn* parse_bin_term(token** tl, size_t* tnt){
    asn* term = parse_cast_exp(tl, tnt);
    asn* next_factor = NULL;
    token* tlp = *tl;
    int tok_type;

    while(term != NULL && tlp != NULL &&
			(tlp->type == token_asterisk ||
			tlp->type == token_div ||
			tlp->type == token_mod)){
        printf("[%zu (%s)] parsing term expr\n", (*tnt), (*tl)->str);
        tok_type = tlp->type;
		pop_token(&tlp, tl, tnt);
        next_factor = parse_cast_exp(tl, tnt);
        term = make_binary_exp(term,
                            next_factor,
                            tok_type);
        tlp = *tl;
    }
    return term;
}

/*
 * <additive-expression> ::= <multiplicative-expression>
 *                         | <additive-expression> + <multiplicative-expression>
 *                         | <additive-expression> - <multiplicative-expression>
 */
asn* parse_bin_sum_exp(token** tl, size_t* tnt){
    asn* expr = parse_bin_term(tl, tnt);
    asn* next_term = NULL;
    token* tlp = *tl;
    int tok_type;


    while(expr != NULL && tlp != NULL &&
			(tlp->type == token_plus ||
			tlp->type == token_minus)){
        printf("[%zu (%s)] parsing sum expr\n", (*tnt), (*tl)->str);
        tok_type = tlp->type;
		pop_token(&tlp, tl, tnt);
        next_term = parse_bin_term(tl, tnt);
        expr = make_binary_exp(expr,
                            next_term,
                            tok_type);
        tlp = *tl;
    }
    return expr;
}

/*
 * <shift-expression> ::= <additive-expression>
 *                      | <shift-expression> << <additive-expression>
 *                      | <shift-expression> >> <additive-expression>
 */
asn* parse_bin_bit_shift_exp(token** tl, size_t* tnt){
    asn* sum = parse_bin_sum_exp(tl, tnt);
    asn* next_sum = NULL;
    token* tlp = *tl;
    int tok_type;

    while(sum != NULL && tlp != NULL &&
			(tlp->type == token_bit_shift_left ||
            tlp->type == token_bit_shift_right)){
        printf("[%zu (%s)] parsing bit shift expr\n", (*tnt), (*tl)->str);
        tok_type = tlp->type;
		pop_token(&tlp, tl, tnt);
        next_sum = parse_bin_sum_exp(tl, tnt);
        sum = make_binary_exp(sum, next_sum, tok_type);
        tlp = *tl;
    }
    return sum;
}

/*
 * <relational-expression> ::= <shift-expression>
 *                           | <relational-expression> < <shift-expression>
 *                           | <relational-expression> > <shift-expression>
 *                           | <relational-expression> <= <shift-expression>
 *                           | <relational-expression> >= <shift-expression>
 */
asn* parse_bin_rel_exp(token** tl, size_t* tnt){
    asn* bit_shift = parse_bin_bit_shift_exp(tl, tnt);
    asn* next_bit_shift = NULL;
    token* tlp = *tl;
    int tok_type;

    while(bit_shift != NULL && tlp != NULL &&
			(tlp->type == token_less ||
            tlp->type == token_less_or_equal ||
            tlp->type == token_greater ||
            tlp->type == token_greater_or_equal)){
        printf("[%zu (%s)] parsing relation expr\n", (*tnt), (*tl)->str);
        tok_type = tlp->type;
		pop_token(&tlp, tl, tnt);
        next_bit_shift = parse_bin_bit_shift_exp(tl, tnt);
        bit_shift = make_binary_exp(bit_shift, next_bit_shift, tok_type);
        tlp = *tl;
    }
    return bit_shift;
}

/*
 * <equality-expression> ::= <relational-expression>
 *                         | <equality-expression> == <relational-expression>
 *                         | <equality-expression> != <relational-expression>
 */
asn* parse_bin_eql_exp(token** tl, size_t* tnt){
    asn* rel = parse_bin_rel_exp(tl, tnt);
    asn* next_rel = NULL;
    token* tlp = *tl;
    int tok_type;

    while(rel != NULL && tlp != NULL &&
			(tlp->type == token_equal ||
            tlp->type == token_not_equal)){
        printf("[%zu (%s)] parsing equality expr\n", (*tnt), (*tl)->str);
        tok_type = tlp->type;
		pop_token(&tlp, tl, tnt);
        next_rel = parse_bin_rel_exp(tl, tnt);
        rel = make_binary_exp(rel, next_rel, tok_type);
        tlp = *tl;
    }
    return rel;
}

/*
 * <and-expression> ::= <equality-expression>
 *                    | <and-expression> & <equality-expression>
 */
asn* parse_bin_bit_and_exp(token** tl, size_t* tnt){
    asn* equal = parse_bin_eql_exp(tl, tnt);
    asn* next_equal = NULL;
    token* tlp = *tl;
    int tok_type;

    while(equal != NULL && tlp != NULL && tlp->type == token_ampersand){
        printf("[%zu (%s)] parsing bitwise and expr\n", (*tnt), (*tl)->str);
        tok_type = tlp->type;
		pop_token(&tlp, tl, tnt);
        next_equal = parse_bin_eql_exp(tl, tnt);
        equal = make_binary_exp(equal, next_equal, tok_type);
        tlp = *tl;
    }
    return equal;

}

/*
 * <exclusive-or-expression> ::= <and-expression>
 *                             | <exclusive-or-expression> ^ <and-expression>
 */
asn* parse_bin_bit_xor_exp(token** tl, size_t* tnt){
    asn* bit_and = parse_bin_bit_and_exp(tl, tnt);
    asn* next_bit_and = NULL;
    token* tlp = *tl;
    int tok_type;

    while(bit_and != NULL && tlp != NULL && tlp->type == token_bit_xor){
        printf("[%zu (%s)] parsing bitwise xor expr\n", (*tnt), (*tl)->str);
        tok_type = tlp->type;
		pop_token(&tlp, tl, tnt);
        next_bit_and = parse_bin_bit_and_exp(tl, tnt);
        bit_and = make_binary_exp(bit_and, next_bit_and, tok_type);
        tlp = *tl;
    }
    return bit_and;

}

/*
 * <inclusive-or-expression> ::= <exclusive-or-expression>
 *                             | <inclusive-or-expression> | <exclusive-or-expression>
 */
asn* parse_bin_bit_or_exp(token** tl, size_t* tnt){
    asn* bit_xor = parse_bin_bit_xor_exp(tl, tnt);
    asn* next_bit_xor = NULL;
    token* tlp = *tl;
    int tok_type;

    while(bit_xor != NULL && tlp != NULL && tlp->type == token_bit_or){
        printf("[%zu (%s)] parsing bitwise or expr\n", (*tnt), (*tl)->str);
        tok_type = tlp->type;
		pop_token(&tlp, tl, tnt);
        next_bit_xor = parse_bin_bit_xor_exp(tl, tnt);
        bit_xor = make_binary_exp(bit_xor, next_bit_xor, tok_type);
        tlp = *tl;
    }
    return bit_xor;

}

/*
 * <logical-and-expression> ::= <inclusive-or-expression>
 *                            | <logical-and-expression> && <inclusive-or-expression>
 *                            | <logical-and-expression> and <inclusive-or-expression>
 */
asn* parse_bin_log_and_exp(token** tl, size_t* tnt){
    asn* bit_or = parse_bin_bit_or_exp(tl, tnt);
    asn* next_bit_or = NULL;
    token* tlp = *tl;
    int tok_type;

    while(bit_or != NULL && tlp != NULL && tlp->type == token_log_and){
        printf("[%zu (%s)] parsing logical and expr\n", (*tnt), (*tl)->str);
        tok_type = tlp->type;
		pop_token(&tlp, tl, tnt);
        next_bit_or = parse_bin_bit_or_exp(tl, tnt);
        bit_or = make_binary_exp(bit_or, next_bit_or, tok_type);
        tlp = *tl;
    }
    return bit_or;
}

/*
 * <logical-xor-expression> ::= <logical-and-expression>
 *                           | <logical-xor-expression> >< <logical-and-expression>
 *                           | <logical-xor-expression> xor <logical-and-expression>
 */
asn* parse_bin_log_xor_exp(token** tl, size_t* tnt){
    asn* log_and = parse_bin_log_and_exp(tl, tnt);
    asn* next_log_and = NULL;
    token* tlp = *tl;
    int tok_type;

    while(log_and != NULL && tlp != NULL && tlp->type == token_log_xor){
        printf("[%zu (%s)] parsing logical xor expr\n", (*tnt), (*tl)->str);
        tok_type = tlp->type;
		pop_token(&tlp, tl, tnt);
        next_log_and = parse_bin_log_and_exp(tl, tnt);
        log_and = make_binary_exp(log_and, next_log_and, tok_type);
        tlp = *tl;
    }
    return log_and;
}

/*
 * <logical-or-expression> ::= <logical-xor-expression>
 *                           | <logical-or-expression> || <logical-xor-expression>
 *                           | <logical-or-expression> or <logical-xor-expression>
 */
asn* parse_bin_log_or_exp(token** tl, size_t* tnt){
    asn* log_xor = parse_bin_log_xor_exp(tl, tnt);
    asn* next_log_xor = NULL;
    token* tlp = *tl;
    int tok_type;

    while(log_xor != NULL && tlp != NULL && tlp->type == token_log_or){
        printf("[%zu (%s)] parsing logical or expr\n", (*tnt), (*tl)->str);
        tok_type = tlp->type;
		pop_token(&tlp, tl, tnt);
        next_log_xor = parse_bin_log_xor_exp(tl, tnt);
        log_xor = make_binary_exp(log_xor, next_log_xor, tok_type);
        tlp = *tl;
    }
    return log_xor;
}

asn* parse_bin_exp(token** tl, size_t* tnt){
    printf("[%zu (%s)] parsing binary expression\n", (*tnt), (*tl)->str);
    asn* bin_log_or = parse_bin_log_or_exp(tl, tnt);
    return bin_log_or;
}

