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

	pv_root* old_symbol_map_ptr = symbol_map_ptr;
	symbol_map_ptr = copy_trie(node->op.while_loop_exp.symbol_map);

    quad_list* body = ic_gen_body(node->op.while_loop_exp.body);
    quad_list_app_quad_list(&IC, body);

    delete_trie(symbol_map_ptr);
	symbol_map_ptr = old_symbol_map_ptr;
    return IC;
}

quad_list* ic_gen_for_loop(asn* node){
    quad_list* IC = NULL;

	pv_root* old_symbol_map_ptr = symbol_map_ptr;
	symbol_map_ptr = copy_trie(node->op.for_loop_exp.symbol_map);

    quad_list* body = ic_gen_body(node->op.for_loop_exp.body);
    quad_list_app_quad_list(&IC, body);

    delete_trie(symbol_map_ptr);
	symbol_map_ptr = old_symbol_map_ptr;
    return IC;
}



