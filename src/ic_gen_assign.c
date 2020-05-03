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
        default: kind = 0;
    }

    quadruple* op;
    char* tmp_id;
    symbol* tmp;

    if(node->tag != assign_tag){
        tmp_id = gen_tmp_name();
        tmp = new_symbol(tmp_id);
        op = make_quad(kind, lval, rhs, tmp);
        quad_list_app_quad(&IC, op);
        delete_symbol(&tmp);
        free(tmp_id);
        rhs = tmp;
    }

    quadruple* store = make_quad(fac_store, rhs, NULL, lval);

    quad_list_app_quad(&IC, store);

    free(rhs_id);
    return IC;
}



