#include "parse.h"

asn* parse_bin_term(token** tl, size_t* tnt, pv_root* symbol_map){
    asn* term = parse_cast_exp(tl, tnt, symbol_map);
    asn* next_factor = NULL;
    token* tlp = *tl;
    int tok_type;

    while(term != NULL && (tlp->type == token_asterisk ||
                        tlp->type == token_div ||
                        tlp->type == token_mod)){
        printf("parsing term expression. %zu tokens left.\n", *tnt);
        tok_type = tlp->type;
		pop_token(&tlp, tl, tnt);
        next_factor = parse_cast_exp(tl, tnt, symbol_map);
        term = make_binary_exp(term, next_factor, tok_type);
        tlp = *tl;
    }
    return term;
}

asn* parse_bin_sum_exp(token** tl, size_t* tnt, pv_root* symbol_map){
    asn* expr = parse_bin_term(tl, tnt, symbol_map);
    asn* next_term = NULL;
    token* tlp = *tl;
    int tok_type;

    while(expr != NULL && (tlp->type == token_plus ||
                        tlp->type == token_minus)){
        printf("parsing binary expression. %zu tokens left\n", *tnt);
        tok_type = tlp->type;
		pop_token(&tlp, tl, tnt);
        next_term = parse_bin_term(tl, tnt, symbol_map);
        expr = make_binary_exp(expr, next_term, tok_type);
        tlp = *tl;
    }
    return expr;
}

asn* parse_bin_bit_shift_exp(token** tl, size_t* tnt, pv_root* symbol_map){
    asn* sum = parse_bin_sum_exp(tl, tnt, symbol_map);
    asn* next_sum = NULL;
    token* tlp = *tl;
    int tok_type;

    while(sum != NULL && (tlp->type == token_bit_shift_left ||
                        tlp->type == token_bit_shift_right)){
        printf("parsing binary bit shift expr. %zu tokens left\n", *tnt);
        tok_type = tlp->type;
		pop_token(&tlp, tl, tnt);
        next_sum = parse_bin_sum_exp(tl, tnt, symbol_map);
        sum = make_binary_exp(sum, next_sum, tok_type);
        tlp = *tl;
    }
    return sum;
}

asn* parse_bin_rel_exp(token** tl, size_t* tnt, pv_root* symbol_map){
    asn* bit_shift = parse_bin_bit_shift_exp(tl, tnt, symbol_map);
    asn* next_bit_shift = NULL;
    token* tlp = *tl;
    int tok_type;

    while(bit_shift != NULL &&(tlp->type == token_less ||
                            tlp->type == token_less_or_equal ||
                            tlp->type == token_greater ||
                            tlp->type == token_greater_or_equal)){
        printf("parsing binary relation expr. %zu tokens left\n", *tnt);
        tok_type = tlp->type;
		pop_token(&tlp, tl, tnt);
        next_bit_shift = parse_bin_bit_shift_exp(tl, tnt, symbol_map);
        bit_shift = make_binary_exp(bit_shift, next_bit_shift, tok_type);
        tlp = *tl;
    }
    return bit_shift;
}

asn* parse_bin_eql_exp(token** tl, size_t* tnt, pv_root* symbol_map){
    asn* rel = parse_bin_rel_exp(tl, tnt, symbol_map);
    asn* next_rel = NULL;
    token* tlp = *tl;
    int tok_type;

    while(rel != NULL && (tlp->type == token_equal ||
                        tlp->type == token_not_equal)){
        printf("parsing binary euality expr. %zu tokens left\n", *tnt);
        tok_type = tlp->type;
		pop_token(&tlp, tl, tnt);
        next_rel = parse_bin_rel_exp(tl, tnt, symbol_map);
        rel = make_binary_exp(rel, next_rel, tok_type);
        tlp = *tl;
    }
    return rel;
}

asn* parse_bin_bit_and_exp(token** tl, size_t* tnt, pv_root* symbol_map){
    asn* equal = parse_bin_eql_exp(tl, tnt, symbol_map);
    asn* next_equal = NULL;
    token* tlp = *tl;
    int tok_type;

    while(equal != NULL && tlp->type == token_ampersand){
        printf("parsing binary bitwise and expr. %zu tokens left\n", *tnt);
        tok_type = tlp->type;
		pop_token(&tlp, tl, tnt);
        next_equal = parse_bin_eql_exp(tl, tnt, symbol_map);
        equal = make_binary_exp(equal, next_equal, tok_type);
        tlp = *tl;
    }
    return equal;

}

asn* parse_bin_bit_xor_exp(token** tl, size_t* tnt, pv_root* symbol_map){
    asn* bit_and = parse_bin_bit_and_exp(tl, tnt, symbol_map);
    asn* next_bit_and = NULL;
    token* tlp = *tl;
    int tok_type;

    while(bit_and != NULL && tlp->type == token_bit_xor){
        printf("parsing binary bitwise xor and expr. %zu tokens left\n", *tnt);
        tok_type = tlp->type;
		pop_token(&tlp, tl, tnt);
        next_bit_and = parse_bin_bit_and_exp(tl, tnt, symbol_map);
        bit_and = make_binary_exp(bit_and, next_bit_and, tok_type);
        tlp = *tl;
    }
    return bit_and;

}

asn* parse_bin_bit_or_exp(token** tl, size_t* tnt, pv_root* symbol_map){
    asn* bit_xor = parse_bin_bit_xor_exp(tl, tnt, symbol_map);
    asn* next_bit_xor = NULL;
    token* tlp = *tl;
    int tok_type;

    while(bit_xor != NULL && tlp->type == token_bit_or){
        printf("parsing binary bitwise or expr. %zu tokens left\n", *tnt);
        tok_type = tlp->type;
		pop_token(&tlp, tl, tnt);
        next_bit_xor = parse_bin_bit_xor_exp(tl, tnt, symbol_map);
        bit_xor = make_binary_exp(bit_xor, next_bit_xor, tok_type);
        tlp = *tl;
    }
    return bit_xor;

}

asn* parse_bin_log_and_exp(token** tl, size_t* tnt, pv_root* symbol_map){
    asn* bit_or = parse_bin_bit_or_exp(tl, tnt, symbol_map);
    asn* next_bit_or = NULL;
    token* tlp = *tl;
    int tok_type;

    while(bit_or != NULL && tlp->type == token_log_and){
        printf("parsing binary logical and expr. %zu tokens left\n", *tnt);
        tok_type = tlp->type;
		pop_token(&tlp, tl, tnt);
        next_bit_or = parse_bin_bit_or_exp(tl, tnt, symbol_map);
        bit_or = make_binary_exp(bit_or, next_bit_or, tok_type);
        tlp = *tl;
    }
    return bit_or;
}

asn* parse_bin_log_xor_exp(token** tl, size_t* tnt, pv_root* symbol_map){
    asn* log_and = parse_bin_log_and_exp(tl, tnt, symbol_map);
    asn* next_log_and = NULL;
    token* tlp = *tl;
    int tok_type;

    while(log_and != NULL && tlp->type == token_log_xor){
        printf("parsing binary logical xor expr. %zu tokens left\n", *tnt);
        tok_type = tlp->type;
		pop_token(&tlp, tl, tnt);
        next_log_and = parse_bin_log_and_exp(tl, tnt, symbol_map);
        log_and = make_binary_exp(log_and, next_log_and, tok_type);
        tlp = *tl;
    }
    return log_and;
}

asn* parse_bin_log_or_exp(token** tl, size_t* tnt, pv_root* symbol_map){
    asn* log_xor = parse_bin_log_xor_exp(tl, tnt, symbol_map);
    asn* next_log_xor = NULL;
    token* tlp = *tl;
    int tok_type;

    while(log_xor != NULL && tlp->type == token_log_or){
        printf("parsing binary logical or expr. %zu tokens left\n", *tnt);
        tok_type = tlp->type;
		pop_token(&tlp, tl, tnt);
        next_log_xor = parse_bin_log_xor_exp(tl, tnt, symbol_map);
        log_xor = make_binary_exp(log_xor, next_log_xor, tok_type);
        tlp = *tl;
    }
    return log_xor;
}

asn* parse_bin_exp(token** tl, size_t* tnt, pv_root* symbol_map){
    printf("parsing binary expression. %zu, %s\n", (*tnt), (*tl)->str);
    asn* bin_log_or = parse_bin_log_or_exp(tl, tnt, symbol_map);
    return bin_log_or;
}

