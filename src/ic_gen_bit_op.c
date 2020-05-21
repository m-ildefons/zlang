/*
 * FILENAME: ic_gen_bit_op.c
 *
 * DESCRIPTION
 *     Implementation of functions for intermediate code generation for
 *     bitwise operations.
 *
 * AUTHOR: Moritz Röhrich <moritzr@pool.math.tu-berlin.de>
 *
 */

#include "ic_gen.h"

quad_list* ic_gen_bit_op(asn* node){
    quad_list* IC = NULL;

	quad_list* lhs_ic = ic_gen(node->op.binary_exp.lval);
	symbol* lhs = get_tmp();

	quad_list* rhs_ic = ic_gen(node->op.binary_exp.rval);
	symbol* rhs = get_tmp();

	symbol* res = gen_tmp();
	copy_type_list(lhs, &res);

	int fac;
	switch(node->tag){
		case bit_shift_left_tag: fac = fac_shl; break;
		case bit_shift_right_tag: fac = fac_shr; break;
		case bit_and_tag: fac = fac_and; break;
		case bit_xor_tag: fac = fac_xor; break;
		case bit_or_tag: fac = fac_or; break;
		default: ic_error("Compiler error in %s\n", __func__);
	}

	quadruple* q = make_quad(fac, lhs, rhs, res);

	quad_list_app_quad_list(&IC, lhs_ic);
	quad_list_app_quad_list(&IC, rhs_ic);
	quad_list_app_quad(&IC, q);

	delete_symbol(&res);
    return IC;
}



