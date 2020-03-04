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

char* asm_gen_binary_plus(asn* b_plus){
    asn* left_exp = b_plus->op.binary_exp.expr_l;
    asn* right_exp = b_plus->op.binary_exp.expr_r;
    char* left_str = asm_gen(left_exp);
    char* right_str = asm_gen(right_exp);
    char* lines = strnew();

    if(b_plus->op.binary_exp.type == at_real){
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

char* asm_gen_binary_sub(asn* b_sub){
    asn* left_exp = b_sub->op.binary_exp.expr_l;
    asn* right_exp = b_sub->op.binary_exp.expr_r;
    char* left_str = asm_gen(left_exp);
    char* right_str = asm_gen(right_exp);
    char* lines = strnew();

    if(b_sub->op.binary_exp.type == at_real){
        strapp(&lines, left_str);
        strapp(&lines, "    subq   $8, %rsp\n");
        strapp(&lines, "    movsd  %xmm0, (%rsp)\n");
        strapp(&lines, right_str);
        strapp(&lines, "    movsd  (%rsp), %xmm1\n");
        strapp(&lines, "    addq   $8, %rsp\n");
        strapp(&lines, "    subsd  %xmm1, %xmm0\n");
    } else {
        strapp(&lines, right_str);
        strapp(&lines, "    pushq  %rax\n");
        strapp(&lines, left_str);
        strapp(&lines, "    popq   %rdx\n");
        strapp(&lines, "    subq   %rdx, %rax\n");
    }
    return lines;
}

char* asm_gen_binary_mult(asn* b_mult){
    asn* left_exp = b_mult->op.binary_exp.expr_l;
    asn* right_exp = b_mult->op.binary_exp.expr_r;
    char* left_str = asm_gen(left_exp);
    char* right_str = asm_gen(right_exp);
    char* lines = strnew();

    if(b_mult->op.binary_exp.type == at_real){
        strapp(&lines, left_str);
        strapp(&lines, "    subq   $8, %rsp\n");
        strapp(&lines, "    movsd  %xmm0, (%rsp)\n");
        strapp(&lines, right_str);
        strapp(&lines, "    movsd  (%rsp), %xmm1\n");
        strapp(&lines, "    addq   $8, %rsp\n");
        strapp(&lines, "    mulsd  %xmm1, %xmm0\n");
    } else {
        strapp(&lines, left_str);
        strapp(&lines, "    pushq  %rax\n");
        strapp(&lines, right_str);
        strapp(&lines, "    popq   %rdx\n");
        strapp(&lines, "    imulq  %rdx, %rax\n");
    }
    return lines;
}

char* asm_gen_binary_div(asn* b_div){
    asn* left_exp = b_div->op.binary_exp.expr_l;
    asn* right_exp = b_div->op.binary_exp.expr_r;
    char* left_str = asm_gen(left_exp);
    char* right_str = asm_gen(right_exp);
    char* lines = strnew();

    if(b_div->op.binary_exp.type == at_real){
        strapp(&lines, right_str);
        strapp(&lines, "    subq   $8, %rsp\n");
        strapp(&lines, "    movsd  %xmm0, (%rsp)\n");
        strapp(&lines, left_str);
        strapp(&lines, "    movsd  (%rsp), %xmm1\n");
        strapp(&lines, "    addq   $8, %rsp\n");
        strapp(&lines, "    divsd  %xmm1, %xmm0\n");
    } else {
        strapp(&lines, right_str);
        strapp(&lines, "    pushq  %rax\n");
        strapp(&lines, left_str);
        strapp(&lines, "    popq   %rbx\n");
        strapp(&lines, "    cqo\n");
        strapp(&lines, "    idivq  %rbx\n");
    }
    return lines;
}

char* asm_gen_binary_mod(asn* b_mod){
    asn* left_exp = b_mod->op.binary_exp.expr_l;
    asn* right_exp = b_mod->op.binary_exp.expr_r;
    char* left_str = asm_gen(left_exp);
    char* right_str = asm_gen(right_exp);
    char* lines = strnew();

    strapp(&lines, right_str);
    strapp(&lines, "    pushq  %rax\n");
    strapp(&lines, left_str);
    strapp(&lines, "    popq   %rbx\n");
    strapp(&lines, "    cqo\n");
    strapp(&lines, "    idivq  %rbx\n");
    strapp(&lines, "    movq   %rdx, %rax\n");
    return lines;
}

