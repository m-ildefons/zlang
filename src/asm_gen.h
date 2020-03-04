/*
 * FILENAME: asm_gen.h
 *
 * DESCRIPTION
 *     Decalrations of functions for assembly generation.
 *
 * AUTHOR: Moritz Röhrich <moritzr@pool.math.tu-berlin.de>
 *
 */

#ifndef __ASM_GEN_H__
#define __ASM_GEN_H__

#include "ast.h"
#include "strop.h"

pv_root* symbol_map_ptr;

const char* loop_start_label;
const char* loop_end_label;

/* implemented in asm_gen.c */
char* gen_label(const char* cust_str);

char* asm_gen_prog(asn* ast_tree);
char* asm_gen(asn* e);
char* asm_gen_fun_def(asn* fun_def);
char* asm_gen_fun_call(asn* call);
char* asm_gen_var_def(asn* var_def);
char* asm_gen_var_ref(asn* var_ref);
char* asm_gen_global_var(asn* e);
char* asm_gen_ret(asn* ret);
char* asm_gen_body(asn_list* body);
char* asm_gen_jump(asn* jump);
char* asm_gen_int_to_real(asn* cast);

/* implemented in asm_gen_const.c */
char* asm_gen_int_const(asn* int_const);
char* asm_gen_real_const(asn* real_const);
char* asm_gen_string_const(asn* str_const);

/* implemented in asm_gen_unary_op.c */
char* asm_gen_unary_minus(asn* u_minus);
char* asm_gen_unary_not(asn* u_not);
char* asm_gen_unary_compl(asn* u_compl);
char* asm_gen_reference(asn* ref);
char* asm_gen_dereference(asn* deref);
char* asm_gen_inc(asn* inc);
char* asm_gen_dec(asn* dec);

/* implemented in asm_gen_calc.c */
char* asm_gen_binary_plus(asn* b_plus);
char* asm_gen_binary_mult(asn* b_mult);
char* asm_gen_binary_sub(asn* b_sub);
char* asm_gen_binary_div(asn* b_div);
char* asm_gen_binary_mod(asn* b_mod);

/* implemented in asm_gen_comp.c */
char* asm_gen_less(asn* less);
char* asm_gen_less_or_equal(asn* leq);
char* asm_gen_greater(asn* greater);
char* asm_gen_greater_or_equal(asn* geq);
char* asm_gen_equal(asn* eq);
char* asm_gen_not_equal(asn* neq);

/* implemented in asm_gen_bit_op.c */
char* asm_gen_bit_op(asn* e);

/* implemented in asm_gen_logic_op.c */
char* asm_gen_log_and(asn* l_and);
char* asm_gen_log_xor(asn* l_xor);
char* asm_gen_log_or(asn* l_or);

/* implemented in asm_gen_cond.c */
char* asm_gen_cond(asn* cond);

/* implemented in asm_gen_assign.c */
char* asm_gen_assign(asn* assign);

/* implemented in asm_gen_loop.c */
char* asm_gen_for_loop(asn* for_loop);
char* asm_gen_while_loop(asn* while_loop);

#endif

