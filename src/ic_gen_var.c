/*
 * FILENAME: ic_gen_var.c
 *
 * DESCRIPTION
 *     Implementation of functions for intermediate code generation for
 *     definition, declaration and references of variables.
 *
 * AUTHOR: Moritz Röhrich <moritzr@pool.math.tu-berlin.de>
 *
 */

#include "ic_gen.h"


quad_list* ic_gen_var_def(asn* node){
    quad_list* IC = NULL;
    quad_list* rhs = NULL;
    if(node->op.var_def_exp.val != NULL){
        rhs = ic_gen(node->op.var_def_exp.val);
    } else {
        asn* tmp_null = make_int_exp(0);
        rhs = ic_gen(tmp_null);
        delete_exp(tmp_null);
    }

    char* reg_id = get_tmp_name();
    symbol* reg = search_symbol(symbol_list_ptr, reg_id);

    symbol* var = search_symbol(symbol_list_ptr, node->op.var_def_exp.ident);

    quadruple* q = make_quad(fac_store, reg, NULL, var);

    quad_list_app_quad_list(&IC, rhs);
    quad_list_app_quad(&IC, q);

    free(reg_id);
    return IC;
}

quad_list* ic_gen_var_ref(asn* node){
    quad_list* IC = NULL;

    symbol* var = search_symbol(symbol_list_ptr, node->op.var_ref_exp.ident);

    char* reg_id = gen_tmp_name();
    symbol* reg = new_symbol(reg_id, var->type);
    symbol_list_append(&symbol_list_ptr, &reg);

    quadruple* q = make_quad(fac_load, var, NULL, reg);

    quad_list_app_quad(&IC, q);

    delete_symbol(&reg);
    free(reg_id);
    return IC;
}

