/*
 * FILENAME: ic_gen_assign.c
 *
 * DESCRIPTION
 *     Implementation of functions for intermediate code generation for
 *     assignment operations.
 *
 * AUTHOR: Moritz Röhrich <moritzr@pool.math.tu-berlin.de>
 *
 */

#include "ic_gen.h"


quad_list* ic_gen_assign(asn* node){
    quad_list* IC = NULL;
    symbol* lval;
    if(node->op.assign_exp.lval->tag == var_tag)
        lval = node->op.assign_exp.lval->op.var_exp.sym;

    quad_list* lhs_code = ic_gen(node->op.assign_exp.lval);
    char* lhs_id = get_tmp_name();
    symbol* lhs = search_symbol(symbol_list_ptr, lhs_id);
    if(node->tag != assign_tag){
        quad_list_app_quad_list(&IC, lhs_code);
    } else {
        delete_quad_list(lhs_code);
    }

    quad_list* rhs_code = ic_gen(node->op.assign_exp.rval);
    char* rhs_id = get_tmp_name();
    symbol* rhs = search_symbol(symbol_list_ptr, rhs_id);
    quad_list_app_quad_list(&IC, rhs_code);


    int kind;
    switch(node->tag){
        case add_assign_tag: kind = fac_add; break;
        case sub_assign_tag: kind = fac_sub; break;
        case mul_assign_tag: kind = fac_mul; break;
        case div_assign_tag: kind = fac_div; break;
        case mod_assign_tag: kind = fac_mod; break;
        case and_assign_tag: kind = fac_and; break;
        case xor_assign_tag: kind = fac_xor; break;
        case or_assign_tag: kind = fac_or; break;
        default: kind = 0;
    }

    quadruple* op;
    char* tmp_id;
    symbol* tmp;

    if(node->tag != assign_tag){
        tmp_id = gen_tmp_name();
        tmp = new_symbol(tmp_id);
        copy_type_list(lhs, &tmp);
        symbol_list_append(&symbol_list_ptr, &tmp);
        op = make_quad(kind, lhs, rhs, tmp);
        quad_list_app_quad(&IC, op);
        delete_symbol(&tmp);
        free(tmp_id);
        rhs = tmp;
    }

    quadruple* store = make_quad(fac_store, rhs, NULL, lval);

    quad_list_app_quad(&IC, store);

    free(lhs_id);
    free(rhs_id);
    return IC;
}



