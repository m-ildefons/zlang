/*
 * FILENAME: lex.h
 *
 * DESCRIPTION:
 *     Declarations of structures and functions for lexical analysis of
 *     source code.
 *
 * AUTHOR: Moritz Röhrich <moritzr@pool.math.tu-berlin.de>
 *
 */

#ifndef __LEX_H__
#define __LEX_H__

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include "fileop.h"
#include "strop.h"
#include "int_stack.h"
#include "regexp.h"

typedef struct {
  enum token_type {
    token_ignore,
    token_null,
    token_void_kw,
    token_bool_kw,
    token_char_kw,
    token_int_kw,
    token_real_kw,
    token_signed_kw,
    token_unsigned_kw,
    token_complex_kw,
    token_string_kw,
    const_int,
    const_real,
    const_complex,
    const_char,
    const_string,
    ident,
    return_kw,
    continue_kw,
    break_kw,
    open_p,
    close_p,
    open_square_bracket,
    close_square_bracket,
    token_dot,
    token_comma,
    token_colon,
    token_semi_colon,
    token_ampersand,
    token_asterisk,
    token_minus,
    not_kw,
    complement,
    token_assign,
    token_plus,
    token_div,
    token_mod,
    token_exp,
    token_log_and,
    token_log_xor,
    token_log_or,
    token_equal,
    token_not_equal,
    token_less,
    token_less_or_equal,
    token_greater,
    token_greater_or_equal,
    token_bit_or,
    token_bit_xor,
    token_bit_shift_left,
    token_bit_shift_right,
    token_inc,
    token_dec,
    token_add_assign,
    token_sub_assign,
    token_mul_assign,
    token_div_assign,
    token_mod_assign,
    token_and_assign,
    token_xor_assign,
    token_or_assign,
    token_bsl_assign,
    token_bsr_assign,
    token_cond_if,
    token_cond_else,
    token_loop_for,
    token_loop_while,
    token_struct,
    token_union,
    token_auto,
    token_register,
    token_static,
    token_extern,
    token_inline,
    token_const,
    token_volatile,
    token_restrict,
  } type;
  char* str;
  int level;
  int line;
} token;


size_t string_count;
char** string_index;

size_t real_count;
double* real_index;

token* make_token(int t, char* str, int level, int line);
void delete_token(token* t);
void lex(const char* filename,
         token** full_token_list,
         unsigned long* total_num_tokens);

#endif

