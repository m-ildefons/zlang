/*
 * FILENAME: ic_gen_fun.c
 *
 * DESCRIPTION
 *     Implementation of functions for intermediate code generation for
 *     definition, declaration and call of functions.
 *
 * AUTHOR: Moritz Röhrich <moritzr@pool.math.tu-berlin.de>
 *
 */

#include "ic_gen.h"

quad_list* ic_gen_fun_def(asn* node){
    quad_list* IC = NULL;

    pv_leaf* fun_start = new_pv_leaf(node->op.fun_def_exp.ident, at_func, 0, 0, 0);
    pv_leaf* fun_end = new_pv_leaf(node->op.fun_def_exp.ident, at_func, 0, 0, 0);

    quadruple* q1 = make_quad(fac_func_start, fun_start, NULL, NULL);
    quadruple* q2 = make_quad(fac_func_end, fun_end, NULL, NULL);

    quad_list_app_quad(&IC, q1);
    quad_list* body = ic_gen_body(node->op.fun_def_exp.body);
    quad_list_app_quad_list(&IC, body);
    quad_list_app_quad(&IC, q2);

    return IC;
}

quad_list* ic_gen_fun_call(asn* node){
    quad_list* IC = NULL;

    pv_leaf* call = new_pv_leaf(node->op.call_exp.ident, at_func, 0, 0, 0);

    quadruple* q1 = make_quad(fac_call, call, NULL, NULL);
    quad_list_app_quad(&IC, q1);
    return IC;
}

