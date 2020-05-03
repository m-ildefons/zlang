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

static void _pretty_print_symbol_list(symbol_list* list, int_stack* ws_stack);
static void _pretty_print_binary_node(asn* tree, int level, int_stack* ws_stack);
static void _pretty_print_unary_node(asn* tree, int level, int_stack* ws_stack);
static void _pretty_print_body(asn_list* body, int level, int_stack* ws_stack);
static void _pretty_print_cond(asn* tree, int level, int_stack* ws_stack);
static void _pretty_print_for(asn* tree, int level, int_stack* ws_stack);
static void _pretty_print_while(asn* tree, int level, int_stack* ws_stack);
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

static void _pretty_print_symbol_list(symbol_list* list, int_stack* ws_stack){
    if(list != NULL){
        push_back(&ws_stack, -1);
        _print_tabs(ws_stack);
        printf("\033[90m[ symbol list (%zu) ]\033[39m\n", list->scope);
        pop_back(&ws_stack);

        symbol_list_entry* e = list->top;
        for(; e != NULL; e = e->next){
            push_back(&ws_stack, -1);
            _print_tabs(ws_stack);
            if(e == list->top && e == list->bottom)
                printf("\033[90mt/b ->");
            else if(e == list->top)
                printf("\033[90m t  ->");
            else if(e == list->bottom)
                printf("\033[90m b  ->");
            else
                printf("\033[90m      ");
            print_symbol_list_entry(e);
            printf("\033[39m");
            pop_back(&ws_stack);
        }
    }
}

static void _pretty_print_binary_node(asn* tree, int level, int_stack* ws_stack){
    asn* left = tree->op.binary_exp.lval;
    asn* right = tree->op.binary_exp.rval;
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
    else if(tree->tag == assign_tag){ /* or an assignment */
        push_back(&ws_stack, 1);
        _pretty_print(tree->op.assign_exp.rval, level+1, ws_stack);
        pop_back(&ws_stack);
        push_back(&ws_stack, 2);
        _pretty_print(tree->op.assign_exp.lval, level+1, ws_stack);
        pop_back(&ws_stack);
        return;
    } else if(tree->tag == add_assign_tag)
        next = tree->op.assign_exp.rval;
    else if(tree->tag == sub_assign_tag)
        next = tree->op.assign_exp.rval;
    else if(tree->tag == mul_assign_tag)
        next = tree->op.assign_exp.rval;
    else if(tree->tag == div_assign_tag)
        next = tree->op.assign_exp.rval;
    else if(tree->tag == mod_assign_tag)
        next = tree->op.assign_exp.rval;
    else if(tree->tag == and_assign_tag)
        next = tree->op.assign_exp.rval;
    else if(tree->tag == xor_assign_tag)
        next = tree->op.assign_exp.rval;
    else if(tree->tag == or_assign_tag){
        next = tree->op.assign_exp.rval;
    } else {
        next = tree->op.unary_exp.val;
	}

	if(next == NULL){
		return;
	}

    push_back(&ws_stack, 2);
    _pretty_print(next, level+1, ws_stack);
    pop_back(&ws_stack);
}

static void _pretty_print_body(asn_list* body, int level, int_stack* ws_stack){
    for(; body != NULL; body = body->next){
        if(body->next != NULL)
            push_back(&ws_stack, 1);
        else
            push_back(&ws_stack, 2);
        _pretty_print(body->expr, level+1, ws_stack);
        pop_back(&ws_stack);
    }
}

static void _pretty_print_cond(asn* tree, int level, int_stack* ws_stack){
    asn_list* body = NULL;
    symbol_list* symbols = NULL;
    int else_body = 0;

    printf("[ if ]\n");
    push_back(&ws_stack, -1);
    push_back(&ws_stack, 2);
    _pretty_print(tree->op.cond_exp.cond, level, ws_stack);
    pop_back(&ws_stack);
    pop_back(&ws_stack);


    body = tree->op.cond_exp.if_body;
    symbols = tree->op.cond_exp.if_symbols;
    if(tree->op.cond_exp.else_body != NULL)
        else_body = 1;

    _pretty_print_symbol_list(symbols, ws_stack);

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
        if(tree->op.cond_exp.else_symbols != NULL){
            _pretty_print_symbol_list(tree->op.cond_exp.else_symbols, ws_stack);
        }
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

static void _pretty_print_for(asn* tree, int level, int_stack* ws_stack){
    printf("[ for ]\n");
    push_back(&ws_stack, -1);
    push_back(&ws_stack, 1);
    _pretty_print(tree->op.for_loop_exp.init, level, ws_stack);
    _pretty_print(tree->op.for_loop_exp.cond, level, ws_stack);
    pop_back(&ws_stack);
    push_back(&ws_stack, 2);
    _pretty_print(tree->op.for_loop_exp.move, level, ws_stack);
    pop_back(&ws_stack);
    pop_back(&ws_stack);

    _pretty_print_symbol_list(tree->op.for_loop_exp.symbols, ws_stack);
    _pretty_print_body(tree->op.for_loop_exp.body, level+1, ws_stack);
}

static void _pretty_print_while(asn* tree, int level, int_stack* ws_stack){
    printf("[ while ]\n");
    push_back(&ws_stack, -1);
    push_back(&ws_stack, 2);
    _pretty_print(tree->op.while_loop_exp.cond, level, ws_stack);
    pop_back(&ws_stack);
    pop_back(&ws_stack);

    _pretty_print_symbol_list(tree->op.while_loop_exp.symbols, ws_stack);
    _pretty_print_body(tree->op.while_loop_exp.body, level+1, ws_stack);
}

static void _pretty_print(asn* tree, int level, int_stack* ws_stack){
    int ty;
    _print_tabs(ws_stack);

    switch(tree->tag){
        case ret_tag:
            printf("[ return ]\n");
            /* return statements have one child always - like an unary op. */
            _pretty_print_unary_node(tree, level, ws_stack);
            break;
        case const_int_tag:
            printf("[ integer constant (%d) ]\n", tree->op.int_exp);
            break;
        case const_real_tag:
            ty = tree->op.int_exp;
            printf("[ real constant (%d) %f ]\n", ty, real_index[ty]);
            break;
        case const_char_tag:
            printf("[ char constant (%c) ]\n", tree->op.char_exp);
            break;
        case const_string_tag:
            ty = tree->op.int_exp;
            printf("[ string constant (%d) \"%s\" ]\n", ty, string_index[ty]);
            break;
        case unary_minus_tag:
            printf("[ unary minus ]\n");
            _pretty_print_unary_node(tree, level, ws_stack);
            break;
        case unary_not_tag:
            printf("[ unary not ]\n");
            _pretty_print_unary_node(tree, level, ws_stack);
            break;
        case unary_compl_tag:
            printf("[ unary complement ]\n");
            _pretty_print_unary_node(tree, level, ws_stack);
            break;
        case ref_tag:
            printf("[ reference ]\n");
            _pretty_print_unary_node(tree, level, ws_stack);
            break;
        case deref_tag:
            printf("[ dereference ]\n");
            _pretty_print_unary_node(tree, level, ws_stack);
            break;
        case bin_add_tag:
            printf("[ add ]\n");
            _pretty_print_binary_node(tree, level, ws_stack);
            break;
        case bin_sub_tag:
            printf("[ substract ]\n");
            _pretty_print_binary_node(tree, level, ws_stack);
            break;
        case bin_mul_tag:
            printf("[ multiply ]\n");
            _pretty_print_binary_node(tree, level, ws_stack);
            break;
        case bin_div_tag:
            printf("[ divide ]\n");
            _pretty_print_binary_node(tree, level, ws_stack);
            break;
        case bin_mod_tag:
            printf("[ modulo ]\n");
            _pretty_print_binary_node(tree, level, ws_stack);
            break;
        case log_and_tag:
            printf("[ logical and ]\n");
            _pretty_print_binary_node(tree, level, ws_stack);
            break;
        case log_xor_tag:
            printf("[ logical xor ]\n");
            _pretty_print_binary_node(tree, level, ws_stack);
            break;
        case log_or_tag:
            printf("[ logical or ]\n");
            _pretty_print_binary_node(tree, level, ws_stack);
            break;
        case equal_tag:
            printf("[ equal ]\n");
            _pretty_print_binary_node(tree, level, ws_stack);
            break;
        case not_equal_tag:
            printf("[ not-equal ]\n");
            _pretty_print_binary_node(tree, level, ws_stack);
            break;
        case less_tag:
            printf("[ less ]\n");
            _pretty_print_binary_node(tree, level, ws_stack);
            break;
        case less_or_equal_tag:
            printf("[ less-or-equal ]\n");
            _pretty_print_binary_node(tree, level, ws_stack);
            break;
        case greater_tag:
            printf("[ greater ]\n");
            _pretty_print_binary_node(tree, level, ws_stack);
            break;
        case greater_or_equal_tag:
            printf("[ greater-or-equal ]\n");
            _pretty_print_binary_node(tree, level, ws_stack);
            break;
        case bit_and_tag:
            printf("[ bitwise and ]\n");
            _pretty_print_binary_node(tree, level, ws_stack);
            break;
        case bit_xor_tag:
            printf("[ bitwise xor ]\n");
            _pretty_print_binary_node(tree, level, ws_stack);
            break;
        case bit_or_tag:
            printf("[ bitwise or ]\n");
            _pretty_print_binary_node(tree, level, ws_stack);
            break;
        case member_access_tag:
            printf("[ struct member access ]\n");
            _pretty_print_binary_node(tree, level, ws_stack);
            break;
        case array_access_tag:
            printf("[ array access ]\n");
            _pretty_print_binary_node(tree, level, ws_stack);
            break;
        case var_tag:
            printf("[ variable %s ]\n", tree->op.var_exp.sym->ident);
            break;
        case ptr_tag:
            printf("[ pointer to ]\n");
            if(tree->op.ptr_exp.to != NULL){
                push_back(&ws_stack, 2);
                _pretty_print(tree->op.ptr_exp.to, level, ws_stack);
                pop_back(&ws_stack);
            }
            break;
        case ident_tag:
            printf("[ identifier %s ]\n",
                    tree->op.ident_exp.ident);
            break;
        case assign_tag:
            printf("[ assign ]\n");
            _pretty_print_unary_node(tree, level, ws_stack);
            break;
        case add_assign_tag:
            printf("[ add assign ]\n");
            _pretty_print_unary_node(tree, level, ws_stack);
            break;
        case sub_assign_tag:
            printf("[ sub assign ]\n");
            _pretty_print_unary_node(tree, level, ws_stack);
            break;
        case mul_assign_tag:
            printf("[ mul assign ]\n");
            _pretty_print_unary_node(tree, level, ws_stack);
            break;
        case div_assign_tag:
            printf("[ div assign ]\n");
            _pretty_print_unary_node(tree, level, ws_stack);
            break;
        case mod_assign_tag:
            printf("[ mod assign ]\n");
            _pretty_print_unary_node(tree, level, ws_stack);
            break;
        case and_assign_tag:
            printf("[ and assign ]\n");
            _pretty_print_unary_node(tree, level, ws_stack);
            break;
        case xor_assign_tag:
            printf("[ xor assign ]\n");
            _pretty_print_unary_node(tree, level, ws_stack);
            break;
        case or_assign_tag:
            printf("[ or assign ]\n");
            _pretty_print_unary_node(tree, level, ws_stack);
            break;
        case inc_tag:
            printf("[ increment ]\n");
            _pretty_print_unary_node(tree, level, ws_stack);
            break;
        case dec_tag:
            printf("[ decrement ]\n");
            _pretty_print_unary_node(tree, level, ws_stack);
            break;
        case cast_to_real_tag:
            printf("[ cast to real ]\n");
            _pretty_print_unary_node(tree, level, ws_stack);
            break;
        case cond_tag:
            _pretty_print_cond(tree, level, ws_stack);
            break;
        case for_loop_tag:
            _pretty_print_for(tree, level, ws_stack);
            break;
        case while_loop_tag:
            _pretty_print_while(tree, level, ws_stack);
            break;
        case fun_def_tag:
            printf("[ function %s ]\n",
                    tree->op.fun_def_exp.sym->ident);
            push_back(&ws_stack, -1);
            _pretty_print_body(tree->op.fun_def_exp.args, level, ws_stack);
            pop_back(&ws_stack);
            _pretty_print_symbol_list(tree->op.fun_def_exp.symbols, ws_stack);
            _pretty_print_body(tree->op.fun_def_exp.body, level, ws_stack);
            break;
        case call_tag:
            printf("[ call %s ]\n", tree->op.call_exp.ident);
            _pretty_print_body(tree->op.call_exp.args, level, ws_stack);
            break;
        case prog_tag:
            printf("[ program %s ]\n", tree->op.prog_exp.name);
            _pretty_print_symbol_list(tree->op.prog_exp.symbols, ws_stack);
            _pretty_print_body(tree->op.prog_exp.prog, level, ws_stack);
            break;
        case struct_tag:
            printf("[ struct (%s, %d) ]\n",
                    tree->op.struct_exp.ident,
                    tree->op.struct_exp.size);
            _pretty_print_symbol_list(tree->op.struct_exp.symbols, ws_stack);
            _pretty_print_body(tree->op.struct_exp.body, level, ws_stack);
            break;
        case union_tag:
            printf("[ union (%s, %d) ]\n",
                    tree->op.struct_exp.ident,
                    tree->op.struct_exp.size);
            _pretty_print_symbol_list(tree->op.struct_exp.symbols, ws_stack);
            _pretty_print_body(tree->op.struct_exp.body, level, ws_stack);
            break;
        default:
            printf("< ! unrecognized ! >\n");
            break;
    }
}

void pretty_print(asn* tree){
    int_stack* ws_stack = malloc(sizeof(int_stack));
    assert(ws_stack != NULL);
    ws_stack->next = NULL;
    ws_stack->val = 0;
    _pretty_print(tree, 0, ws_stack);
    free(ws_stack);
    ws_stack = NULL;
}


