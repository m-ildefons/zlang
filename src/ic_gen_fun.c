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

    symbol_list* old_symbol_list_ptr = symbol_list_ptr;
    symbol_list_ptr = new_symbol_list((size_t) node->op.fun_def_exp.scope);
    symbol_list_attach(&(node->op.fun_def_exp.symbols), &symbol_list_ptr);

    symbol* sym_func = search_symbol(symbol_list_ptr,
                                    node->op.fun_def_exp.ident);
    quadruple* q1 = make_quad(fac_label, sym_func, NULL, NULL);
    quadruple* q2 = make_quad(fac_func_start, sym_func, NULL, NULL);
    quadruple* q3 = make_quad(fac_func_end, sym_func, NULL, NULL);

    quad_list_app_quad(&IC, q1);
    quad_list_app_quad(&IC, q2);
    quad_list* body = ic_gen_body(node->op.fun_def_exp.body);
    quad_list_app_quad_list(&IC, body);
    quad_list_app_quad(&IC, q3);

    delete_symbol_list(symbol_list_ptr);
    symbol_list_ptr = old_symbol_list_ptr;
    return IC;
}

quad_list* ic_gen_fun_call(asn* node){
    quad_list* IC = NULL;

    symbol* sym_call = search_symbol(symbol_list_ptr,
                                    node->op.call_exp.ident);
    int external = 1;
    if(sym_call != NULL)
        external = 0;
    else
        sym_call = new_symbol(node->op.call_exp.ident, at_func);

    char* res_id = gen_tmp_name();
    symbol* res = new_symbol(res_id, at_void);
    symbol_list_append(&symbol_list_ptr, &res);

    quadruple* q_call = make_quad(fac_call, sym_call, NULL, res);
    quad_list_app_quad(&IC, q_call);

    free(res_id);
    delete_symbol(&res);
    if(external == 1)
        delete_symbol(&sym_call);

    return IC;
}

