/*
 * FILENAME: ic_gen_loop.c
 *
 * DESCRIPTION
 *     Implementation of functions for intermediate code generation for
 *     looping statements.
 *
 * AUTHOR: Moritz Röhrich <moritzr@pool.math.tu-berlin.de>
 *
 */

#include "ic_gen.h"

quad_list* ic_gen_while_loop(asn* node){
    quad_list* IC = NULL;

    symbol_list* old_symbol_list_ptr = symbol_list_ptr;
    symbol_list_ptr = new_symbol_list((size_t) node->op.while_loop_exp.scope);
    symbol_list_attach(&(node->op.while_loop_exp.symbols), &symbol_list_ptr);

    char* start_label_str = gen_label("wstart");
    symbol* start_label = new_symbol(start_label_str, at_void);
    quadruple* qstart = make_quad(fac_label, start_label, NULL, NULL);
    loop_start_symbol = start_label;

    char* end_label_str = gen_label("wend");
    symbol* end_label = new_symbol(end_label_str, at_void);
    quadruple* qend = make_quad(fac_label, end_label, NULL, NULL);
    loop_end_symbol = end_label;

    quad_list* body = ic_gen_body(node->op.while_loop_exp.body);
    quad_list* cond = ic_gen(node->op.while_loop_exp.cond);

    quad_list_app_quad(&IC, qstart);
    quad_list_app_quad_list(&IC, cond);
    // need to act here
    quad_list_app_quad_list(&IC, body);
    quad_list_app_quad(&IC, qend);

    loop_start_symbol = NULL;
    loop_end_symbol = NULL;
    delete_symbol_list(symbol_list_ptr);
    symbol_list_ptr = old_symbol_list_ptr;
    free(start_label_str);
    free(end_label_str);
    delete_symbol(&start_label);
    delete_symbol(&end_label);
    return IC;
}

quad_list* ic_gen_for_loop(asn* node){
    quad_list* IC = NULL;

    symbol_list* old_symbol_list_ptr = symbol_list_ptr;
    symbol_list_ptr = new_symbol_list((size_t) node->op.for_loop_exp.scope);
    symbol_list_attach(&(node->op.for_loop_exp.symbols), &symbol_list_ptr);

    char* start_label_str = gen_label("fstart");
    symbol* start_label = new_symbol(start_label_str, at_void);
    quadruple* qstart = make_quad(fac_label, start_label, NULL, NULL);
    loop_start_symbol = start_label;

    char* end_label_str = gen_label("fend");
    symbol* end_label = new_symbol(end_label_str, at_void);
    quadruple* qend = make_quad(fac_label, end_label, NULL, NULL);
    loop_end_symbol = end_label;

    quad_list* body = ic_gen_body(node->op.for_loop_exp.body);
    quad_list* init = ic_gen(node->op.for_loop_exp.init);
    quad_list* cond = ic_gen(node->op.for_loop_exp.cond);
    quad_list* move = ic_gen(node->op.for_loop_exp.move);

    quad_list_app_quad_list(&IC, init);
    quad_list_app_quad(&IC, qstart);
    quad_list_app_quad_list(&IC, cond);
    // need to act here
    quad_list_app_quad_list(&IC, body);
    quad_list_app_quad_list(&IC, move);
    quad_list_app_quad(&IC, qend);

    loop_start_symbol = NULL;
    loop_end_symbol = NULL;
    delete_symbol_list(symbol_list_ptr);
    symbol_list_ptr = old_symbol_list_ptr;
    free(start_label_str);
    free(end_label_str);
    delete_symbol(&start_label);
    delete_symbol(&end_label);
    return IC;
}



