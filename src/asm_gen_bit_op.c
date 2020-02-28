/*
 * FILENAME asm_gen_bit_op.c
 *
 * DESCRIPTION:
 *     Implementation of assembly generation for bitwise operations.
 *
 * AUTHOR: Moritz Röhrich <moritzr@pool.math.tu-berlin.de>
 *
 */

#include "asm_gen.h"

const char* asm_gen_bit_op(asn* e){
    asn* left_exp = e->op.binary_exp.expr_l;
    asn* right_exp = e->op.binary_exp.expr_r;
    const char* left_str = asm_gen(left_exp);
    size_t left_len = strlen(left_str);
    const char* right_str = asm_gen(right_exp);
    size_t right_len = strlen(right_str);

    size_t bufsize = (left_len + right_len + 80 * 4);
    char* lines = (char*) malloc(bufsize  * sizeof(char));

    strcpy(lines, left_str);
    strcat(lines, "    pushq  %rax\n");
    strcat(lines, right_str);
	strcat(lines, "    movq   %rax, %rcx\n");
    strcat(lines, "    popq   %rax\n");
    switch(e->tag){
        case bit_shift_left_tag: strcat(lines, "    shlq   %cl, %rax\n"); break;
        case bit_shift_right_tag: strcat(lines, "    shrq   %cl, %rax\n"); break;
        case bit_and_tag: strcat(lines, "    andq   %rcx, %rax\n"); break;
        case bit_xor_tag: strcat(lines, "    xorq   %rcx, %rax\n"); break;
        case bit_or_tag: strcat(lines, "    orq    %rcx, %rax\n"); break;
        default:
            break;
    }
    return lines;
}

