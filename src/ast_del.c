/*
 * FILENAME: ast_del.c
 *
 * DESCRIPTION:
 *     Implementation of deconstructors for the abstract syntax tree.
 *
 * AUTHOR: Moritz Röhrich
 *
 */

#include "ast.h"

void delete_exp(asn* e){
    if(e == NULL)
        return;

    switch(e->tag){
        case const_int_tag: free(e); break;
        case const_real_tag: free(e); break;
        case const_char_tag: free(e); break;
        case const_string_tag: free(e); break;
        case var_tag: delete_var_exp(e); break;
        case ptr_tag: delete_ptr_exp(e); break;
        case ident_tag: free(e); break;
        case call_tag: delete_call_exp(e); break;
        case ret_tag: delete_return_exp(e); break;
        case break_tag: free(e); break;
        case continue_tag: free(e); break;
        case fun_def_tag: delete_fun_def_exp(e); break;
        case cond_tag: delete_cond_exp(e); break;
        case prog_tag: delete_prog_exp(e); break;
        case assign_tag: delete_assign_exp(e); break;
        case unary_minus_tag: delete_unary_exp(e); break;
        case unary_not_tag: delete_unary_exp(e); break;
        case unary_compl_tag: delete_unary_exp(e); break;
        case ref_tag: delete_unary_exp(e); break;
        case deref_tag: delete_unary_exp(e); break;
        case bin_add_tag: delete_binary_exp(e); break;
        case bin_sub_tag: delete_binary_exp(e); break;
        case bin_mul_tag: delete_binary_exp(e); break;
        case bin_div_tag: delete_binary_exp(e); break;
        case bin_mod_tag: delete_binary_exp(e); break;
        case bit_shift_left_tag: delete_binary_exp(e); break;
        case bit_shift_right_tag: delete_binary_exp(e); break;
        case less_tag: delete_binary_exp(e); break;
        case less_or_equal_tag: delete_binary_exp(e); break;
        case greater_tag: delete_binary_exp(e); break;
        case greater_or_equal_tag: delete_binary_exp(e); break;
        case equal_tag: delete_binary_exp(e); break;
        case not_equal_tag: delete_binary_exp(e); break;
        case bit_and_tag: delete_binary_exp(e); break;
        case bit_xor_tag: delete_binary_exp(e); break;
        case bit_or_tag: delete_binary_exp(e); break;
        case log_and_tag: delete_binary_exp(e); break;
        case log_xor_tag: delete_binary_exp(e); break;
        case log_or_tag: delete_binary_exp(e); break;
        case inc_tag: delete_unary_exp(e); break;
        case dec_tag: delete_unary_exp(e); break;
        case add_assign_tag: delete_assign_exp(e); break;
        case sub_assign_tag: delete_assign_exp(e); break;
        case mul_assign_tag: delete_assign_exp(e); break;
        case div_assign_tag: delete_assign_exp(e); break;
        case mod_assign_tag: delete_assign_exp(e); break;
        case and_assign_tag: delete_assign_exp(e); break;
        case xor_assign_tag: delete_assign_exp(e); break;
        case or_assign_tag: delete_assign_exp(e); break;
        case bsl_assign_tag: delete_assign_exp(e); break;
        case bsr_assign_tag: delete_assign_exp(e); break;
        case for_loop_tag: delete_for_loop_exp(e); break;
        case while_loop_tag: delete_while_loop_exp(e); break;
        case cast_to_real_tag: delete_unary_exp(e); break;
        case struct_tag: delete_struct_exp(e); break;
        case union_tag: delete_struct_exp(e); break;
        default: free(e); return;
    }
}

void delete_asn_list(asn_list* l){
    if(l == NULL)
        return;

    if(l->expr != NULL)
        delete_exp(l->expr);

    delete_asn_list(l->next);
    free(l);
}

void delete_prog_exp(asn* e){
    if(e == NULL)
        return;

    delete_asn_list(e->op.prog_exp.prog);
    delete_symbol_list(e->op.prog_exp.symbols);
    free(e->op.prog_exp.name);
	free(e);
}

void delete_var_exp(asn* e){
    if(e == NULL)
        return;

    delete_symbol(&e->op.var_exp.sym);
    free(e);
}

void delete_ptr_exp(asn* e){
    if(e == NULL)
        return;

    delete_exp(e->op.ptr_exp.to);
    free(e);
}

void delete_fun_def_exp(asn* e){
    if(e == NULL)
        return;

    delete_asn_list(e->op.fun_def_exp.args);
    delete_asn_list(e->op.fun_def_exp.body);
    delete_symbol_list(e->op.fun_def_exp.symbols);
    free(e);
}

void delete_call_exp(asn* e){
    if(e == NULL)
        return;

    delete_asn_list(e->op.call_exp.args);
    free(e);
}

void delete_return_exp(asn* e){
    if(e == NULL)
        return;

    delete_exp(e->op.ret_exp.val);
    free(e);
}

void delete_cond_exp(asn* e){
    if(e == NULL)
        return;

    delete_exp(e->op.cond_exp.cond);
    delete_asn_list(e->op.cond_exp.if_body);
    delete_asn_list(e->op.cond_exp.else_body);
    delete_symbol_list(e->op.cond_exp.if_symbols);
    delete_symbol_list(e->op.cond_exp.else_symbols);
    free(e);
}

void delete_while_loop_exp(asn* e){
    if(e == NULL)
        return;

    delete_exp(e->op.while_loop_exp.cond);
    delete_asn_list(e->op.while_loop_exp.body);
    delete_symbol_list(e->op.while_loop_exp.symbols);
    free(e);
}

void delete_for_loop_exp(asn* e){
    if(e == NULL)
        return;

    delete_exp(e->op.for_loop_exp.init);
    delete_exp(e->op.for_loop_exp.cond);
    delete_exp(e->op.for_loop_exp.move);
    delete_asn_list(e->op.for_loop_exp.body);
    delete_symbol_list(e->op.for_loop_exp.symbols);
    free(e);
}

void delete_unary_exp(asn* e){
    if(e == NULL)
        return;

    delete_exp(e->op.unary_exp.val);
    free(e);
}

void delete_binary_exp(asn* e){
    if(e == NULL)
        return;

    delete_exp(e->op.binary_exp.lval);
    delete_exp(e->op.binary_exp.rval);
    free(e);
}

void delete_assign_exp(asn* e){
    if(e == NULL)
        return;

    delete_exp(e->op.assign_exp.lval);
    delete_exp(e->op.assign_exp.rval);
    free(e);
}

void delete_struct_exp(asn* e){
    if(e == NULL)
        return;

    delete_asn_list(e->op.struct_exp.body);
    delete_symbol_list(e->op.struct_exp.symbols);
    free(e);
}

