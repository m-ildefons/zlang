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
		(*list) = malloc(sizeof(asn_list));
        assert((*list) != NULL);
		(*list)->expr = e;
		(*list)->next = NULL;
		return;
	}

	asn_list* lp = (*list);
    while(lp->next)
        lp = lp->next;

    asn_list* el = malloc(sizeof(asn_list));
    assert(el != NULL);
    el->expr = e;
    el->next = NULL;
    lp->next = el;
}

asn* make_exp(){
    asn* e = malloc(sizeof(asn));
    assert(e != NULL);
    return e;
}

asn* make_int_exp(int val){
    asn* e = make_exp();
    e->tag = const_int_tag;
    e->op.int_exp = val;
    return e;
}

asn* make_real_exp(int idx){
    asn* e = make_exp();
    e->tag = const_real_tag;
    e->op.int_exp = idx;
    return e;
}

asn* make_char_exp(char val){
    asn* e = make_exp();
    e->tag = const_char_tag;
    e->op.char_exp = val;
    return e;
}

asn* make_string_exp(int idx){
    asn* e = make_exp();
    e->tag = const_string_tag;
    e->op.int_exp = idx;
    return e;
}

asn* make_call_exp(char* id, asn_list* args){
    asn* e = make_exp();
    e->tag = call_tag;
    e->op.call_exp.ident = id;
    e->op.call_exp.args = args;
    return e;
}

asn* make_ret_exp(asn* v){
    asn* e = make_exp();
    e->tag = ret_tag;
    e->op.ret_exp.val = v;
    return e;
}

asn* make_fun_def_exp(symbol* sym,
					asn_list* args,
					asn_list* body,
					int scope){
    asn* e = make_exp();
    e->tag = fun_def_tag;
    e->op.fun_def_exp.sym = sym;
    e->op.fun_def_exp.args = args;
    e->op.fun_def_exp.body = body;
    e->op.fun_def_exp.scope = scope;
    return e;
}

asn* make_cond_exp(asn* cond,
				asn_list* if_body,
				asn_list* else_body,
				int scope){
    asn* e = make_exp();
    e->tag = cond_tag;
    e->op.cond_exp.cond = cond;
    e->op.cond_exp.if_body = if_body;
    e->op.cond_exp.if_symbols = NULL;
	e->op.cond_exp.else_body = else_body;
    e->op.cond_exp.else_symbols = NULL;
    e->op.cond_exp.scope = scope;
    return e;
}

asn* make_for_exp(asn* init,
                asn* cond,
                asn* move,
                asn_list* body,
                int scope){
    asn* e = make_exp();
    e->tag = for_loop_tag;
    e->op.for_loop_exp.init = init;
    e->op.for_loop_exp.cond = cond;
    e->op.for_loop_exp.move = move;
    e->op.for_loop_exp.body = body;
    e->op.for_loop_exp.symbols = NULL;
    e->op.for_loop_exp.scope = scope;
    return e;
}

asn* make_while_exp(asn* cond, asn_list* body, int scope){
    asn* e = make_exp();
    e->tag = while_loop_tag;
    e->op.while_loop_exp.cond = cond;
    e->op.while_loop_exp.body = body;
    e->op.while_loop_exp.symbols = NULL;
    e->op.while_loop_exp.scope = scope;
    return e;
}

asn* make_var_exp(symbol* sym){
    asn* e = make_exp();
    e->tag = var_tag;
    e->op.var_exp.sym = sym;
    e->op.var_exp.sym->ref_count++;
    return e;
}

asn* make_ptr_exp(asn* to){
    asn* e = make_exp();
    e->tag = ptr_tag;
    e->op.ptr_exp.to = to;
    return e;
}

asn* make_ident_exp(char* id){
    asn* e = make_exp();
    e->tag = ident_tag;
    e->op.ident_exp.ident = id;
    return e;
}

asn* make_prog_exp(const char* name){
    asn* e = make_exp();
    e->tag = prog_tag;
    e->op.prog_exp.name = strdup(name);
    e->op.prog_exp.prog = NULL;
    e->op.prog_exp.symbols = new_symbol_list(0);
    return e;
}

asn* make_unary_exp(asn* expr, int type){
    asn* e = make_exp();

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
    e->op.unary_exp.val = expr;
    return e;
}

asn* make_binary_exp(asn* expr_l, asn* expr_r, int type){
    asn* e = make_exp();

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
        case token_dot: e->tag = member_access_tag; break;
        case open_square_bracket: e->tag = array_access_tag; break;
        default:
            abort();
    }
    e->op.binary_exp.lval = expr_l;
    e->op.binary_exp.rval = expr_r;
    return e;
}

asn* make_assign_exp(asn* lhs, asn* val, int assign_type){
    asn* e = make_exp();

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
    e->op.assign_exp.rval = val;
    return e;
}

asn* make_jump_exp(int type){
	asn* e = make_exp();

	switch(type){
		case break_kw: e->tag = break_tag; break;
		case continue_kw: e->tag = continue_tag; break;
		default: abort();
	}
	return e;
}

asn* make_cast_to_real(asn* val){
	asn* e = make_exp();
    e->tag = cast_to_real_tag;
    e->op.unary_exp.val = val;
	return e;
}

asn* make_struct_exp(int tag, char* id, int scope){
	asn* e = make_exp();
    if(tag != struct_tag && tag != union_tag)
        abort();

    e->tag = tag;
    e->op.struct_exp.ident = id;
    e->op.struct_exp.body = NULL;
    e->op.struct_exp.size = 0;
    e->op.struct_exp.symbols = new_symbol_list(0);
    e->op.struct_exp.scope = scope;
	return e;
}

