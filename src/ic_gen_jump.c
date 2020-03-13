/*
 * FILENAME: ic_gen_jump.c
 *
 * DESCRIPTION
 *     Implementation of functions for intermediate code generation for
 *     jumping statements.
 *
 * AUTHOR: Moritz Röhrich <moritzr@pool.math.tu-berlin.de>
 *
 */

#include "ic_gen.h"

quad_list* ic_gen_ret(asn* node){
    quad_list* IC = NULL;
	if(node->op.ret_exp.val != NULL)
		IC = ic_gen(node->op.ret_exp.val);
    quadruple* q = make_quad(fac_ret, NULL, NULL, NULL);
    quad_list_app_quad(&IC, q);
    return IC;
}

quad_list* ic_gen_break(asn* node){
    quad_list* IC = NULL;
    return IC;
}

quad_list* ic_gen_continue(asn* node){
    quad_list* IC = NULL;
    return IC;
}



