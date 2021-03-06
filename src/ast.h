/*
 * FILENAME: ast.h
 *
 * DESCRIPTION:
 *     Declaration of structures and functions for an abstract syntax tree.
 *
 * AUTHOR: Moritz Röhrich <moritzr@pool.math.tu-berlin.de>
 *
 */

#ifndef __AST_H__
#define __AST_H__

#include <assert.h>

#include "lex.h"
#include "strop.h"
#include "int_stack.h"
#include "symbol_list.h"

typedef struct ast_node {
    enum {
        const_int_tag,
        const_real_tag,
        const_char_tag,
        const_string_tag,
        type_tag,
        var_tag,
        ptr_tag,
        ident_tag,
        call_tag,
        ret_tag,
		break_tag,
		continue_tag,
        fun_def_tag,
        cond_tag,
        prog_tag,
        assign_tag,
        unary_minus_tag,
        unary_not_tag,
        unary_compl_tag,
        ref_tag,
        deref_tag,
        bin_add_tag,
        bin_sub_tag,
        bin_mul_tag,
        bin_div_tag,
        bin_mod_tag,
        less_tag,
        less_or_equal_tag,
        greater_tag,
        greater_or_equal_tag,
        equal_tag,
        not_equal_tag,
        bit_shift_left_tag,
        bit_shift_right_tag,
        bit_and_tag,
        bit_xor_tag,
        bit_or_tag,
        log_and_tag,
        log_xor_tag,
        log_or_tag,
        inc_tag,
        dec_tag,
        add_assign_tag,
        sub_assign_tag,
        mul_assign_tag,
        div_assign_tag,
        mod_assign_tag,
        and_assign_tag,
        xor_assign_tag,
        or_assign_tag,
        bsl_assign_tag,
        bsr_assign_tag,
        for_loop_tag,
        while_loop_tag,
        cast_to_real_tag,
        struct_tag,
        union_tag,
        member_access_tag,
        array_access_tag,
    } tag;
    union {
        int int_exp;

        char char_exp;

        struct {
            struct ast_node* to; // pointer to
        } ptr_exp;

        struct {
            symbol* sym;
        } var_exp;

        struct {
            char* ident;
        } ident_exp;

        struct {
            char* ident;
            struct ast_node_list* args;
        } call_exp;

        struct {
            struct ast_node* val;
        } ret_exp;

        struct {
            symbol* sym;
            struct ast_node_list* args;
            struct ast_node_list* body;
            struct s_symbol_list_t* symbols;
            int scope;
        } fun_def_exp;

        struct {
            struct ast_node* cond;
            struct ast_node_list* if_body;
			struct ast_node_list* else_body;
            struct s_symbol_list_t* if_symbols;
            struct s_symbol_list_t* else_symbols;
            int scope;
        } cond_exp;

        struct {
            struct ast_node* init;
            struct ast_node* cond;
            struct ast_node* move;
            struct ast_node_list* body;
            struct s_symbol_list_t* symbols;
            int scope;
        } for_loop_exp;

        struct {
            struct ast_node* cond;
            struct ast_node_list* body;
            struct s_symbol_list_t* symbols;
            int scope;
        } while_loop_exp;

        struct {
            char* name;
            struct ast_node_list* prog;
            struct s_symbol_list_t* symbols;
        } prog_exp;

        struct {
            struct ast_node* val;
        } unary_exp;

        struct {
            struct ast_node* lval;
            struct ast_node* rval;
        } binary_exp;

        struct {
            struct ast_node* lval;
            struct ast_node* rval;
        } assign_exp;

        struct {
            char* ident;
            int size;
            struct ast_node_list* body;
            struct s_symbol_list_t* symbols;
            int scope;
        } struct_exp;
    } op;
} asn;

typedef struct ast_node_list {
    asn* expr;
    struct ast_node_list* next;
} asn_list;

/* implemented in ast_print.c */
void _print_tabs(int_stack* ws_stack);
void pretty_print(asn* tree);

/* implemented in ast.c */
void append_exp_list(asn_list** list, asn* e);
asn* make_exp(void);
asn* make_int_exp(int val);
asn* make_real_exp(int idx);
asn* make_char_exp(char val);
asn* make_string_exp(int idx);
asn* make_call_exp(char* id, asn_list* args);
asn* make_ret_exp(asn* v);
asn* make_fun_def_exp(symbol* sym,
                    asn_list* args,
                    asn_list* body,
                    int scope);
asn* make_cond_exp(asn* cond,
				asn_list* if_body,
				asn_list* else_body,
				int scope);
asn* make_for_exp(asn* init,
                asn* cond,
                asn* move,
                asn_list* body,
                int scope);
asn* make_while_exp(asn* cond, asn_list* body, int scope);
asn* make_var_exp(symbol* sym);
asn* make_ptr_exp(asn* to);
asn* make_ident_exp(char* ident);
asn* make_prog_exp(const char* name);
asn* make_unary_exp(asn* expr, int type);
asn* make_binary_exp(asn* expr_l, asn* expr_r, int type);
asn* make_assign_exp(asn* lhs, asn* val, int assign_type);
asn* make_jump_exp(int type);
asn* make_cast_to_real(asn* val);
asn* make_struct_exp(int tag, char* id, int scope);

/* implemented in ast_del.c */
void delete_exp(asn* e);
void delete_asn_list(asn_list* l);
void delete_var_exp(asn* e);
void delete_ptr_exp(asn* e);
void delete_fun_def_exp(asn* e);
void delete_call_exp(asn* e);
void delete_return_exp(asn* e);
void delete_cond_exp(asn* e);
void delete_while_loop_exp(asn* e);
void delete_for_loop_exp(asn* e);
void delete_unary_exp(asn* e);
void delete_binary_exp(asn* e);
void delete_assign_exp(asn* e);
void delete_prog_exp(asn* e);
void delete_struct_exp(asn* e);

#endif

