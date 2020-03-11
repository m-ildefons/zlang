/*
 * FILENAME: ic_gen_const.c
 *
 * DESCRIPTION
 *     Implementation of functions for intermediate code generation for
 *     definition, of literal constants.
 *
 * AUTHOR: Moritz Röhrich <moritzr@pool.math.tu-berlin.de>
 *
 */

#include "ic_gen.h"

quad_list* ic_gen_int_const(asn* node){
    quad_list* IC = NULL;

    char* arg1_id = salloc(12);
    sprintf(arg1_id, "%12d", node->op.int_exp);
    char* res_id = gen_tmp_name();

    pv_leaf* arg1 = new_pv_leaf(arg1_id, at_int, 8, 0, 0);
    pv_leaf* res = new_pv_leaf(res_id, at_int, 8, 0, 0);

    quadruple* q = make_quad(fac_load, arg1, NULL, res);
    quad_list_app_quad(&IC, q);

    free(arg1_id);
    free(res_id);
    return IC;
}

quad_list* ic_gen_real_const(asn* node){
    quad_list* IC = NULL;
    return IC;
}

quad_list* ic_gen_string_const(asn* node){
    quad_list* IC = NULL;
    return IC;
}

