/*
 * FILENAME: parse_const.c
 *
 * DESCRIPTION:
 *     Implementation of functions for parsing constant expressions.
 *
 * AUTHOR: Moritz Röhrich <moritzr@pool.math.tu-berlin.de>
 *
 */

#include "parse.h"

asn* parse_const_exp(token** tl, size_t* tnt){
    printf("parsing const expr. %lu tokens\n", *tnt);
    token* tlp = *tl;
    asn* r = NULL;
    int i_val;
    double f_val;
    char* s_val;

    switch(tlp->type){
        case const_int:
            printf("found integer const\n");
            i_val = atoi(tlp->str);
            r = make_int_exp(i_val);
            break;
        case const_real:
            f_val = atof(tlp->str);
            if(real_count == 0)
                real_index = (double*) malloc(sizeof(double));
            else
                real_index = (double*) realloc(real_index, (real_count + 1) * sizeof(double));
            real_index[real_count] = f_val;
            r = make_real_exp(real_count);
            real_count++;
            printf("found real const %f\n", f_val);
            break;
        case const_char:
            printf("%s\n", tlp->str);
            break;
        case const_string:
            printf("found string const\n");
            s_val = tlp->str;
            i_val = atoi(s_val);
            r = make_string_exp(i_val);
            break;
        default: return NULL;
    }

    pop_token(&tlp, tl, tnt);
    if(tlp != NULL && tlp->type == token_semi_colon){
        pop_token(&tlp, tl, tnt);
    }

    return r;
}

