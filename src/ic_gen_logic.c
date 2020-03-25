/*
 * FILENAME: ic_gen_logic.c
 *
 * DESCRIPTION
 *     Implementation of functions for intermediate code generation for
 *     logical operators.
 *
 * AUTHOR: Moritz Röhrich <moritzr@pool.math.tu-berlin.de>
 *
 */

#include "ic_gen.h"

quad_list* ic_gen_log_and(asn* node){
    quad_list* IC = NULL;

    quad_list* lval_code = ic_gen(node->op.binary_exp.lval);
    char* lval_res_id = get_tmp_name();
    symbol* lval_res = search_symbol(symbol_list_ptr, lval_res_id);

    quad_list* rval_code = ic_gen(node->op.binary_exp.rval);
    char* rval_res_id = get_tmp_name();
    symbol* rval_res = search_symbol(symbol_list_ptr, rval_res_id);

    char* zero_id = salloc(12);
    sprintf(zero_id, "0");
    symbol* zero = new_symbol(zero_id, at_int);
    quadruple* qcmplval = make_quad(fac_compare, zero, lval_res, NULL);
    quadruple* qcmprval = make_quad(fac_compare, zero, rval_res, NULL);

    char* label_id = gen_label("label");
    symbol* label = new_symbol(label_id, at_void);
    symbol_list_append(&symbol_list_ptr, &label);
    quadruple* qlabel = make_quad(fac_label, label, NULL, NULL);
    quadruple* jlabel = make_quad(fac_jne, label, NULL, NULL);

    char* end_id = gen_label("end");
    symbol* end = new_symbol(end_id, at_void);
    symbol_list_append(&symbol_list_ptr, &end);
    quadruple* qend = make_quad(fac_label, end, NULL, NULL);
    quadruple* jend = make_quad(fac_jump, end, NULL, NULL);

    char* res_id = gen_tmp_name();
    symbol* res = new_symbol(res_id, at_int);
    symbol_list_append(&symbol_list_ptr, &res);
    quadruple* qset = make_quad(fac_setne, NULL, NULL, res);
    quadruple* set_zero = make_quad(fac_load, zero, NULL, res);

    quad_list_app_quad_list(&IC, lval_code);
    quad_list_app_quad(&IC, qcmplval);
    quad_list_app_quad(&IC, jlabel);
    quad_list_app_quad(&IC, set_zero);
    quad_list_app_quad(&IC, jend);
    quad_list_app_quad(&IC, qlabel);
    quad_list_app_quad_list(&IC, rval_code);
    quad_list_app_quad(&IC, qcmprval);
    quad_list_app_quad(&IC, qset);
    quad_list_app_quad(&IC, qend);

    free(lval_res_id);
    free(rval_res_id);
    free(zero_id);
    free(label_id);
    free(end_id);
    free(res_id);
    delete_symbol(&zero);
    delete_symbol(&label);
    delete_symbol(&end);
    delete_symbol(&res);
    return IC;
}

quad_list* ic_gen_log_xor(asn* node){
    quad_list* IC = NULL;
    return IC;
}

quad_list* ic_gen_log_or(asn* node){
    quad_list* IC = NULL;
    return IC;
}

