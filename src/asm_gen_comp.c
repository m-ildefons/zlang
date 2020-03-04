/*
 * FILENAME: asm_gen_comp.c
 *
 * DESCRIPTION:
 *     Implementation of assembly generation of comparison operators.
 *
 * AUTHOR: Moritz Röhrich <moritzr@pool.math.tu-berlin.de>
 *
 */

#include "asm_gen.h"

char* asm_gen_less(asn* less){
    asn* left_exp = less->op.binary_exp.expr_l;
    asn* right_exp = less->op.binary_exp.expr_r;
    char* left_str = asm_gen(left_exp);
    char* right_str = asm_gen(right_exp);

    char* lines = strnew();

    strapp(&lines, left_str);
    strapp(&lines, "    push   %rax\n");
    strapp(&lines, right_str);
    strapp(&lines, "    pop    %rdx\n");
    strapp(&lines, "    cmpq   %rax, %rdx\n");
    strapp(&lines, "    movq   $0, %rax\n");
    strapp(&lines, "    setl   %al\n");
    return lines;
}

char* asm_gen_less_or_equal(asn* leq){
    asn* left_exp = leq->op.binary_exp.expr_l;
    asn* right_exp = leq->op.binary_exp.expr_r;
    char* left_str = asm_gen(left_exp);
    char* right_str = asm_gen(right_exp);

    char* lines = strnew();

    strapp(&lines, left_str);
    strapp(&lines, "    push   %rax\n");
    strapp(&lines, right_str);
    strapp(&lines, "    pop    %rdx\n");
    strapp(&lines, "    cmpq   %rax, %rdx\n");
    strapp(&lines, "    movq   $0, %rax\n");
    strapp(&lines, "    setle  %al\n");
    return lines;
}

char* asm_gen_greater(asn* greater){
    asn* left_exp = greater->op.binary_exp.expr_l;
    asn* right_exp = greater->op.binary_exp.expr_r;
    char* left_str = asm_gen(left_exp);
    char* right_str = asm_gen(right_exp);

    char* lines = strnew();

    strapp(&lines, left_str);
    strapp(&lines, "    push   %rax\n");
    strapp(&lines, right_str);
    strapp(&lines, "    pop    %rdx\n");
    strapp(&lines, "    cmpq   %rax, %rdx\n");
    strapp(&lines, "    movq   $0, %rax\n");
    strapp(&lines, "    setg   %al\n");
    return lines;
}

char* asm_gen_greater_or_equal(asn* geq){
    asn* left_exp = geq->op.binary_exp.expr_l;
    asn* right_exp = geq->op.binary_exp.expr_r;
    char* left_str = asm_gen(left_exp);
    char* right_str = asm_gen(right_exp);

    char* lines = strnew();

    strapp(&lines, left_str);
    strapp(&lines, "    push   %rax\n");
    strapp(&lines, right_str);
    strapp(&lines, "    pop    %rdx\n");
    strapp(&lines, "    cmpq   %rax, %rdx\n");
    strapp(&lines, "    movq   $0, %rax\n");
    strapp(&lines, "    setge  %al\n");
    return lines;

}

char* asm_gen_equal(asn* eq){
    asn* left_exp = eq->op.binary_exp.expr_l;
    asn* right_exp = eq->op.binary_exp.expr_r;
    char* left_str = asm_gen(left_exp);
    char* right_str = asm_gen(right_exp);

    char* lines = strnew();

    strapp(&lines, left_str);
    strapp(&lines, "    push   %rax\n");
    strapp(&lines, right_str);
    strapp(&lines, "    pop    %rdx\n");
    strapp(&lines, "    cmpq   %rax, %rdx\n");
    strapp(&lines, "    movq   $0, %rax\n");
    strapp(&lines, "    sete   %al\n");
    return lines;
}

char* asm_gen_not_equal(asn* neq){
    asn* left_exp = neq->op.binary_exp.expr_l;
    asn* right_exp = neq->op.binary_exp.expr_r;
    char* left_str = asm_gen(left_exp);
    char* right_str = asm_gen(right_exp);

    char* lines = strnew();

    strapp(&lines, left_str);
    strapp(&lines, "    push   %rax\n");
    strapp(&lines, right_str);
    strapp(&lines, "    pop    %rdx\n");
    strapp(&lines, "    cmpq   %rax, %rdx\n");
    strapp(&lines, "    movq   $0, %rax\n");
    strapp(&lines, "    setne  %al\n");
    return lines;
}

