/*
 * FILENAME: ic_gen.c
 *
 * DESCRIPTION
 *     Implementations of functions for intermediate code generation.
 *
 * AUTHOR: Moritz Röhrich <moritzr@pool.math.tu-berlin.de>
 *
 */

#include "ic_gen.h"


static size_t tmp_counter;
static unsigned int label_counter;


char* get_tmp_name(void){
    char* name = salloc(tmp_counter / 10 + 3);
    sprintf(name, ".t%zu", tmp_counter);
    return name;
}

char* gen_tmp_name(void){
    tmp_counter++;
    return get_tmp_name();
}

char* gen_label(const char* cust_str){
    /* printf("generating label %u\n", label_counter); */
    size_t len_cs = strlen(cust_str);
    char* label = salloc((len_cs + 10));
    sprintf(label, "_%s%.2x", cust_str, label_counter);
    label_counter++;
    return label;
}


quad_list* ic_gen_translation_unit(asn* ast){
    asn_list* expr_list;
    asn* node;

    quad_list* IC = NULL;
    quad_list* sub_ic = NULL;

    symbol_list_ptr = ast->op.prog_exp.symbols;

    for(expr_list = ast->op.prog_exp.prog;
        expr_list != NULL;
        expr_list = expr_list->next){
        node = expr_list->expr;

        if(node->tag == fun_def_tag){
            sub_ic = ic_gen_fun_def(node);
        } else if(node->tag == assign_tag){
        } else {
        }

        quad_list_app_quad_list(&IC, sub_ic);
    }
    return IC;
}

quad_list* ic_gen(asn* node){
    quad_list* res = NULL;
    switch(node->tag){
        case fun_def_tag: res = ic_gen_fun_def(node); break;
        case call_tag: res = ic_gen_fun_call(node); break;
        case ret_tag: res = ic_gen_ret(node); break;
        case break_tag: res = ic_gen_break(); break;
        case continue_tag: res = ic_gen_continue(); break;
        case var_tag: res = ic_gen_var(node); break;
        case const_int_tag: res = ic_gen_int_const(node); break;
        case const_real_tag: res = ic_gen_real_const(node); break;
        case const_string_tag: res = ic_gen_string_const(node); break;
        case unary_minus_tag: res = ic_gen_unary_minus(node); break;
        case unary_not_tag: res = ic_gen_unary_not(node); break;
        case unary_compl_tag: res = ic_gen_unary_compl(node); break;
        case ref_tag: res = ic_gen_reference(node); break;
        case deref_tag: res = ic_gen_dereference(node); break;
        case bin_add_tag: res = ic_gen_binary(node); break;
        case bin_sub_tag: res = ic_gen_binary(node); break;
        case bin_mul_tag: res = ic_gen_binary(node); break;
        case bin_div_tag: res = ic_gen_binary(node); break;
        case bin_mod_tag: res = ic_gen_binary(node); break;
        case less_tag: res = ic_gen_comp(node); break;
        case less_or_equal_tag: res = ic_gen_comp(node); break;
        case greater_tag: res = ic_gen_comp(node); break;
        case greater_or_equal_tag: res = ic_gen_comp(node); break;
        case equal_tag: res = ic_gen_comp(node); break;
        case not_equal_tag: res = ic_gen_comp(node); break;
        case bit_shift_left_tag: res = ic_gen_bit_op(node); break;
        case bit_shift_right_tag: res = ic_gen_bit_op(node); break;
        case bit_and_tag: res = ic_gen_bit_op(node); break;
        case bit_xor_tag: res = ic_gen_bit_op(node); break;
        case bit_or_tag: res = ic_gen_bit_op(node); break;
        case log_and_tag: res = ic_gen_log_and(node); break;
        case log_xor_tag: res = ic_gen_log_xor(node); break;
        case log_or_tag: res = ic_gen_log_or(node); break;
        case assign_tag: res = ic_gen_assign(node); break;
        case add_assign_tag: res = ic_gen_assign(node); break;
        case sub_assign_tag: res = ic_gen_assign(node); break;
        case mul_assign_tag: res = ic_gen_assign(node); break;
        case div_assign_tag: res = ic_gen_assign(node); break;
        case mod_assign_tag: res = ic_gen_assign(node); break;
        case and_assign_tag: res = ic_gen_assign(node); break;
        case xor_assign_tag: res = ic_gen_assign(node); break;
        case or_assign_tag: res = ic_gen_assign(node); break;
        case bsl_assign_tag: res = ic_gen_assign(node); break;
        case bsr_assign_tag: res = ic_gen_assign(node); break;
        case inc_tag: res = ic_gen_incdec(node); break;
        case dec_tag: res = ic_gen_incdec(node); break;
        case cond_tag: res = ic_gen_cond(node); break;
        case while_loop_tag: res = ic_gen_while_loop(node); break;
        case for_loop_tag: res = ic_gen_for_loop(node); break;
        case cast_to_real_tag: res = ic_gen_int_to_real(node); break;
        default:
            printf("encountered unimplemented expression... skipping\n");
    }
    return res;
}

quad_list* ic_gen_body(asn_list* body){
    quad_list* IC = NULL;
    quad_list* statement_code = NULL;

    asn_list* bptr = NULL;
    for(bptr = body; bptr != NULL; bptr = bptr->next){
        statement_code = ic_gen(bptr->expr);
        quad_list_app_quad_list(&IC, statement_code);
    }

    return IC;
}

