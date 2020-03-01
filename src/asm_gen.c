/*
 * FILENAME: asm_gen.c
 *
 * DESCRIPTION:
 *     Implementation of assembly generation - Various functions.
 *
 * AUTHOR: Moritz Röhrich <moritzr@pool.math.tu-berlin.de>
 *
 */

#include "asm_gen.h"

const char* gen_label(const char* cust_str){
    /* printf("generating label %u\n", label_counter); */
    size_t len_cs = strlen(cust_str);
    char* label = (char*) malloc((len_cs + 10) * sizeof(char));
    assert(label != NULL);
    sprintf(label, "_%s%.2x", cust_str, label_counter);
    label_counter++;
    return label;
}

void asm_gen_prog(asn* ast_tree, char** asm_src){
    char* src = (char*) malloc(80 * sizeof(char));
    assert(src != NULL);
    char* line = (char*) malloc(80 * sizeof(char));
    assert(line != NULL);

    if(ast_tree->tag != prog_tag){
        printf("Error: Not a program!\n");
        return;
    }

    sprintf(line, "    .file      \"%s\"\n", ast_tree->op.prog_exp.name);
    strcpy(src, line);

    asn_list* prog = NULL;
    asn* expr = NULL;
    const char* expr_src = NULL;
    char* block_src = NULL;
    for(prog = ast_tree->op.prog_exp.prog; prog != NULL; prog = prog->next){
        expr = prog->expr;
        if(expr->tag == var_def_tag){
            expr_src = asm_gen_global_var(expr);
            block_src = (char*) malloc((strlen(expr_src) + 5 * 80) * sizeof(char));
            assert(block_src != NULL);
            sprintf(block_src, "%c", '\0');
            strcat(block_src, "    .text\n");
            sprintf(line, "    .global    %s\n", expr->op.var_def_exp.ident);
            strcat(block_src, line);
            strcat(block_src, "    .data\n");
            strcat(block_src, "    .align 8\n");
            sprintf(line, "    .type      %s, @object\n", expr->op.var_def_exp.ident);
            strcat(block_src, line);
            strcat(block_src, expr_src);
            strapp(&src, block_src);

            free(block_src);
        } else if(expr->tag == fun_def_tag){
            expr_src = asm_gen_fun_def(expr);
            block_src = (char*) malloc((strlen(expr_src) + 3 * 80) * sizeof(char));
            assert(block_src != NULL);
            sprintf(block_src, "%c", '\0');
            strcat(block_src, "    .text\n");
            sprintf(line, "    .global    %s\n", expr->op.fun_def_exp.ident);
            strcat(block_src, line);
            sprintf(line, "    .type      %s, @function\n", expr->op.fun_def_exp.ident);
            strcat(block_src, line);
            strcat(block_src, expr_src);
            strapp(&src, block_src);

            sprintf(line, "    .size   %s, .-%s\n", expr->op.fun_def_exp.ident, expr->op.fun_def_exp.ident);
            strapp(&src, line);

            free(block_src);
        }
    }
    unsigned int i;
    for(i = 0; i < string_count; i++){
        block_src = (char*) malloc(80 * sizeof(char));
        assert(block_src != NULL);
        sprintf(block_src, ".SC%u:\n", i);
        strapp(&src, "    .section    .rodata\n");
        strapp(&src, block_src);
        sprintf(block_src, "    .string \"%s\"\n", string_index[i]);
        strapp(&src, block_src);
        free(block_src);
    }

    for(i = 0; i < real_count; i++){
        block_src = (char*) malloc(80 * sizeof(char));
        assert(block_src != NULL);
        sprintf(block_src, ".FC%u:\n", i);
        strapp(&src, "    .section    .rodata\n");
        strapp(&src, "    .align 8\n");
        strapp(&src, block_src);
        unsigned long bits = *((unsigned long*) &(real_index[i]));
        unsigned int upper = ((unsigned int*) &bits)[0];
        unsigned int lower = ((unsigned int*) &bits)[1];
        sprintf(block_src, "    .long   %u\n", upper);
        strapp(&src, block_src);
        sprintf(block_src, "    .long   %u\n", lower);
        strapp(&src, block_src);
        free(block_src);
    }

    strapp(&src, "    .ident  \"Boa Version 0.1\"\n");
    (*asm_src) = src;
    free(line);
}

const char* asm_gen(asn* e){
    printf("generating asm, tag: %u\n", e->tag);
    const char* res = NULL;
    switch(e->tag){
        case fun_def_tag: res = asm_gen_fun_def(e); break;
		case call_tag: res = asm_gen_fun_call(e); break;
        case var_def_tag: res = asm_gen_var_def(e); break;
        case var_ref_tag: res = asm_gen_var_ref(e); break;
        case const_int_tag: res = asm_gen_int_const(e); break;
        case const_real_tag: res = asm_gen_real_const(e); break;
        case const_string_tag: res = asm_gen_string_const(e); break;
        case unary_minus_tag: res = asm_gen_unary_minus(e); break;
        case unary_not_tag: res = asm_gen_unary_not(e); break;
        case unary_compl_tag: res = asm_gen_unary_compl(e); break;
        case ref_tag: res = asm_gen_reference(e); break;
        case deref_tag: res = asm_gen_dereference(e); break;
        case bin_add_tag: res = asm_gen_binary_plus(e); break;
        case bin_sub_tag: res = asm_gen_binary_sub(e); break;
        case bin_mul_tag: res = asm_gen_binary_mult(e); break;
        case bin_div_tag: res = asm_gen_binary_div(e); break;
        case bin_mod_tag: res = asm_gen_binary_mod(e); break;
        case less_tag: res = asm_gen_less(e); break;
        case less_or_equal_tag: res = asm_gen_less_or_equal(e); break;
        case greater_tag: res = asm_gen_greater(e); break;
        case greater_or_equal_tag: res = asm_gen_greater_or_equal(e); break;
        case equal_tag: res = asm_gen_equal(e); break;
        case not_equal_tag: res = asm_gen_not_equal(e); break;
        case bit_shift_left_tag: res = asm_gen_bit_op(e); break;
        case bit_shift_right_tag: res = asm_gen_bit_op(e); break;
        case bit_and_tag: res = asm_gen_bit_op(e); break;
        case bit_xor_tag: res = asm_gen_bit_op(e); break;
        case bit_or_tag: res = asm_gen_bit_op(e); break;
        case log_and_tag: res = asm_gen_log_and(e); break;
        case log_xor_tag: res = asm_gen_log_xor(e); break;
        case log_or_tag: res = asm_gen_log_or(e); break;
		case assign_tag: res = asm_gen_assign(e); break;
		case add_assign_tag: res = asm_gen_assign(e); break;
        case sub_assign_tag: res = asm_gen_assign(e); break;
        case mul_assign_tag: res = asm_gen_assign(e); break;
        case div_assign_tag: res = asm_gen_assign(e); break;
        case mod_assign_tag: res = asm_gen_assign(e); break;
        case and_assign_tag: res = asm_gen_assign(e); break;
        case xor_assign_tag: res = asm_gen_assign(e); break;
        case or_assign_tag: res = asm_gen_assign(e); break;
        case bsl_assign_tag: res = asm_gen_assign(e); break;
        case bsr_assign_tag: res = asm_gen_assign(e); break;
        case inc_tag: res = asm_gen_inc(e); break;
        case dec_tag: res = asm_gen_dec(e); break;
		case cond_tag: res = asm_gen_cond(e); break;
		case while_loop_tag: res = asm_gen_while_loop(e); break;
		case for_loop_tag: res = asm_gen_for_loop(e); break;
        default:
            printf("encountered unimplemented expression... skipping\n");
            res = "\n";
    }
    return res;
}

const char* asm_gen_fun_def(asn* fun_def){
    pv_root* old_symbol_map_ptr = symbol_map_ptr;
    symbol_map_ptr = fun_def->op.fun_def_exp.symbol_map;

    char* id = fun_def->op.fun_def_exp.ident;
    asn_list* args = fun_def->op.fun_def_exp.args;
    asn_list* body = fun_def->op.fun_def_exp.body;
    pv_root* symbol_map = fun_def->op.fun_def_exp.symbol_map;
    int map_scope = symbol_map->scope;
    ca_list* key_list = symbol_map->key_list;
    pv_leaf* symbol;
    int num_vars = 0;
    size_t num_chars = 480;

    for(; key_list != NULL; key_list = key_list->next){
        symbol = pv_search(symbol_map, key_list->key);
        /*printf("%s %d %d\n", key_list->key, symbol->scope, map_scope);*/
        if(symbol->scope == map_scope)
            num_vars++;
    }

    /* printf("function containing %d vars\n", num_vars); */

    char* src = (char*) malloc((strlen(id) + 3) * sizeof(char));
    assert(src != NULL);
    sprintf(src, "%s:\n", id);
    strapp(&src, "    pushq  %rbp\n");
    strapp(&src, "    movq   %rsp, %rbp\n");
    const char* other_src = NULL;
	int i;
	char* arg_src;
	for(i = 0; i < 6 && args != NULL; i++, args = args->next){
		switch(i){
			case 0: strapp(&src, "    pushq  %rdi\n"); break;
			case 1: strapp(&src, "    pushq  %rsi\n"); break;
			case 2: strapp(&src, "    pushq  %rdx\n"); break;
			case 3: strapp(&src, "    pushq  %rcx\n"); break;
			case 4: strapp(&src, "    pushq  %r8\n"); break;
			case 5: strapp(&src, "    pushq  %r9\n"); break;
			default: strcat(src, "; should not have happened\n"); break;
		}
	}

    for(; body != NULL; body = body->next){
        if(body->expr->tag != ret_tag){
            other_src = asm_gen(body->expr);
        } else {
            other_src = asm_gen_ret(body->expr, num_vars);
		}
        strapp(&src, other_src);
    }

    strapp(&src, "    movq   $0, %rax\n");
	strapp(&src, "    movq   %rbp, %rsp\n");
	strapp(&src, "    popq   %rbp\n");
	strapp(&src, "    ret\n");

    symbol_map_ptr = old_symbol_map_ptr;
    return src;
}

const char* asm_gen_fun_call(asn* call){
    printf("generating function call\n");
	const char* id = call->op.call_exp.ident;

	char* src = (char*) malloc(sizeof(char));
    assert(src != NULL);
	sprintf(src, "%c", '\0');
    char* stack_args_src = (char*) malloc(sizeof(char));
    assert(stack_args_src != NULL);
	sprintf(stack_args_src, "%c", '\0');

	const char* arg_ref;
    const char* arg_ident;
    pv_leaf* leaf;
    unsigned int c_xmm_regs = 0;
	int i = 0;
	asn_list* args = call->op.call_exp.args;
	for(; args != NULL; args = args->next, i++){
		arg_ref = asm_gen(args->expr);
        if(args->expr->tag == var_ref_tag){
            arg_ident = args->expr->op.var_ref_exp.ident;
            leaf = pv_search(symbol_map_ptr, arg_ident);
            if(leaf->type == at_real){
                //strapp(&src, arg_ref);
                c_xmm_regs++;
                continue;
            }
        }
		switch(i){
			case 0:
		        strapp(&src, arg_ref);
                strapp(&src, "    movq   %rax, %rdi\n");
                break;
			case 1:
		        strapp(&src, arg_ref);
                strapp(&src, "    movq   %rax, %rsi\n");
                break;
			case 2:
		        strapp(&src, arg_ref);
                strapp(&src, "    movq   %rax, %rdx\n");
                break;
			case 3:
		        strapp(&src, arg_ref);
                strapp(&src, "    movq   %rax, %rcx\n");
                break;
			case 4:
		        strapp(&src, arg_ref);
                strapp(&src, "    movq   %rax, %r8\n");
                break;
			case 5:
		        strapp(&src, arg_ref);
                strapp(&src, "    movq   %rax, %r9\n");
                break;
			default:
                strprp(&stack_args_src, "    pushq  %rax\n");
                strprp(&stack_args_src, arg_ref);
                break;
		}
	}
    strapp(&src, stack_args_src);

    char* line = (char*) malloc(80 * sizeof(char));
    assert(line != NULL);
    sprintf(line, "    movq   $%u, %%rax\n", c_xmm_regs);
    strapp(&src, line);

    int stack_align = (16 - (symbol_map_ptr->mem_offset % 16)) % 16;
    if(stack_align != 0){
        sprintf(line, "    subq   $%d, %%rsp\n", stack_align);
        strapp(&src, line);
    }

	sprintf(line, "    call   %s\n", id);
    strapp(&src, line);

    if(stack_align != 0){
        sprintf(line, "    addq   $%d, %%rsp\n", stack_align);
        strapp(&src, line);
    }

	args = call->op.call_exp.args;
	for(i = 0; args != NULL; args = args->next, i++){
		if(i < 6)
			continue;
        strapp(&src, "    popq   %rdx\n");
	}

    free(line);
	return src;
}

const char* asm_gen_var_def(asn* var_def){
    printf("generating var def\n");
    asn* val = NULL;
    const char* rhs = NULL;
    size_t rhs_len = 0;
    if(var_def->op.var_def_exp.val != NULL){
        val = var_def->op.var_def_exp.val;
        rhs = asm_gen(val);
    } else {
        rhs = asm_gen_int_const(make_int_exp(0));
    }
    rhs_len = strlen(rhs);
    char* code = (char*) malloc((80 + rhs_len) * sizeof(char));
    assert(code != NULL);
    strcpy(code, rhs);

    const char* id;
    pv_leaf* leaf;
    int offset;
    char* line = (char*) malloc(80 * sizeof(char));
    if(var_def->op.var_def_exp.type == at_real){
        id = var_def->op.var_def_exp.ident;
        leaf = pv_search(symbol_map_ptr, id);
        offset = -(leaf->offset + leaf->size);
        strapp(&code, "    subq   $8, %rsp\n");
        sprintf(line, "    movsd  %%xmm0, %d(%%rbp)\n", offset);
        strapp(&code, line);
    } else {
        strapp(&code, "    pushq  %rax\n");
    }
    return code;
}

const char* asm_gen_var_ref(asn* var_ref){
    printf("generatin var ref\n");
    char* id = var_ref->op.var_ref_exp.ident;
    pv_leaf* leaf = pv_search(symbol_map_ptr, id);
    char* src = (char*) malloc(80 * sizeof(char));
    assert(src != NULL);
    if(leaf->scope != 0)
        sprintf(src, "    movq   %d(%%rbp), %%rax\n", -(leaf->offset+leaf->size));
    else
        sprintf(src, "    movq   %s(%%rip), %%rax\n", leaf->ident);
    return src;
}

const char* asm_gen_global_var(asn* e){
    char* src = (char*) malloc(160 * sizeof(char));
    assert(src != NULL);
    const char* id = e->op.var_def_exp.ident;
    asn* val = e->op.var_def_exp.val;

    if(val->tag == assign_tag)
        val = val->op.assign_exp.val;

    if(val->tag == const_int_tag)
        sprintf(src, "%s:\n    .long %d\n", id, val->op.int_exp);
    return src;
}

const char* asm_gen_ret(asn* ret_exp, int num_vars){
    asn* e = ret_exp->op.ret_exp.val;
    const char* str = asm_gen(e);
    size_t code_len = strlen(str);
    size_t ret_code_str_len = (size_t) ((num_vars + 3) * 80);
    int i;

	size_t bufsize = code_len + ret_code_str_len;
    char* expr_str = (char*) malloc(bufsize * sizeof(char));
    assert(expr_str != NULL);
    strcpy(expr_str, str);
	strcat(expr_str, "    movq   %rbp, %rsp\n");
    strcat(expr_str, "    popq   %rbp\n");
    strcat(expr_str, "    ret\n");
    return expr_str;
}

const char* asm_gen_body(asn_list* body, int num_vars){
	int i;
	size_t num_chars = 2;
	char* body_src = (char*) malloc(num_chars * sizeof(char));
    assert(body_src != NULL);
	strcpy(body_src, "");

	const char* statement_src = NULL;

	for(; body != NULL; body = body->next){
		if(body->expr->tag == ret_tag){
			statement_src = asm_gen_ret(body->expr, num_vars);
		} else if(body->expr->tag == break_tag){
			statement_src = asm_gen_jump(body->expr);
		} else if(body->expr->tag == continue_tag){
			statement_src = asm_gen_jump(body->expr);
		} else {
			statement_src = asm_gen(body->expr);
		}
        strapp(&body_src, statement_src);
	}

	return body_src;
}

const char* asm_gen_jump(asn* jump){
	char* src = (char*) malloc(80 * sizeof(char*));
    assert(src != NULL);

	const char* jump_label;
	if(jump->tag == break_tag)
		jump_label = loop_end_label;
	else if(jump->tag == continue_tag)
		jump_label = loop_start_label;

	sprintf(src, "    jmp    %s\n", jump_label);
	return src;
}

