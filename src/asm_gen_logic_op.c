#include "asm_gen.h"

const char* asm_gen_log_and(asn* l_and){
    asn* left_exp = l_and->op.binary_exp.expr_l;
    asn* right_exp = l_and->op.binary_exp.expr_r;
    const char* left_str = asm_gen(left_exp);
    size_t left_len = strlen(left_str);
    const char* right_str = asm_gen(right_exp);
    size_t right_len = strlen(right_str);

    size_t bufsize = (left_len + right_len + 80 * 8);
    char* lines = (char*) malloc(bufsize * sizeof(char));

    const char* la_1 = gen_label("clause");
    char* la_1_l = (char*) malloc(80 * sizeof(char));
    sprintf(la_1_l, "%s:\n", la_1);
    char* jump_line = (char*) malloc(80 * sizeof(char));
    sprintf(jump_line, "    jne    %s\n", la_1);
    const char* la_end = gen_label("end");
    char* la_end_l = (char*) malloc(80 * sizeof(char));
    sprintf(la_end_l, "%s:\n", la_end);
    char* jump_line2 = (char*) malloc(80 * sizeof(char));
    sprintf(jump_line2, "    jmp    %s\n", la_end);

    strcpy(lines, left_str);
    strcat(lines, "    cmpq   $0, %rax\n");
    strcat(lines, jump_line);
    strcat(lines, jump_line2);
    strcat(lines, la_1_l);
    strcat(lines, right_str);
    strcat(lines, "    cmpq   $0, %rax\n");
    strcat(lines, "    movq   $0, %rax\n");
    strcat(lines, "    setne  %al\n");
    strcat(lines, la_end_l);
    return lines;
}

const char* asm_gen_log_xor(asn* l_xor){
    asn* left_exp = l_xor->op.binary_exp.expr_l;
    asn* right_exp = l_xor->op.binary_exp.expr_r;
    const char* left_str = asm_gen(left_exp);
    size_t left_len = strlen(left_str);
    const char* right_str = asm_gen(right_exp);
    size_t right_len = strlen(right_str);

    size_t bufsize = (left_len + right_len + 80 * 14);
    char* lines = (char*) malloc(bufsize * sizeof(char));

    const char* label_rt = gen_label("rt");
    char* label_rt_line = (char*) malloc(80 * sizeof(char));
    sprintf(label_rt_line, "%s:\n", label_rt);
    char* line_rt_jmp = (char*) malloc(80 * sizeof(char));
    sprintf(line_rt_jmp, "    jne    %s\n", label_rt);

    const char* label_end = gen_label("end");
    char* label_end_line = (char*) malloc(80 * sizeof(char));
    sprintf(label_end_line, "%s:\n", label_end);
    char* line_end_jmp = (char*) malloc(80 * sizeof(char));
    sprintf(line_end_jmp, "    jmp    %s\n", label_end);

    strcpy(lines, left_str);
    strcat(lines, "    pushq  %rax\n");
    strcat(lines, right_str);
    strcat(lines, "    cmpq   $0, %rax\n");
    strcat(lines, line_rt_jmp);
    strcat(lines, "    popq   %rax\n");
    strcat(lines, "    cmpq   $0, %rax\n");
    strcat(lines, "    movq   $0, %rax\n");
    strcat(lines, "    setne  %al\n");
    strcat(lines, line_end_jmp);
    strcat(lines, label_rt_line);
    strcat(lines, "    popq   %rax\n");
    strcat(lines, "    cmpq   $0, %rax\n");
    strcat(lines, "    movq   $0, %rax\n");
    strcat(lines, "    sete   %al\n");
    strcat(lines, label_end_line);
    return lines;
}

const char* asm_gen_log_or(asn* l_or){
    asn* left_exp = l_or->op.binary_exp.expr_l;
    asn* right_exp = l_or->op.binary_exp.expr_r;
    const char* left_str = asm_gen(left_exp);
    size_t left_len = strlen(left_str);
    const char* right_str = asm_gen(right_exp);
	size_t right_len = strlen(right_str);

    size_t bufsize = (left_len + right_len + 80 * 9);
    char* lines = (char*) malloc(bufsize * sizeof(char));

    const char* label_c2 = gen_label("clause");
    char* label_c2_line = (char*) malloc(80 * sizeof(char));
    sprintf(label_c2_line, "%s:\n", label_c2);

    const char* label_end = gen_label("end");
    char* label_end_line = (char*) malloc(80 * sizeof(char));
    sprintf(label_end_line, "%s:\n", label_end);

    char* line_jmp1 = (char*) malloc(80 * sizeof(char));
    sprintf(line_jmp1, "    je     %s\n", label_c2);
    char* line_jmp2 = (char*) malloc(80 * sizeof(char));
    sprintf(line_jmp2, "    jmp    %s\n", label_end);

    strcpy(lines, left_str);
    strcat(lines, "    cmpq   $0, %rax\n");
    strcat(lines, line_jmp1);
    strcat(lines, "    movq   $1, %rax\n");
    strcat(lines, line_jmp2);
    strcat(lines, label_c2_line);
    strcat(lines, right_str);
    strcat(lines, "    cmpq   $0, %rax\n");
    strcat(lines, "    movq   $0, %rax\n");
    strcat(lines, "    setne  %al\n");
    strcat(lines, label_end_line);
    return lines;
}

