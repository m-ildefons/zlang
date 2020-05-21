/*
 * FILENAME: ic_gen.h
 *
 * DESCRIPTION
 *     Declarations of functions for intermediate code generation.
 *
 * AUTHOR: Moritz Röhrich <moritzr@pool.math.tu-berlin.de>
 *
 */

#ifndef __IC_GEN_H__
#define __IC_GEN_H__

#include "ast.h"
#include "ic.h"
#include "parse.h" // for symbol_list_ptr


symbol* loop_start_symbol;
symbol* loop_end_symbol;

/* implemented in ic_gen.c */
char* get_tmp_name(void);
char* gen_tmp_name(void);
char* gen_label_name(const char* cust_str);
symbol* gen_symbol(const char* name);
symbol* get_tmp(void);
symbol* gen_tmp(void);
symbol* gen_label(const char* name);

void ic_error(const char* fmt, ...);
void ic_warning(const char* fmt, ...);

quad_list* ic_gen_translation_unit(asn* ast);
quad_list* ic_gen(asn* node);
quad_list* ic_gen_body(asn_list* body);

/* implemented in ic_gen_fun.c */
quad_list* ic_gen_fun_def(asn* node);
quad_list* ic_gen_fun_call(asn* node);
quad_list* ic_gen_fun_arg(symbol* s);

/* implemented in ic_gen_var.c */
quad_list* ic_gen_var(asn* node);

/* implemented in ic_gen_const.c */
quad_list* ic_gen_int_const(asn* node);
quad_list* ic_gen_real_const(asn* node);
quad_list* ic_gen_string_const(asn* node);

/* implemented in ic_gen_unary.c */
quad_list* ic_gen_unary_minus(asn* node);
quad_list* ic_gen_unary_not(asn* node);
quad_list* ic_gen_unary_compl(asn* node);
quad_list* ic_gen_reference(asn* node);
quad_list* ic_gen_dereference(asn* node);
quad_list* ic_gen_incdec(asn* node);

/* implemented in ic_gen_binary */
quad_list* ic_gen_binary(asn* node);

/* implemented in ic_gen_comp.c */
quad_list* ic_gen_comp(asn* node);

/* implemented in ic_gen_logic.c */
quad_list* ic_gen_log_and(asn* node);
quad_list* ic_gen_log_xor(asn* node);
quad_list* ic_gen_log_or(asn* node);

/* implemented in ic_gen_assign.c */
quad_list* ic_gen_assign(asn* node);

/* implemented in ic_gen_cond.c */
quad_list* ic_gen_cond(asn* node);

/* implemented in ic_gen_loop.c */
quad_list* ic_gen_while_loop(asn* node);
quad_list* ic_gen_for_loop(asn* node);

/* implemented in ic_gen_cast.c */
quad_list* ic_gen_int_to_real(asn* node);

/* implemented in ic_gen_jump.c */
quad_list* ic_gen_ret(asn* node);
quad_list* ic_gen_break(void);
quad_list* ic_gen_continue(void);

#endif

