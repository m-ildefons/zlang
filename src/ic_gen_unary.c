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
    quad_list* inner = ic_gen(node->op.unary_exp.val);

    char* tmp_id = get_tmp_name();
    symbol* tmp = search_symbol(symbol_list_ptr, tmp_id);

    char* tmp2_id = gen_tmp_name();
    symbol* tmp2 = new_symbol(tmp2_id);
    copy_type_list(tmp, &tmp2);
    symbol_list_append(&symbol_list_ptr, &tmp2);

    quadruple* q = make_quad(fac_neg, tmp, NULL, tmp2);

    quad_list_app_quad_list(&IC, inner);
    quad_list_app_quad(&IC, q);

    free(tmp_id);
    free(tmp2_id);
    delete_symbol(&tmp2);
    return IC;
}

quad_list* ic_gen_unary_not(asn* node){
    quad_list* IC = NULL;
    quad_list* inner = ic_gen(node->op.unary_exp.val);
    quad_list_app_quad_list(&IC, inner);
    return IC;
}

quad_list* ic_gen_unary_compl(asn* node){
    quad_list* IC = NULL;
    quad_list* inner = ic_gen(node->op.unary_exp.val);
    quad_list_app_quad_list(&IC, inner);
    return IC;
}

quad_list* ic_gen_reference(asn* node){
    quad_list* IC = NULL;
    quad_list* inner = ic_gen(node->op.unary_exp.val);
    quad_list_app_quad_list(&IC, inner);
    return IC;
}

quad_list* ic_gen_dereference(asn* node){
    quad_list* IC = NULL;
    quad_list* inner = ic_gen(node->op.unary_exp.val);
    quad_list_app_quad_list(&IC, inner);
    return IC;
}

quad_list* ic_gen_incdec(asn* node){
    quad_list* IC = NULL;
    symbol* val = NULL;
    if(node->op.unary_exp.val->tag == var_tag)
        val = node->op.unary_exp.val->op.var_exp.sym;

    quad_list* inner = ic_gen(node->op.unary_exp.val);
    char* inner_id = get_tmp_name();
    symbol* inner_sym = search_symbol(symbol_list_ptr, inner_id);

    char* one_id = "1";
    symbol* one_sym = new_symbol(one_id);
    copy_type_list(inner_sym, &one_sym);
    symbol_list_append(&symbol_list_ptr, &one_sym);

    char* tmp_id = gen_tmp_name();
    symbol* tmp_sym = new_symbol(tmp_id);
    copy_type_list(one_sym, &tmp_sym);
    symbol_list_append(&symbol_list_ptr, &tmp_sym);

    char* res_id = gen_tmp_name();
    symbol* res_sym = new_symbol(res_id);
    copy_type_list(inner_sym, &res_sym);
    symbol_list_append(&symbol_list_ptr, &res_sym);

    int fac_type;
    switch(node->tag){
        case inc_tag: fac_type = fac_add; break;
        case dec_tag: fac_type = fac_sub; break;
        default: fac_type = fac_add;
    }

    quadruple* q1 = make_quad(fac_load, one_sym, NULL, tmp_sym);
    quadruple* q2 = make_quad(fac_type, inner_sym, tmp_sym, res_sym);
    quadruple* q3;
    if(val != NULL)
        q3 = make_quad(fac_store, res_sym, NULL, val);
    else
        q3 = make_quad(fac_store, res_sym, NULL, inner_sym);

    quad_list_app_quad_list(&IC, inner);
    quad_list_app_quad(&IC, q1);
    quad_list_app_quad(&IC, q2);
    quad_list_app_quad(&IC, q3);

    free(inner_id);
    free(tmp_id);
    free(res_id);
    delete_symbol(&one_sym);
    delete_symbol(&tmp_sym);
    delete_symbol(&res_sym);
    return IC;
}

