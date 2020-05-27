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

//    symbol_list* old_symbol_list_ptr = symbol_list_ptr;
//    symbol_list_ptr = new_symbol_list((size_t) node->op.while_loop_exp.scope);
//    symbol_list_attach(&(node->op.while_loop_exp.symbols), &symbol_list_ptr);

	symbol* start_label = gen_label("wstart");
    quadruple* qstart = make_quad(fac_label, start_label, NULL, NULL);
	quadruple* jstart = make_quad(fac_jump, start_label, NULL, NULL);
    loop_start_symbol = start_label;

    symbol* end_label = gen_label("wend");
    quadruple* qend = make_quad(fac_label, end_label, NULL, NULL);
	quadruple* jend = make_quad(fac_je, end_label, NULL, NULL);
    loop_end_symbol = end_label;

    quad_list* cond = ic_gen(node->op.while_loop_exp.cond);
    quad_list* body = ic_gen_body(node->op.while_loop_exp.body);

    quad_list_app_quad(&IC, qstart);
    quad_list_app_quad_list(&IC, cond);
    quad_list_app_quad(&IC, jend);
    quad_list_app_quad_list(&IC, body);
	quad_list_app_quad(&IC, jstart);
    quad_list_app_quad(&IC, qend);

    loop_start_symbol = NULL;
    loop_end_symbol = NULL;
//    delete_symbol_list(symbol_list_ptr);
//    symbol_list_ptr = old_symbol_list_ptr;
    delete_symbol(&start_label);
    delete_symbol(&end_label);
    return IC;
}

quad_list* ic_gen_for_loop(asn* node){
    quad_list* IC = NULL;

//    symbol_list* old_symbol_list_ptr = symbol_list_ptr;
//    symbol_list_ptr = new_symbol_list((size_t) node->op.for_loop_exp.scope);
//    symbol_list_attach(&(node->op.for_loop_exp.symbols), &symbol_list_ptr);

    symbol* start_label = gen_label("fstart");
    quadruple* qstart = make_quad(fac_label, start_label, NULL, NULL);
	quadruple* jstart = make_quad(fac_jump, start_label, NULL, NULL);
    loop_start_symbol = start_label;

    symbol* end_label = gen_label("fend");
    quadruple* qend = make_quad(fac_label, end_label, NULL, NULL);
	quadruple* jend = make_quad(fac_je, end_label, NULL, NULL);
    loop_end_symbol = end_label;

    quad_list* init = ic_gen(node->op.for_loop_exp.init);
    quad_list* cond = ic_gen(node->op.for_loop_exp.cond);
    quad_list* move = ic_gen(node->op.for_loop_exp.move);
    quad_list* body = ic_gen_body(node->op.for_loop_exp.body);

    quad_list_app_quad_list(&IC, init);
    quad_list_app_quad(&IC, qstart);
    quad_list_app_quad_list(&IC, cond);
	quad_list_app_quad(&IC, jend);
    quad_list_app_quad_list(&IC, body);
    quad_list_app_quad_list(&IC, move);
	quad_list_app_quad(&IC, jstart);
    quad_list_app_quad(&IC, qend);

    loop_start_symbol = NULL;
    loop_end_symbol = NULL;
//    delete_symbol_list(symbol_list_ptr);
//    symbol_list_ptr = old_symbol_list_ptr;
    delete_symbol(&start_label);
    delete_symbol(&end_label);
    return IC;
}



