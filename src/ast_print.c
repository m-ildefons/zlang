/*
 * FILENAME: ast_print.c
 *
 * DESCRIPTION:
 *     Implementation of functions for pretty-printing an abstract syntax tree.
 *
 * AUTHOR: Moritz Röhrich <moritzr@pool.math.tu-berlin.de>
 *
 */

#include "ast.h"

static void _pretty_print_binary_node(asn* tree, int level, int_stack* ws_stack);
static void _pretty_print_unary_node(asn* tree, int level, int_stack* ws_stack);
static void _pretty_print_body(asn* tree, int level, int_stack* ws_stack);
static void _pretty_print(asn* tree, int level, int_stack* ws_stack);

void _print_tabs(int_stack* ws_stack){
    int_stack* sp;
    for(sp = ws_stack; sp->next != NULL; sp = sp->next){
        if(sp->val == 1)
            printf(" \u2502");
        else if(sp->val == -1)
            printf(" \u2502");
        else
            printf("  ");
    }

    if(sp == ws_stack)
        return;

    if(sp->val == 1)
        printf(" \u251c\u2500");
    else if(sp->val == 2)
        printf(" \u2570\u2500");
    else if(sp->val == -1)
        printf(" \u2502 ");
    else
        printf("   ");
}

static void _pretty_print_binary_node(asn* tree, int level, int_stack* ws_stack){
    asn* left = tree->op.binary_exp.expr_l;
    asn* right = tree->op.binary_exp.expr_r;
    push_back(&ws_stack, 1);
    _pretty_print(left, level+1, ws_stack);
    pop_back(&ws_stack);
    push_back(&ws_stack, 2);
    _pretty_print(right, level+1, ws_stack);
    pop_back(&ws_stack);
}

static void _pretty_print_unary_node(asn* tree, int level, int_stack* ws_stack){
    asn* next = NULL;
    /* we might be dealing with a return statement */
    if(tree->tag == ret_tag)
        next = tree->op.ret_exp.val;
    else if(tree->tag == var_def_tag) /* or a variable definition */
        next = tree->op.var_def_exp.val;
    else if(tree->tag == assign_tag){ /* or an assignment */
        push_back(&ws_stack, 1);
        _pretty_print(tree->op.assign_exp.val, level+1, ws_stack);
        pop_back(&ws_stack);
        push_back(&ws_stack, 2);
        _pretty_print(tree->op.assign_exp.lval, level+1, ws_stack);
        pop_back(&ws_stack);
        return;
    } else if(tree->tag == add_assign_tag)
        next = tree->op.assign_exp.val;
    else if(tree->tag == sub_assign_tag)
        next = tree->op.assign_exp.val;
    else if(tree->tag == mul_assign_tag)
        next = tree->op.assign_exp.val;
    else if(tree->tag == div_assign_tag)
        next = tree->op.assign_exp.val;
    else if(tree->tag == mod_assign_tag)
        next = tree->op.assign_exp.val;
    else if(tree->tag == and_assign_tag)
        next = tree->op.assign_exp.val;
    else if(tree->tag == xor_assign_tag)
        next = tree->op.assign_exp.val;
    else if(tree->tag == or_assign_tag)
        next = tree->op.assign_exp.val;
    else
        next = tree->op.unary_exp.expr;
    push_back(&ws_stack, 2);
    _pretty_print(next, level+1, ws_stack);
    pop_back(&ws_stack);
}

static void _pretty_print_body(asn* tree, int level, int_stack* ws_stack){
    asn_list* body = NULL;
    pv_root* symbol_map = NULL;
    ca_list* key_list = NULL;
    int else_body = 0;

    switch(tree->tag){
        case prog_tag:
            body = tree->op.prog_exp.prog;
            symbol_map = tree->op.prog_exp.symbol_map;
            break;
        case fun_def_tag:
            body = tree->op.fun_def_exp.body;
            symbol_map = tree->op.fun_def_exp.symbol_map;
            break;
        case cond_tag:
            body = tree->op.cond_exp.if_body;
            symbol_map = tree->op.cond_exp.if_symbol_map;
            if(tree->op.cond_exp.else_body != NULL)
                else_body = 1;
            break;
        case for_loop_tag:
            body = tree->op.for_loop_exp.body;
            symbol_map = tree->op.for_loop_exp.symbol_map;
            break;
        case while_loop_tag:
            body = tree->op.while_loop_exp.body;
            symbol_map = tree->op.while_loop_exp.symbol_map;
            break;
        case call_tag:
            body = tree->op.call_exp.args;
            symbol_map = NULL;
            break;
        default: abort();
    }

    if(symbol_map != NULL){
	    key_list = symbol_map->key_list;

        push_back(&ws_stack, -1);
        _print_tabs(ws_stack); printf("[ symbol map (%d) ]\n", symbol_map->scope);
        pop_back(&ws_stack);

        for(; key_list != NULL; key_list = key_list->next){
            pv_leaf* leaf = pv_search(symbol_map, key_list->key);
            if(key_list->next != NULL || body != NULL) {
                push_back(&ws_stack, -1);
            } else {
                push_back(&ws_stack, 2);
            }
            if(key_list->next != NULL)
                push_back(&ws_stack, 1);
            else
                push_back(&ws_stack, 2);
            _print_tabs(ws_stack);
            printf("[ %d %s (%s) ]\n",
                leaf->scope,
                leaf->ident,
                atomic_type_cn[leaf->type]);
            pop_back(&ws_stack);
            pop_back(&ws_stack);
        }
    }
    for(; body != NULL; body = body->next){
        if(body->next != NULL || else_body != 0)
            push_back(&ws_stack, 1);
        else
            push_back(&ws_stack, 2);
        _pretty_print(body->expr, level+1, ws_stack);
        pop_back(&ws_stack);
    }

    if(else_body != 0){
        body = tree->op.cond_exp.else_body;
        int tmp_val = pop_back(&ws_stack);
        push_back(&ws_stack, 0);
        _print_tabs(ws_stack); printf("[ else ]\n");
        pop_back(&ws_stack);
        push_back(&ws_stack, tmp_val);
        for(; body != NULL; body = body->next){
            if(body->next != NULL)
                push_back(&ws_stack, 1);
            else
                push_back(&ws_stack, 2);
            _pretty_print(body->expr, level+1, ws_stack);
            pop_back(&ws_stack);
        }
    }
}

static void _pretty_print(asn* tree, int level, int_stack* ws_stack){
    int ty;
    _print_tabs(ws_stack);

    asn_list* body = NULL;
    switch(tree->tag){
        case ret_tag:
            printf("[ return node ]\n");
            /* return statements have one child always - like an unary op. */
            _pretty_print_unary_node(tree, level, ws_stack);
            break;
        case const_int_tag:
            printf("[ integer node (%d) ]\n", tree->op.int_exp);
            break;
        case const_real_tag:
            ty = tree->op.int_exp;
            printf("[ real node (%d) %f ]\n", ty, real_index[ty]);
            break;
        case const_char_tag:
            printf("[ char node (%c) ]\n", tree->op.char_exp);
            break;
        case const_string_tag:
            ty = tree->op.int_exp;
            printf("[ string node (%d) \"%s\" ]\n", ty, string_index[ty]);
            break;
        case unary_minus_tag:
            ty = tree->op.unary_exp.type;
            printf("[ unary minus node (%s) ]\n", atomic_type_cn[ty]);
            _pretty_print_unary_node(tree, level, ws_stack);
            break;
        case unary_not_tag:
            ty = tree->op.unary_exp.type;
            printf("[ unary not node (%s) ]\n", atomic_type_cn[ty]);
            _pretty_print_unary_node(tree, level, ws_stack);
            break;
        case unary_compl_tag:
            ty = tree->op.unary_exp.type;
            printf("[ unary complement node (%s) ]\n", atomic_type_cn[ty]);
            _pretty_print_unary_node(tree, level, ws_stack);
            break;
        case ref_tag:
            ty = tree->op.unary_exp.type;
            printf("[ reference node (%s) ]\n", atomic_type_cn[ty]);
            _pretty_print_unary_node(tree, level, ws_stack);
            break;
        case deref_tag:
            ty = tree->op.unary_exp.type;
            printf("[ dereference node (%s) ]\n", atomic_type_cn[ty]);
            _pretty_print_unary_node(tree, level, ws_stack);
            break;
        case bin_add_tag:
            ty = tree->op.unary_exp.type;
            printf("[ binary add node (%s) ]\n", atomic_type_cn[ty]);
            _pretty_print_binary_node(tree, level, ws_stack);
            break;
        case bin_sub_tag:
            ty = tree->op.unary_exp.type;
            printf("[ binary sub node (%s) ]\n", atomic_type_cn[ty]);
            _pretty_print_binary_node(tree, level, ws_stack);
            break;
        case bin_mul_tag:
            ty = tree->op.unary_exp.type;
            printf("[ binary mult node (%s) ]\n", atomic_type_cn[ty]);
            _pretty_print_binary_node(tree, level, ws_stack);
            break;
        case bin_div_tag:
            ty = tree->op.unary_exp.type;
            printf("[ binary div node (%s) ]\n", atomic_type_cn[ty]);
            _pretty_print_binary_node(tree, level, ws_stack);
            break;
        case bin_mod_tag:
            ty = tree->op.unary_exp.type;
            printf("[ binary mod node (%s) ]\n", atomic_type_cn[ty]);
            _pretty_print_binary_node(tree, level, ws_stack);
            break;
        case log_and_tag:
            ty = tree->op.unary_exp.type;
            printf("[ logical and node (%s) ]\n", atomic_type_cn[ty]);
            _pretty_print_binary_node(tree, level, ws_stack);
            break;
        case log_xor_tag:
            ty = tree->op.unary_exp.type;
            printf("[ logical xor node (%s) ]\n", atomic_type_cn[ty]);
            _pretty_print_binary_node(tree, level, ws_stack);
            break;
        case log_or_tag:
            ty = tree->op.unary_exp.type;
            printf("[ logical or node (%s) ]\n", atomic_type_cn[ty]);
            _pretty_print_binary_node(tree, level, ws_stack);
            break;
        case equal_tag:
            printf("[ equal node ]\n");
            _pretty_print_binary_node(tree, level, ws_stack);
            break;
        case not_equal_tag:
            printf("[ not-equal node ]\n");
            _pretty_print_binary_node(tree, level, ws_stack);
            break;
        case less_tag:
            printf("[ less node ]\n");
            _pretty_print_binary_node(tree, level, ws_stack);
            break;
        case less_or_equal_tag:
            printf("[ less-or-equal node ]\n");
            _pretty_print_binary_node(tree, level, ws_stack);
            break;
        case greater_tag:
            printf("[ greater node ]\n");
            _pretty_print_binary_node(tree, level, ws_stack);
            break;
        case greater_or_equal_tag:
            printf("[ greater-or-equal node ]\n");
            _pretty_print_binary_node(tree, level, ws_stack);
            break;
        case bit_and_tag:
            printf("[ bitwise and node ]\n");
            _pretty_print_binary_node(tree, level, ws_stack);
            break;
        case bit_xor_tag:
            printf("[ bitwise xor node ]\n");
            _pretty_print_binary_node(tree, level, ws_stack);
            break;
        case bit_or_tag:
            printf("[ bitwise or node ]\n");
            _pretty_print_binary_node(tree, level, ws_stack);
            break;
        case var_def_tag:
            ty = tree->op.var_def_exp.type;
            printf("[ variable definition (%s %s) ]\n",
                    atomic_type_cn[ty],
                    tree->op.var_def_exp.ident);
            if(tree->op.var_def_exp.val != NULL)
                _pretty_print_unary_node(tree, level, ws_stack);
            break;
        case var_ref_tag:
            printf("[ variable reference (%s) ]\n",
                    tree->op.var_ref_exp.ident);
            break;
        case assign_tag:
            printf("[ assign node ]\n");
            _pretty_print_unary_node(tree, level, ws_stack);
            break;
        case add_assign_tag:
            printf("[ add assign node ]\n");
            _pretty_print_unary_node(tree, level, ws_stack);
            break;
        case sub_assign_tag:
            printf("[ sub assign node ]\n");
            _pretty_print_unary_node(tree, level, ws_stack);
            break;
        case mul_assign_tag:
            printf("[ mul assign node ]\n");
            _pretty_print_unary_node(tree, level, ws_stack);
            break;
        case div_assign_tag:
            printf("[ div assign node ]\n");
            _pretty_print_unary_node(tree, level, ws_stack);
            break;
        case mod_assign_tag:
            printf("[ mod assign node ]\n");
            _pretty_print_unary_node(tree, level, ws_stack);
            break;
        case and_assign_tag:
            printf("[ and assign node ]\n");
            _pretty_print_unary_node(tree, level, ws_stack);
            break;
        case xor_assign_tag:
            printf("[ xor assign node ]\n");
            _pretty_print_unary_node(tree, level, ws_stack);
            break;
        case or_assign_tag:
            printf("[ or assign node ]\n");
            _pretty_print_unary_node(tree, level, ws_stack);
            break;
        case inc_tag:
            printf("[ inc node ]\n");
            _pretty_print_unary_node(tree, level, ws_stack);
            break;
        case dec_tag:
            printf("[ dec node ]\n");
            _pretty_print_unary_node(tree, level, ws_stack);
            break;
        case cast_to_real_tag:
            printf("[ cast to real ]\n");
            _pretty_print_unary_node(tree, level, ws_stack);
            break;
        case cond_tag:
            printf("[ condition node ]\n");
            _pretty_print_body(tree, level, ws_stack);
            break;
        case for_loop_tag:
            printf("[ for loop ]\n");
            _pretty_print_body(tree, level, ws_stack);
            break;
        case while_loop_tag:
            printf("[ while loop ]\n");
            _pretty_print_body(tree, level, ws_stack);
            break;
        case fun_def_tag:
            printf("[ function def node (%s %s) ]\n",
                    atomic_type_cn[tree->op.fun_def_exp.type],
                    tree->op.fun_def_exp.ident);
            _pretty_print_body(tree, level, ws_stack);
            break;
        case call_tag:
            printf("[ function call (%s) ]\n", tree->op.call_exp.ident);
            _pretty_print_body(tree, level, ws_stack);
            break;
        case prog_tag:
            printf("[ program node (%s) ]\n", tree->op.prog_exp.name);
            _pretty_print_body(tree, level, ws_stack);
            break;
        default:
            printf("< ! unrecognized node ! >\n");
            break;
    }
}

void pretty_print(asn* tree){
    int_stack* ws_stack = (int_stack*) malloc(sizeof(int_stack));
    assert(ws_stack != NULL);
    ws_stack->next = NULL;
    ws_stack->val = 0;
    _pretty_print(tree, 0, ws_stack);
    free(ws_stack);
    ws_stack = NULL;
}


