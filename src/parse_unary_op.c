#include "parse.h"

asn* parse_unary_exp(token** tl, size_t* tnt, pv_root* symbol_map){
    printf("parsing unary expr. %lu tokens left\n", *tnt);

    token* tlp = *tl;
    int tok_type = tlp->type;

    asn* inner = NULL;
    if(tok_type == token_minus ||
        tok_type == not_kw ||
        tok_type == complement ||
        tok_type == token_ampersand ||
        tok_type == token_asterisk){

        pop_token(&tlp, tl, tnt);
        inner = parse_cast_exp(tl, tnt, symbol_map);
    } else if(tok_type == token_inc || tok_type == token_dec){
        pop_token(&tlp, tl, tnt);
        inner = parse_unary_exp(tl, tnt, symbol_map);
    } else {
        asn* postfix = parse_postfix_exp(tl, tnt, symbol_map);
        if(postfix != NULL)
            return postfix;

        return NULL;
    }

    if(inner == NULL){
        return NULL;
    }

    printf("found unary expr %d\n", tok_type);
    asn* ue = make_unary_exp(inner, tok_type);

    return ue;
}

