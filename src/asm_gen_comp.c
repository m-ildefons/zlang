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

const char* asm_gen_less(asn* less){
    asn* left_exp = less->op.binary_exp.expr_l;
    asn* right_exp = less->op.binary_exp.expr_r;
    const char* left_str = asm_gen(left_exp);
    const char* right_str = asm_gen(right_exp);
    size_t left_len = strlen(left_str);
    size_t right_len = strlen(right_str);

    size_t bufsize = (left_len + right_len + 80 * 5);
    char* lines = (char*) malloc(bufsize * sizeof(char));

    strcpy(lines, left_str);
    strcat(lines, "    push   %rax\n");
    strcat(lines, right_str);
    strcat(lines, "    pop    %rdx\n");
    strcat(lines, "    cmpq   %rax, %rdx\n");
    strcat(lines, "    movq   $0, %rax\n");
    strcat(lines, "    setl   %al\n");
    return lines;
}

const char* asm_gen_less_or_equal(asn* leq){
    asn* left_exp = leq->op.binary_exp.expr_l;
    asn* right_exp = leq->op.binary_exp.expr_r;
    const char* left_str = asm_gen(left_exp);
    const char* right_str = asm_gen(right_exp);
    size_t left_len = strlen(left_str);
    size_t right_len = strlen(right_str);

    size_t bufsize = (left_len + right_len + 80 * 5);
    char* lines = (char*) malloc(bufsize * sizeof(char));

    strcpy(lines, left_str);
    strcat(lines, "    push   %rax\n");
    strcat(lines, right_str);
    strcat(lines, "    pop    %rdx\n");
    strcat(lines, "    cmpq   %rax, %rdx\n");
    strcat(lines, "    movq   $0, %rax\n");
    strcat(lines, "    setle  %al\n");
    return lines;
}

const char* asm_gen_greater(asn* greater){
    asn* left_exp = greater->op.binary_exp.expr_l;
    asn* right_exp = greater->op.binary_exp.expr_r;
    const char* left_str = asm_gen(left_exp);
    const char* right_str = asm_gen(right_exp);
    size_t left_len = strlen(left_str);
    size_t right_len = strlen(right_str);

    size_t bufsize = (left_len + right_len + 80 * 5);
    char* lines = (char*) malloc(bufsize * sizeof(char));

    strcpy(lines, left_str);
    strcat(lines, "    push   %rax\n");
    strcat(lines, right_str);
    strcat(lines, "    pop    %rdx\n");
    strcat(lines, "    cmpq   %rax, %rdx\n");
    strcat(lines, "    movq   $0, %rax\n");
    strcat(lines, "    setg   %al\n");
    return lines;
}

const char* asm_gen_greater_or_equal(asn* geq){
    asn* left_exp = geq->op.binary_exp.expr_l;
    asn* right_exp = geq->op.binary_exp.expr_r;
    const char* left_str = asm_gen(left_exp);
    const char* right_str = asm_gen(right_exp);
    size_t left_len = strlen(left_str);
    size_t right_len = strlen(right_str);

    size_t bufsize = (left_len + right_len + 80 * 5);
    char* lines = (char*) malloc(bufsize * sizeof(char));

    strcpy(lines, left_str);
    strcat(lines, "    push   %rax\n");
    strcat(lines, right_str);
    strcat(lines, "    pop    %rdx\n");
    strcat(lines, "    cmpq   %rax, %rdx\n");
    strcat(lines, "    movq   $0, %rax\n");
    strcat(lines, "    setge  %al\n");
    return lines;

}

const char* asm_gen_equal(asn* eq){
    asn* left_exp = eq->op.binary_exp.expr_l;
    asn* right_exp = eq->op.binary_exp.expr_r;
    const char* left_str = asm_gen(left_exp);
    const char* right_str = asm_gen(right_exp);
    size_t left_len = strlen(left_str);
    size_t right_len = strlen(right_str);

    size_t bufsize = (left_len + right_len + 80 * 5);
    char* lines = (char*) malloc(bufsize * sizeof(char));

    strcpy(lines, left_str);
    strcat(lines, "    push   %rax\n");
    strcat(lines, right_str);
    strcat(lines, "    pop    %rdx\n");
    strcat(lines, "    cmpq   %rax, %rdx\n");
    strcat(lines, "    movq   $0, %rax\n");
    strcat(lines, "    sete   %al\n");
    return lines;
}

const char* asm_gen_not_equal(asn* neq){
    asn* left_exp = neq->op.binary_exp.expr_l;
    asn* right_exp = neq->op.binary_exp.expr_r;
    const char* left_str = asm_gen(left_exp);
    const char* right_str = asm_gen(right_exp);
    size_t left_len = strlen(left_str);
    size_t right_len = strlen(right_str);

    size_t bufsize = (left_len + right_len + 80 * 5);
    char* lines = (char*) malloc(bufsize * sizeof(char));

    strcpy(lines, left_str);
    strcat(lines, "    push   %rax\n");
    strcat(lines, right_str);
    strcat(lines, "    pop    %rdx\n");
    strcat(lines, "    cmpq   %rax, %rdx\n");
    strcat(lines, "    movq   $0, %rax\n");
    strcat(lines, "    setne  %al\n");
    return lines;
}

