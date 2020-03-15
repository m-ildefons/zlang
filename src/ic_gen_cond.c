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

	pv_root* old_symbol_map_ptr = symbol_map_ptr;
	symbol_map_ptr = node->op.cond_exp.if_symbol_map;

	char* post_body_tmp = gen_tmp_name();
	pv_leaf* post_body_leaf = new_pv_leaf(post_body_tmp, at_void, 0, 0, 0);

	char* post_else_tmp = gen_tmp_name();
	pv_leaf* post_else_leaf = new_pv_leaf(post_else_tmp, at_void, 0, 0, 0);

	quad_list* cond = ic_gen(node->op.cond_exp.cond);
	quad_list_app_quad_list(&IC, cond);

    quad_list* if_body = ic_gen_body(node->op.cond_exp.if_body);
    quad_list_app_quad_list(&IC, if_body);

	if(node->op.cond_exp.else_body){
		quadruple* post_else_jump = make_quad(fac_jump, post_else_leaf, NULL, NULL);
		quad_list_app_quad(&IC, post_else_jump);
	}

	quadruple* post_body = make_quad(fac_label, post_body_leaf, NULL, NULL);
	quad_list_app_quad(&IC, post_body);

    if(node->op.cond_exp.else_body != NULL){
        symbol_map_ptr = node->op.cond_exp.else_symbol_map;
        quad_list* else_body = ic_gen_body(node->op.cond_exp.else_body);
        quad_list_app_quad_list(&IC, else_body);

		post_else_leaf->ref_count++;
		quadruple* post_else = make_quad(fac_label, post_else_leaf, NULL, NULL);
		quad_list_app_quad(&IC, post_else);
    }

	symbol_map_ptr = old_symbol_map_ptr;
	free(post_else_tmp);
	free(post_body_tmp);
    return IC;
}

