/*
 * FILENAME: ic_gen_cond.c
 *
 * DESCRIPTION
 *     Implementation of functions for intermediate code generation for
 *     conditional statements.
 *
 * AUTHOR: Moritz Röhrich <moritzr@pool.math.tu-berlin.de>
 *
 */

#include "ic_gen.h"


quad_list* ic_gen_cond(asn* node){
    quad_list* IC = NULL;

//    symbol_list* old_symbol_list_ptr = symbol_list_ptr;

    quad_list* cond = ic_gen(node->op.cond_exp.cond);

    symbol* res = get_tmp();

	symbol* zero = new_symbol("0");
    quadruple* qcmp = make_quad(fac_compare, zero, res, NULL);

    quad_list_app_quad_list(&IC, cond);
    quad_list_app_quad(&IC, qcmp);

    symbol* label = gen_label("lpos");
    quadruple* qlabel = make_quad(fac_label, label, NULL, NULL);
    quadruple* jlabel = make_quad(fac_je, label, NULL, NULL);

	symbol* end = gen_label("end");
    quadruple* qend = make_quad(fac_label, end, NULL, NULL);
    quadruple* jend = make_quad(fac_jump, end, NULL, NULL);

    quad_list_app_quad(&IC, jlabel);

//    symbol_list_ptr = new_symbol_list((size_t) node->op.cond_exp.scope);
//    symbol_list_attach(&(node->op.cond_exp.if_symbols), &symbol_list_ptr);

    quad_list* if_body = ic_gen_body(node->op.cond_exp.if_body);
    quad_list_app_quad_list(&IC, if_body);

//    delete_symbol_list(symbol_list_ptr);

    quad_list_app_quad(&IC, jend);
    quad_list_app_quad(&IC, qlabel);

    if(node->op.cond_exp.else_body != NULL){
//        symbol_list_ptr = new_symbol_list((size_t) node->op.cond_exp.scope);
//        symbol_list_attach(&(node->op.cond_exp.else_symbols), &symbol_list_ptr);

        quad_list* else_body = ic_gen_body(node->op.cond_exp.else_body);
        quad_list_app_quad_list(&IC, else_body);

//        delete_symbol_list(symbol_list_ptr);
    }

    quad_list_app_quad(&IC, qend);

    delete_symbol(&zero);
    delete_symbol(&label);
    delete_symbol(&end);
//    symbol_list_ptr = old_symbol_list_ptr;
    return IC;
}

