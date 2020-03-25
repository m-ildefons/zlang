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
    quadruple* q = NULL;
	if(node->op.ret_exp.val != NULL){
		IC = ic_gen(node->op.ret_exp.val);

        char* val_id = get_tmp_name();
        symbol* val = search_symbol(symbol_list_ptr, val_id);
        q = make_quad(fac_ret, val, NULL, NULL);
        free(val_id);
    } else {
        q = make_quad(fac_ret, NULL, NULL, NULL);
    }

    quad_list_app_quad(&IC, q);
    return IC;
}

quad_list* ic_gen_break(){
    quad_list* IC = NULL;
    quadruple* q = make_quad(fac_jump, loop_end_symbol, NULL, NULL);
    quad_list_app_quad(&IC, q);
    return IC;
}

quad_list* ic_gen_continue(){
    quad_list* IC = NULL;
    quadruple* q = make_quad(fac_jump, loop_start_symbol, NULL, NULL);
    quad_list_app_quad(&IC, q);
    return IC;
}



