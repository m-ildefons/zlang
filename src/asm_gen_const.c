/*
 * FILENAME: asm_gen_const.c
 *
 * DESCRIPTION:
 *     Implementation of functions for generating assembly source for dealing
 *     with constants.
 *
 * AUTHOR: Moritz Röhrich <moritzr@pool.math.tu-berlin.de>
 *
 */

#include "asm_gen.h"

char* asm_gen_int_const(asn* int_const){
    printf("generating const int\n");
    int num = int_const->op.int_exp;

    char* buffer = salloc(80);
    sprintf(buffer, "    movq   $%d, %%rax\n", num);
    return buffer;
}

char* asm_gen_real_const(asn* real_const){
    printf("generating const real\n");
    int idx = real_const->op.int_exp;
    char* buffer = salloc(80);

    sprintf(buffer, "    movsd  .FC%d(%%rip), %%xmm0\n", idx);
    return buffer;
}

char* asm_gen_string_const(asn* str_const){
    printf("generating const string\n");
    int idx = str_const->op.int_exp;
    char* buffer = salloc(80);

    sprintf(buffer, "    leaq   .SC%d(%%rip), %%rax\n", idx);
    return buffer;
}


