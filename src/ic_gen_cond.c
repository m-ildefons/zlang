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
//
//// TODO: Make correct, the pv_leaf situation of the else symbol map
//// also make sure all facs are created correctly.
////
//	pv_root* old_symbol_map_ptr = symbol_map_ptr;
//    pv_pretty_print(symbol_map_ptr);
//    symbol_map_ptr = copy_trie(node->op.cond_exp.if_symbol_map);
//
//    pv_pretty_print(symbol_map_ptr);
//	char* post_body_tmp = gen_tmp_name();
//	pv_leaf* post_body_leaf = new_pv_leaf(post_body_tmp, at_void, 0, 0, 0);
//    pv_root* osm = symbol_map_ptr;
//    symbol_map_ptr = pv_insert(symbol_map_ptr, post_body_tmp, post_body_leaf);
//    delete_trie(osm);
//
//    pv_pretty_print(symbol_map_ptr);
//
//    char* post_else_tmp = NULL;
//    pv_leaf* post_else_leaf = NULL;
//	if(node->op.cond_exp.else_body){
//	    post_else_tmp = gen_tmp_name();
//	    post_else_leaf = new_pv_leaf(post_else_tmp, at_void, 0, 0, 0);
//    }
//
//	quad_list* cond = ic_gen(node->op.cond_exp.cond);
//	quad_list_app_quad_list(&IC, cond);
//
//    quad_list* if_body = ic_gen_body(node->op.cond_exp.if_body);
//    quad_list_app_quad_list(&IC, if_body);
//
//	if(node->op.cond_exp.else_body){
//		quadruple* post_else_jump = make_quad(fac_jump, post_else_leaf, NULL, NULL);
//		quad_list_app_quad(&IC, post_else_jump);
//	}
//
//	quadruple* post_body = make_quad(fac_label, post_body_leaf, NULL, NULL);
//	quad_list_app_quad(&IC, post_body);
//
//    if(node->op.cond_exp.else_body != NULL){
//        delete_trie(symbol_map_ptr);
//        symbol_map_ptr = copy_trie(node->op.cond_exp.else_symbol_map);
//
//        osm = symbol_map_ptr;
//        symbol_map_ptr = pv_insert(symbol_map_ptr, post_else_tmp, post_else_leaf);
//        delete_trie(osm);
//
//        quad_list* else_body = ic_gen_body(node->op.cond_exp.else_body);
//        quad_list_app_quad_list(&IC, else_body);
//
//		quadruple* post_else = make_quad(fac_label, post_else_leaf, NULL, NULL);
//		quad_list_app_quad(&IC, post_else);
//    }
//
//    delete_trie(symbol_map_ptr);
//	symbol_map_ptr = old_symbol_map_ptr;
//
//	free(post_body_tmp);
//	free(post_else_tmp);
    return IC;
}

