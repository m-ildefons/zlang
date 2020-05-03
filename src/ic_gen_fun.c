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
                                    node->op.fun_def_exp.sym->ident);
    quadruple* q2 = make_quad(fac_func_start, sym_func, NULL, NULL);
    q2->symbol_list_ptr = node->op.fun_def_exp.symbols;
    q2->temp_list_ptr = symbol_list_ptr;
    q2->symbol_list_ptr->ref_count++;
    q2->temp_list_ptr->ref_count++;
    quadruple* q3 = make_quad(fac_func_end, sym_func, NULL, NULL);
    q3->symbol_list_ptr = node->op.fun_def_exp.symbols;
    q3->temp_list_ptr = symbol_list_ptr;
    q3->symbol_list_ptr->ref_count++;
    q3->temp_list_ptr->ref_count++;

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

    asn_list* args = node->op.call_exp.args;
    for(; args != NULL; args = args->next){
        quad_list* arg_IC = ic_gen(args->expr);
        quad_list_app_quad_list(&IC, arg_IC);
        char* arg_id = get_tmp_name();
        symbol* arg = search_symbol(symbol_list_ptr, arg_id);
        quad_list_app_quad_list(&IC, ic_gen_fun_arg(arg));
        free(arg_id);
    }

    symbol* sym_call = search_symbol(symbol_list_ptr,
                                    node->op.call_exp.ident);
    int external = 1;
    if(sym_call != NULL)
        external = 0;
    else
        sym_call = new_symbol(node->op.call_exp.ident);

    char* res_id = gen_tmp_name();
    symbol* res = new_symbol(res_id);
    copy_return_type(sym_call, &res);
    symbol_list_append(&symbol_list_ptr, &res);

    quadruple* q_call = make_quad(fac_call, sym_call, NULL, res);
    quad_list_app_quad(&IC, q_call);

    free(res_id);
    delete_symbol(&res);
    if(external == 1)
        delete_symbol(&sym_call);

    return IC;
}

quad_list* ic_gen_fun_arg(symbol* s){
    quad_list* IC = NULL;
    quadruple* q = make_quad(fac_arg, s, NULL, NULL);
    quad_list_app_quad(&IC, q);
    return IC;
}

