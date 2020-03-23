/*
 * FILENAME: asm_gen_logic_op.c
 *
 * DESCRIPTION:
 *     Implementation of assembly generation of logical operators.
 *
 * AUTHOR: Moritz Röhrich <moritzr@pool.math.tu-berlin.de>
 *
 */

#include "asm_gen.h"

char* asm_gen_log_and(asn* l_and){
    asn* left_exp = l_and->op.binary_exp.lval;
    asn* right_exp = l_and->op.binary_exp.rval;
    char* left_str = asm_gen(left_exp);
    char* right_str = asm_gen(right_exp);

    char* lines = strnew();

    char* la_1 = gen_label("clause");
    char* la_1_l = salloc(80);
    sprintf(la_1_l, "%s:\n", la_1);
    char* jump_line = salloc(80);
    sprintf(jump_line, "    jne    %s\n", la_1);

    char* la_end = gen_label("end");
    char* la_end_l = salloc(80);
    sprintf(la_end_l, "%s:\n", la_end);
    char* jump_line2 = salloc(80);
    sprintf(jump_line2, "    jmp    %s\n", la_end);

    strapp(&lines, left_str);
    strapp(&lines, "    cmpq   $0, %rax\n");
    strapp(&lines, jump_line);
    strapp(&lines, jump_line2);
    strapp(&lines, la_1_l);
    strapp(&lines, right_str);
    strapp(&lines, "    cmpq   $0, %rax\n");
    strapp(&lines, "    movq   $0, %rax\n");
    strapp(&lines, "    setne  %al\n");
    strapp(&lines, la_end_l);

    free(left_str);
    free(jump_line);
    free(jump_line2);
    free(la_1_l);
    free(right_str);
    free(la_end_l);
    free(la_end);
    free(la_1);
    return lines;
}

char* asm_gen_log_xor(asn* l_xor){
    asn* left_exp = l_xor->op.binary_exp.lval;
    asn* right_exp = l_xor->op.binary_exp.rval;
    char* left_str = asm_gen(left_exp);
    char* right_str = asm_gen(right_exp);

    char* lines = strnew();

    char* label_rt = gen_label("rt");
    char* label_rt_line = salloc(80);
    sprintf(label_rt_line, "%s:\n", label_rt);
    char* line_rt_jmp = salloc(80);
    sprintf(line_rt_jmp, "    jne    %s\n", label_rt);

    char* label_end = gen_label("end");
    char* label_end_line = salloc(80);
    sprintf(label_end_line, "%s:\n", label_end);
    char* line_end_jmp = salloc(80);
    sprintf(line_end_jmp, "    jmp    %s\n", label_end);

    strapp(&lines, left_str);
    strapp(&lines, "    pushq  %rax\n");
    strapp(&lines, right_str);
    strapp(&lines, "    cmpq   $0, %rax\n");
    strapp(&lines, line_rt_jmp);
    strapp(&lines, "    popq   %rax\n");
    strapp(&lines, "    cmpq   $0, %rax\n");
    strapp(&lines, "    movq   $0, %rax\n");
    strapp(&lines, "    setne  %al\n");
    strapp(&lines, line_end_jmp);
    strapp(&lines, label_rt_line);
    strapp(&lines, "    popq   %rax\n");
    strapp(&lines, "    cmpq   $0, %rax\n");
    strapp(&lines, "    movq   $0, %rax\n");
    strapp(&lines, "    sete   %al\n");
    strapp(&lines, label_end_line);

    free(left_str);
    free(right_str);
    free(label_rt_line);
    free(line_rt_jmp);
    free(label_end_line);
    free(line_end_jmp);
    free(label_end);
    free(label_rt);

    return lines;
}

char* asm_gen_log_or(asn* l_or){
    asn* left_exp = l_or->op.binary_exp.lval;
    asn* right_exp = l_or->op.binary_exp.rval;
    char* left_str = asm_gen(left_exp);
    char* right_str = asm_gen(right_exp);

    char* lines = strnew();

    char* label_c2 = gen_label("clause");
    char* label_c2_line = salloc(80);
    sprintf(label_c2_line, "%s:\n", label_c2);
    char* line_jmp1 = salloc(80);
    sprintf(line_jmp1, "    je     %s\n", label_c2);

    char* label_end = gen_label("end");
    char* label_end_line = salloc(80);
    sprintf(label_end_line, "%s:\n", label_end);
    char* line_jmp2 = salloc(80);
    sprintf(line_jmp2, "    jmp    %s\n", label_end);

    strapp(&lines, left_str);
    strapp(&lines, "    cmpq   $0, %rax\n");
    strapp(&lines, line_jmp1);
    strapp(&lines, "    movq   $1, %rax\n");
    strapp(&lines, line_jmp2);
    strapp(&lines, label_c2_line);
    strapp(&lines, right_str);
    strapp(&lines, "    cmpq   $0, %rax\n");
    strapp(&lines, "    movq   $0, %rax\n");
    strapp(&lines, "    setne  %al\n");
    strapp(&lines, label_end_line);

    free(left_str);
    free(right_str);
    free(label_c2_line);
    free(label_end_line);
    free(line_jmp1);
    free(line_jmp2);
    free(label_c2);
    free(label_end);

    return lines;
}

