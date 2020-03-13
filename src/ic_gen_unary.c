/*
 * FILENAME: ic_gen_unary.c
 *
 * DESCRIPTION
 *     Implementation of functions for intermediate code generation for
 *     unary operations.
 *
 * AUTHOR: Moritz Röhrich <moritzr@pool.math.tu-berlin.de>
 *
 */

#include "ic_gen.h"

quad_list* ic_gen_unary_minus(asn* node){
    quad_list* IC = NULL;
    return IC;
}

quad_list* ic_gen_unary_not(asn* node){
    quad_list* IC = NULL;
    return IC;
}

quad_list* ic_gen_unary_compl(asn* node){
    quad_list* IC = NULL;
    return IC;
}

quad_list* ic_gen_reference(asn* node){
    quad_list* IC = NULL;
    return IC;
}

quad_list* ic_gen_dereference(asn* node){
    quad_list* IC = NULL;
    return IC;
}

quad_list* ic_gen_inc(asn* node){
    quad_list* IC = NULL;
    return IC;
}

quad_list* ic_gen_dec(asn* node){
    quad_list* IC = NULL;
    quad_list* inner = ic_gen(node->op.unary_exp.val);
    quad_list_app_quad_list(&IC, inner);
    return IC;
}


