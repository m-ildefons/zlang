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

static unsigned int label_counter;
const char* gen_label(const char* cust_str);

/* implemented in asm_gen.c */
void asm_gen_prog(asn* ast_tree, char** asm_src);
const char* asm_gen(asn* e);
const char* asm_gen_fun_def(asn* fun_def);
const char* asm_gen_fun_call(asn* call);
const char* asm_gen_var_def(asn* var_def);
const char* asm_gen_var_ref(asn* var_ref);
const char* asm_gen_global_var(asn* e);
const char* asm_gen_ret(asn* ret, int num_vars);
const char* asm_gen_int_const(asn* int_const);
const char* asm_gen_string_const(asn* str_const);
const char* asm_gen_body(asn_list* body, int num_vars);
const char* asm_gen_jump(asn* jump);

/* implemented in asm_gen_unary_op.c */
const char* asm_gen_unary_minus(asn* u_minus);
const char* asm_gen_unary_not(asn* u_not);
const char* asm_gen_unary_compl(asn* u_compl);
const char* asm_gen_reference(asn* ref);
const char* asm_gen_dereference(asn* deref);
const char* asm_gen_inc(asn* inc);
const char* asm_gen_dec(asn* dec);

/* implemented in asm_gen_calc.c */
const char* asm_gen_binary_plus(asn* b_plus);
const char* asm_gen_binary_mult(asn* b_mult);
const char* asm_gen_binary_sub(asn* b_sub);
const char* asm_gen_binary_div(asn* b_div);
const char* asm_gen_binary_mod(asn* b_mod);

/* implemented in asm_gen_comp.c */
const char* asm_gen_less(asn* less);
const char* asm_gen_less_or_equal(asn* leq);
const char* asm_gen_greater(asn* greater);
const char* asm_gen_greater_or_equal(asn* geq);
const char* asm_gen_equal(asn* eq);
const char* asm_gen_not_equal(asn* neq);

/* implemented in asm_gen_bit_op.c */
const char* asm_gen_bit_op(asn* e);

/* implemented in asm_gen_logic_op.c */
const char* asm_gen_log_and(asn* l_and);
const char* asm_gen_log_xor(asn* l_xor);
const char* asm_gen_log_or(asn* l_or);

/* implemented in asm_gen_cond.c */
const char* asm_gen_cond(asn* cond);

/* implemented in asm_gen_assign.c */
const char* asm_gen_assign(asn* assign);

/* implemented in asm_gen_loop.c */
const char* asm_gen_for_loop(asn* for_loop);
const char* asm_gen_while_loop(asn* while_loop);

#endif

