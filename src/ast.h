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
#include "persistent_vector.h"
#include "atomic_type.h"

typedef struct ast_node {
    enum {
        const_int_tag,
        const_real_tag,
        const_char_tag,
        const_string_tag,
        var_def_tag,
        var_ref_tag,
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
        bit_shift_left_tag,
        bit_shift_right_tag,
        less_tag,
        less_or_equal_tag,
        greater_tag,
        greater_or_equal_tag,
        equal_tag,
        not_equal_tag,
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
    } tag;
    union {
        int int_exp;
        char char_exp;
        struct {atomic_type type;
                char* ident;
                struct ast_node* val;
                int scope;} var_def_exp;
        struct {char* ident;} var_ref_exp;
        struct {char* ident;
            struct ast_node_list* args;} call_exp;
        struct {struct ast_node* val;} ret_exp;
        struct {struct ast_node* ret_exp;
            char* ident;
            struct ast_node_list* args;
            struct ast_node_list* body;
            struct trie_root_node* symbol_map;
            int scope;} fun_def_exp;
        struct {struct ast_node* cond;
            struct ast_node_list* if_body;
			struct ast_node_list* else_body;
            struct trie_root_node* if_symbol_map;
			struct trie_root_node* else_symbol_map;
            int scope;} cond_exp;
        struct {struct ast_node* init;
            struct ast_node* cond;
            struct ast_node* move;
            struct ast_node_list* body;
            struct trie_root_node* symbol_map;
            int scope;} for_loop_exp;
        struct {struct ast_node* cond;
            struct ast_node_list* body;
            struct trie_root_node* symbol_map;
            int scope;} while_loop_exp;
        struct {const char* name;
            struct ast_node_list* prog;
            struct trie_root_node* symbol_map;} prog_exp;
        struct {atomic_type type;
            struct ast_node* expr;} unary_exp;
        struct {atomic_type type;
            struct ast_node* expr_l;
            struct ast_node* expr_r;} binary_exp;
        struct {struct ast_node* lval;
            struct ast_node* val;} assign_exp;
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
asn* make_int_exp(int val);
asn* make_real_exp(int idx);
asn* make_char_exp(char val);
asn* make_string_exp(int idx);
asn* make_call_exp(char* id, asn_list* args);
asn* make_ret_exp(asn* v);
asn* make_fun_def_exp(asn* ret_exp,
                    char* ident,
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
asn* make_var_def_exp(atomic_type type, char* ident, int scope);
asn* make_var_ref_exp(char* ident);
asn* make_prog_exp(const char* name, asn_list* prog);
asn* make_unary_exp(atomic_type at_type, asn* expr, int type);
asn* make_binary_exp(atomic_type at_type, asn* expr_l, asn* expr_r, int type);
asn* make_assign_exp(asn* lhs, asn* val, int assign_type);
asn* make_jump_exp(int type);

atomic_type get_atomic_type(asn* expr, pv_root* symbol_map);
#endif

