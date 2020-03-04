/*
 * FILENAME: ast.c
 *
 * DESCRIPTION:
 *     Implementation of functions for an abstract syntax tree.
 *
 * AUTHOR: Moritz Röhrich <moritzr@pool.math.tu-berlin.de>
 *
 */

#include "ast.h"

void append_exp_list(asn_list** list, asn* e){
	if((*list) == NULL){
		(*list) = (asn_list*) malloc(sizeof(asn_list));
        assert((*list) != NULL);
		(*list)->expr = e;
		(*list)->next = NULL;
		return;
	}

	asn_list* lp = (*list);
    while(lp->next)
        lp = lp->next;

    asn_list* el = (asn_list*) malloc(sizeof(asn_list));
    assert(el != NULL);
    el->expr = e;
    el->next = NULL;
    lp->next = el;
}

asn* make_int_exp(int val){
    asn* e = (asn*) malloc(sizeof(asn));
    assert(e != NULL);
    e->tag = const_int_tag;
    e->op.int_exp = val;
    return e;
}

asn* make_real_exp(int idx){
    asn* e = (asn*) malloc(sizeof(asn));
    assert(e != NULL);
    e->tag = const_real_tag;
    e->op.int_exp = idx;
    return e;
}

asn* make_char_exp(char val){
    asn* e = (asn*) malloc(sizeof(asn));
    assert(e != NULL);
    e->tag = const_char_tag;
    e->op.char_exp = val;
    return e;
}

asn* make_string_exp(int idx){
    asn* e = (asn*) malloc(sizeof(asn));
    assert(e != NULL);
    e->tag = const_string_tag;
    e->op.int_exp = idx;
    return e;
}

asn* make_call_exp(char* id, asn_list* args){
    asn* e = (asn*) malloc(sizeof(asn));
    assert(e != NULL);
    e->tag = call_tag;
    e->op.call_exp.ident = id;
    e->op.call_exp.args = args;
    return e;
}

asn* make_ret_exp(asn* v){
    asn* e = (asn*) malloc(sizeof(asn));
    assert(e != NULL);
    e->tag = ret_tag;
    e->op.ret_exp.val = v;
    return e;
}

asn* make_fun_def_exp(atomic_type type,
					char* id,
					asn_list* args,
					asn_list* body,
					int scope){
    asn* e = (asn*) malloc(sizeof(asn));
    assert(e != NULL);
    e->tag = fun_def_tag;
    e->op.fun_def_exp.type = type;
    e->op.fun_def_exp.ident = id;
    e->op.fun_def_exp.args = args;
    e->op.fun_def_exp.body = body;
    e->op.fun_def_exp.scope = scope;
    return e;
}

asn* make_cond_exp(asn* cond,
				asn_list* if_body,
				asn_list* else_body,
				int scope){
    asn* e = (asn*) malloc(sizeof(asn));
    assert(e != NULL);
    e->tag = cond_tag;
    e->op.cond_exp.cond = cond;
    e->op.cond_exp.if_body = if_body;
	e->op.cond_exp.else_body = else_body;
    e->op.cond_exp.scope = scope;
    return e;
}

asn* make_for_exp(asn* init,
                asn* cond,
                asn* move,
                asn_list* body,
                int scope){
    asn* e = (asn*) malloc(sizeof(asn));
    assert(e != NULL);
    e->tag = for_loop_tag;
    e->op.for_loop_exp.init = init;
    e->op.for_loop_exp.cond = cond;
    e->op.for_loop_exp.move = move;
    e->op.for_loop_exp.body = body;
    e->op.for_loop_exp.scope = scope;
    return e;
}

asn* make_while_exp(asn* cond, asn_list* body, int scope){
    asn* e = (asn*) malloc(sizeof(asn));
    assert(e != NULL);

    e->tag = while_loop_tag;
    e->op.while_loop_exp.cond = cond;
    e->op.while_loop_exp.body = body;
    e->op.while_loop_exp.scope = scope;
    return e;
}

asn* make_var_def_exp(atomic_type type,
                    char* id,
                    int scope){
    asn* e = (asn*) malloc(sizeof(asn));
    assert(e != NULL);

    e->tag = var_def_tag;
    e->op.var_def_exp.type = type;
    e->op.var_def_exp.ident = id;
    e->op.var_def_exp.val = NULL;
    e->op.var_def_exp.scope = scope;
    return e;
}

asn* make_var_ref_exp(char* id){
    asn* e = (asn*) malloc(sizeof(asn));
    assert(e != NULL);

    e->tag = var_ref_tag;
    e->op.var_ref_exp.ident = id;
    return e;
}

asn* make_prog_exp(const char* name, asn_list* prog){
    asn* e = (asn*) malloc(sizeof(asn));
    assert(e != NULL);

    e->tag = prog_tag;
    e->op.prog_exp.name = name;
    e->op.prog_exp.prog = prog;
    e->op.prog_exp.symbol_map = new_trie();
    return e;
}

asn* make_unary_exp(atomic_type at_type, asn* expr, int type){
    asn* e = (asn*) malloc(sizeof(asn));
    assert(e != NULL);

    switch(type){
        case token_minus: e->tag = unary_minus_tag; break;
        case not_kw: e->tag = unary_not_tag; break;
        case complement: e->tag = unary_compl_tag; break;
        case token_inc: e->tag = inc_tag; break;
        case token_dec: e->tag = dec_tag; break;
        case token_ampersand: e->tag = ref_tag; break;
        case token_asterisk: e->tag = deref_tag; break;
        default: abort();
    }
    e->op.unary_exp.type = at_type;
    e->op.unary_exp.expr = expr;
    return e;
}

asn* make_binary_exp(atomic_type at_type, asn* expr_l, asn* expr_r, int type){
    asn* e = (asn*) malloc(sizeof(asn));
    assert(e != NULL);
    switch(type){
        case token_minus: e->tag = bin_sub_tag; break;
        case token_plus: e->tag = bin_add_tag; break;
        case token_asterisk: e->tag = bin_mul_tag; break;
        case token_div: e->tag = bin_div_tag; break;
        case token_mod: e->tag = bin_mod_tag; break;
        case token_log_and: e->tag = log_and_tag; break;
        case token_log_xor: e->tag = log_xor_tag; break;
        case token_log_or: e->tag = log_or_tag; break;
        case token_equal: e->tag = equal_tag; break;
        case token_not_equal: e->tag = not_equal_tag; break;
        case token_less: e->tag = less_tag; break;
        case token_less_or_equal: e->tag = less_or_equal_tag; break;
        case token_greater: e->tag = greater_tag; break;
        case token_greater_or_equal: e->tag = greater_or_equal_tag; break;
        case token_ampersand: e->tag = bit_and_tag; break;
        case token_bit_or: e->tag = bit_or_tag; break;
        case token_bit_xor: e->tag = bit_xor_tag; break;
        case token_bit_shift_left: e->tag = bit_shift_left_tag; break;
        case token_bit_shift_right: e->tag = bit_shift_right_tag; break;
        default:
            abort();
    }
    e->op.binary_exp.type = at_type;
    e->op.binary_exp.expr_l = expr_l;
    e->op.binary_exp.expr_r = expr_r;
    return e;
}

asn* make_assign_exp(asn* lhs, asn* val, int assign_type){
    asn* e = (asn*) malloc(sizeof(asn));
    assert(e != NULL);

    switch(assign_type){
        case token_assign: e->tag = assign_tag; break;
        case token_inc: e->tag = inc_tag; break;
        case token_dec: e->tag = dec_tag; break;
        case token_add_assign: e->tag = add_assign_tag; break;
        case token_sub_assign: e->tag = sub_assign_tag; break;
        case token_mul_assign: e->tag = mul_assign_tag; break;
        case token_div_assign: e->tag = div_assign_tag; break;
        case token_mod_assign: e->tag = mod_assign_tag; break;
        case token_and_assign: e->tag = and_assign_tag; break;
        case token_xor_assign: e->tag = xor_assign_tag; break;
        case token_or_assign: e->tag = or_assign_tag; break;
        case token_bsl_assign: e->tag = bsl_assign_tag; break;
        case token_bsr_assign: e->tag = bsr_assign_tag; break;
        default:
            abort();
    }
    e->op.assign_exp.lval = lhs;
    e->op.assign_exp.val = val;
    return e;
}

asn* make_jump_exp(int type){
	asn* e = (asn*) malloc(sizeof(asn));
    assert(e != NULL);

	switch(type){
		case break_kw: e->tag = break_tag; break;
		case continue_kw: e->tag = continue_tag; break;
		default: abort();
	}
	return e;
}

asn* make_cast_to_real(asn* val){
	asn* e = (asn*) malloc(sizeof(asn));
    assert(e != NULL);
    e->tag = cast_to_real_tag;
    e->op.unary_exp.type = at_real;
    e->op.unary_exp.expr = val;
	return e;
}

atomic_type get_atomic_type(asn* expr, pv_root* symbol_map){
    pv_leaf* leaf;
    if(expr == NULL)
        return at_void;
    switch(expr->tag){
        case const_int_tag: return at_int;
        case const_real_tag: return at_real;
        case const_char_tag: return at_char;
        case const_string_tag: return at_char_ptr;
        case unary_minus_tag: return expr->op.unary_exp.type;
        case unary_not_tag: return expr->op.unary_exp.type;
        case unary_compl_tag: return expr->op.unary_exp.type;
        case inc_tag: return expr->op.unary_exp.type;
        case dec_tag: return expr->op.unary_exp.type;

        case bin_add_tag: return expr->op.binary_exp.type;
        case bin_sub_tag: return expr->op.binary_exp.type;
        case bin_mul_tag: return expr->op.binary_exp.type;
        case bin_div_tag: return expr->op.binary_exp.type;
        case bin_mod_tag: return expr->op.binary_exp.type;
        case log_and_tag: return expr->op.binary_exp.type;
        case log_xor_tag: return expr->op.binary_exp.type;
        case log_or_tag: return expr->op.binary_exp.type;

        case var_def_tag: return expr->op.var_def_exp.type;
        case var_ref_tag:
            leaf = pv_search(symbol_map, expr->op.var_ref_exp.ident);
            if(leaf == NULL)
                return at_void;
            return leaf->type;
        default: return at_void;
    }
}

