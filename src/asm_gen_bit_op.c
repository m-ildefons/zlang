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

char* asm_gen_bit_op(asn* e){
    asn* left_exp = e->op.binary_exp.lval;
    asn* right_exp = e->op.binary_exp.rval;
    char* left_str = asm_gen(left_exp);
    char* right_str = asm_gen(right_exp);

    char* lines = strnew();

    strapp(&lines, left_str);
    strapp(&lines, "    pushq  %rax\n");
    strapp(&lines, right_str);
	strapp(&lines, "    movq   %rax, %rcx\n");
    strapp(&lines, "    popq   %rax\n");
    switch(e->tag){
        case bit_shift_left_tag: strapp(&lines, "    shlq   %cl, %rax\n"); break;
        case bit_shift_right_tag: strapp(&lines, "    shrq   %cl, %rax\n"); break;
        case bit_and_tag: strapp(&lines, "    andq   %rcx, %rax\n"); break;
        case bit_xor_tag: strapp(&lines, "    xorq   %rcx, %rax\n"); break;
        case bit_or_tag: strapp(&lines, "    orq    %rcx, %rax\n"); break;
        default:
            break;
    }
    return lines;
}

