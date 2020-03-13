/*
 * FILENAME: ic_gen_binary.c
 *
 * DESCRIPTION
 *     Implementation of functions for intermediate code generation for
 *     binary operations.
 *
 * AUTHOR: Moritz Röhrich <moritzr@pool.math.tu-berlin.de>
 *
 */

#include "ic_gen.h"

quad_list* ic_gen_binary(asn* node){
    quad_list* IC = NULL;

    quad_list* lhs_ic = ic_gen(node->op.binary_exp.lval);
    char* t1_id = get_tmp_name();
    pv_leaf* t1 = new_pv_leaf(t1_id, at_void, 0, 0, 0);

    quad_list* rhs_ic = ic_gen(node->op.binary_exp.rval);
    char* t2_id = get_tmp_name();
    pv_leaf* t2 = new_pv_leaf(t2_id, at_void, 0, 0, 0);

    char* res_id = gen_tmp_name();
    pv_leaf* res = new_pv_leaf(res_id, node->op.binary_exp.type, 0, 0, 0);

    int fac;
    switch(node->tag){
        case bin_add_tag: fac = fac_add; break;
        case bin_sub_tag: fac = fac_sub; break;
        case bin_mul_tag: fac = fac_mul; break;
        case bin_div_tag: fac = fac_div; break;
        case bin_mod_tag: fac = fac_mod; break;
        default: fac = 0;
    }

    quadruple* q = make_quad(fac, t1, t2, res);

    quad_list_app_quad_list(&IC, lhs_ic);
    quad_list_app_quad_list(&IC, rhs_ic);
    quad_list_app_quad(&IC, q);

    free(t1_id);
    free(t2_id);
    free(res_id);
    return IC;
}

