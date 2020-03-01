/*
 * FILENAME: asm_gen_calc.c
 *
 * DESCRIPTION:
 *     Implementation of assembly generation of integer arithmetic.
 *
 * AUTHOR: Moritz Röhrich <moritzr@pool.math.tu-berlin.de>
 *
 */

#include "asm_gen.h"

const char* asm_gen_binary_plus(asn* b_plus){
    asn* left_exp = b_plus->op.binary_exp.expr_l;
    asn* right_exp = b_plus->op.binary_exp.expr_r;
    const char* left_str = asm_gen(left_exp);
    const char* right_str = asm_gen(right_exp);
    char* lines = (char*) malloc(sizeof(char));
    assert(lines != NULL);
    sprintf(lines, "%c", '\0');

    if(left_exp->tag == const_float_tag ||
        right_exp->tag == const_float_tag){
        strapp(&lines, left_str);
        strapp(&lines, "    subq   $8, %rsp\n");
        strapp(&lines, "    movsd  %xmm0, (%rsp)\n");
        strapp(&lines, right_str);
        strapp(&lines, "    movsd  (%rsp), %xmm1\n");
        strapp(&lines, "    addq   $8, %rsp\n");
        strapp(&lines, "    addsd  %xmm1, %xmm0\n");
    } else {
        strapp(&lines, left_str);
        strapp(&lines, "    pushq  %rax\n");
        strapp(&lines, right_str);
        strapp(&lines, "    popq   %rdx\n");
        strapp(&lines, "    addq   %rdx, %rax\n");
    }
    return lines;
}

const char* asm_gen_binary_sub(asn* b_sub){
    asn* left_exp = b_sub->op.binary_exp.expr_l;
    asn* right_exp = b_sub->op.binary_exp.expr_r;
    const char* left_str = asm_gen(left_exp);
    size_t left_len = strlen(left_str);
    const char* right_str = asm_gen(right_exp);
    size_t right_len = strlen(right_str);
	size_t src_len = 240 + left_len + right_len;
    char* lines = (char*) malloc(src_len * sizeof(char));
    strcpy(lines, right_str);
    strcat(lines, "    pushq  %rax\n");
    strcat(lines, left_str);
    strcat(lines, "    popq   %rdx\n");
    strcat(lines, "    subq   %rdx, %rax\n");
    return lines;
}

const char* asm_gen_binary_mult(asn* b_mult){
    asn* left_exp = b_mult->op.binary_exp.expr_l;
    asn* right_exp = b_mult->op.binary_exp.expr_r;
    const char* left_str = asm_gen(left_exp);
    size_t left_len = strlen(left_str);
    const char* right_str = asm_gen(right_exp);
    size_t right_len = strlen(right_str);
	size_t src_len = 240 + left_len + right_len;
    char* lines = (char*) malloc(src_len * sizeof(char));
    strcpy(lines, left_str);
    strcat(lines, "    pushq  %rax\n");
    strcat(lines, right_str);
    strcat(lines, "    popq   %rdx\n");
    strcat(lines, "    imulq  %rdx, %rax\n");
    return lines;
}

const char* asm_gen_binary_div(asn* b_div){
    asn* left_exp = b_div->op.binary_exp.expr_l;
    asn* right_exp = b_div->op.binary_exp.expr_r;
    const char* left_str = asm_gen(left_exp);
    size_t left_len = strlen(left_str);
    const char* right_str = asm_gen(right_exp);
    size_t right_len = strlen(right_str);
	size_t src_len = 300 + left_len + right_len;
    char* lines = (char*) malloc(src_len * sizeof(char));
    strcpy(lines, right_str);
    strcat(lines, "    pushq  %rax\n");
    strcat(lines, left_str);
    strcat(lines, "    popq   %rbx\n");
    strcat(lines, "    cqo\n");
    strcat(lines, "    idivq  %rbx\n");
    return lines;
}

const char* asm_gen_binary_mod(asn* b_mod){
    asn* left_exp = b_mod->op.binary_exp.expr_l;
    asn* right_exp = b_mod->op.binary_exp.expr_r;
    const char* left_str = asm_gen(left_exp);
    size_t left_len = strlen(left_str);
    const char* right_str = asm_gen(right_exp);
    size_t right_len = strlen(right_str);
	size_t src_len = 400 + left_len + right_len;
    char* lines = (char*) malloc(src_len * sizeof(char));
    strcpy(lines, right_str);
    strcat(lines, "    pushq  %rax\n");
    strcat(lines, left_str);
    strcat(lines, "    popq   %rbx\n");
    strcat(lines, "    cqo\n");
    strcat(lines, "    idivq  %rbx\n");
    strcat(lines, "    movq   %rdx, %rax\n");
    return lines;
}

