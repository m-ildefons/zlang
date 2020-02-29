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

asn* parse_const_exp(token** tl, size_t* tnt, pv_root* symbol_map){
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
        case const_float:
            f_val = atof(tlp->str);
            if(float_count == 0)
                float_index = (double*) malloc(sizeof(double));
            else
                float_index = (double*) realloc(float_index, (float_count + 1) * sizeof(double));
            float_index[float_count] = f_val;
            r = make_float_exp(float_count);
            float_count++;
            printf("found float const %f\n", f_val);
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
    if(tlp->type == token_semi_colon){
		pop_token(&tlp, tl, tnt);
    }

    return r;
}

