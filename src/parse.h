/*
 * FILENAME: parse.h
 *
 * DESCRIPTION:
 *     Declarations for parsing functions. These generate the abstract
 *     syntax tree.
 *
 * AUTHOR: Moritz Röhrich <moritzr@pool.math.tu-berlin.de>
 *
 */

#ifndef __PARSE_H__
#define __PARSE_H__

#include <stddef.h>

#include "util.h"
#include "ast.h"
#include "lex.h"
#include "int_stack.h"
#include "symbol_list.h"


symbol_list* symbol_list_ptr;

/* implemented in parse.c */
asn* parse_translation_unit(token** tl, size_t* tnt, const char* filename);
asn* parse_ident(token** tl, size_t* tnt);

void pop_token(token** tlp, token** tl, size_t* tnt);
void parse_error(const char* err, token* tok);

/* implemented in parse_expression.c */
asn* parse_exp(token** tl, size_t* tnt);
asn* parse_expression(token** tl, size_t* tnt);
asn* parse_cast_exp(token** tl, size_t* tnt);
asn* parse_postfix_exp(token** tl, size_t* tnt);
asn* parse_primary_exp(token** tl, size_t* tnt);

/* implemented in parse_declaration.c */
asn* parse_external_declaration(token** tl, size_t* tnt);
asn* parse_declaration(token** tl, size_t* tnt, type_link* decl_spec);
asn* parse_init_decl(token** tl, size_t* tnt);
asn* parse_declarator(token** tl, size_t* tnt);
type_link* parse_pointer(token** tl, size_t* tnt);
type_link* parse_declaration_specifier(token** tl, size_t* tnt);
type_link* parse_storage_class_specifier(token** tl, size_t* tnt);
type_link* parse_type_specifier(token** tl, size_t* tnt);
type_link* parse_type_qualifier(token** tl, size_t* tnt);
asn* parse_direct_declarator(token** tl, size_t* tnt);
asn_list* parse_parameter_type_list(token** tl, size_t* tnt);
asn_list* parse_parameter_list(token** tl, size_t* tnt);
asn* parse_parameter_declaration(token** tl, size_t* tnt);

/* implemented in parse_statement.c */
void parse_compound_statement(token** tl,
							size_t* tnt,
							asn_list** body,
							int top_level);
asn* parse_statement(token** tl, size_t* tnt);
asn* parse_expression_statement(token** tl, size_t* tnt);
asn* parse_selection_statement(token** tl, size_t* tnt);
asn* parse_iteration_statement(token** tl, size_t* tnt);
asn* parse_jump_statement(token** tl, size_t* tnt);

/* implemented in parse_const.c */
asn* parse_const_exp(token** tl, size_t* tnt);

/* implemented in parse_def.c */
asn* parse_function_definition(token** tl,
                            size_t* tnt,
                            asn* function);

/* implemented in parse_assign_op.c */
asn* parse_assign_exp(token** tl, size_t* tnt);

/* implemented in parse_unary_op.c */
asn* parse_unary_exp(token** tl, size_t* tnt);

/* implemented in parse_binary_op.c */
asn* parse_bin_term(token** tl, size_t* tnt);
asn* parse_bin_sum_exp(token** tl, size_t* tnt);
asn* parse_bin_bit_shift_exp(token** tl, size_t* tnt);
asn* parse_bin_rel_exp(token** tl, size_t* tnt);
asn* parse_bin_eql_exp(token** tl, size_t* tnt);
asn* parse_bin_bit_and_exp(token** tl, size_t* tnt);
asn* parse_bin_bit_xor_exp(token** tl, size_t* tnt);
asn* parse_bin_bit_or_exp(token** tl, size_t* tnt);
asn* parse_bin_log_and_exp(token** tl, size_t* tnt);
asn* parse_bin_log_xor_exp(token** tl, size_t* tnt);
asn* parse_bin_log_or_exp(token** tl, size_t* tnt);
asn* parse_bin_exp(token** tl, size_t* tnt);

/* implemented in parse_loop.c */
asn* parse_for_loop_exp(token** tl, size_t* tnt);
asn* parse_while_loop_exp(token** tl, size_t* tnt);

/* implemented in parse_struct.c */
asn* parse_struct_specifier(token** tl, size_t* tnt);

#endif

