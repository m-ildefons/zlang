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

	pv_root* old_symbol_map_ptr = symbol_map_ptr;
	symbol_map_ptr = node->op.fun_def_exp.symbol_map;

	pv_leaf* func = pv_search(symbol_map_ptr, node->op.fun_def_exp.ident);

    quadruple* q1 = make_quad(fac_func_start, func, NULL, NULL);
	func->ref_count++;
    quadruple* q2 = make_quad(fac_label, func, NULL, NULL);
	func->ref_count++;
    quadruple* q3 = make_quad(fac_func_end, func, NULL, NULL);
	func->ref_count++;

    quad_list_app_quad(&IC, q1);
    quad_list_app_quad(&IC, q2);
    quad_list* body = ic_gen_body(node->op.fun_def_exp.body);
    quad_list_app_quad_list(&IC, body);
    quad_list_app_quad(&IC, q3);

	symbol_map_ptr = old_symbol_map_ptr;

    return IC;
}

quad_list* ic_gen_fun_call(asn* node){
    quad_list* IC = NULL;

    pv_leaf* call = new_pv_leaf(node->op.call_exp.ident, at_func, 0, 0, 0);

    quadruple* q1 = make_quad(fac_call, call, NULL, NULL);
    quad_list_app_quad(&IC, q1);
    return IC;
}

