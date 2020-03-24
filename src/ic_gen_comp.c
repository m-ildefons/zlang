/*
 * FILENAME: ic_gen_comp.c
 *
 * DESCRIPTION
 *     Implementation of functions for intermediate code generation for
 *     comparison operators.
 *
 * AUTHOR: Moritz Röhrich <moritzr@pool.math.tu-berlin.de>
 *
 */

#include "ic_gen.h"

quad_list* ic_gen_comp(asn* node){
    quad_list* IC = NULL;

    quad_list* lhs = ic_gen(node->op.binary_exp.lval);
    char* cmp1_id = get_tmp_name();
    symbol* cmp1 = search_symbol(symbol_list_ptr, cmp1_id);

    quad_list* rhs = ic_gen(node->op.binary_exp.rval);
    char* cmp2_id = get_tmp_name();
    symbol* cmp2 = search_symbol(symbol_list_ptr, cmp2_id);

    char* res_id = gen_tmp_name();
    symbol* res = new_symbol(res_id, at_int);
    symbol_list_append(&symbol_list_ptr, &res);

    int fac;
    switch(node->tag){
        case less_tag: fac = fac_setl; break;
        case less_or_equal_tag: fac = fac_setle; break;
        case greater_tag: fac = fac_setg; break;
        case greater_or_equal_tag: fac = fac_setge; break;
        case equal_tag: fac = fac_sete; break;
        case not_equal_tag: fac = fac_setne; break;
        default: fac = 0;
    }

    quadruple* qcmp = make_quad(fac_compare, cmp1, cmp2, NULL);
    quadruple* set = make_quad(fac, NULL, NULL, res);

    quad_list_app_quad_list(&IC, lhs);
    quad_list_app_quad_list(&IC, rhs);
    quad_list_app_quad(&IC, qcmp);
    quad_list_app_quad(&IC, set);

    free(cmp1_id);
    free(cmp2_id);
    free(res_id);
    delete_symbol(&res);
    return IC;
}

