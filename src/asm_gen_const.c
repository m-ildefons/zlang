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

const char* asm_gen_int_const(asn* int_const){
    printf("generating const int\n");
    int num = int_const->op.int_exp;
    char* _str = (char*) malloc(20 * sizeof(char));
    sprintf(_str, "%d", num);

	size_t bufsize = strlen(_str) + 1;
    char* ret = (char*) malloc(bufsize * sizeof(char));
    sprintf(ret, "    movq   $%d, %%rax\n", num);
    return ret;
}

const char* asm_gen_float_const(asn* float_const){
    printf("generating const float\n");
    int idx = float_const->op.int_exp;
    char* buffer = (char*) malloc(80 * sizeof(char));

    sprintf(buffer, "    movsd  .FC%d(%%rip), %%xmm0\n", idx);
    return buffer;
}

const char* asm_gen_string_const(asn* str_const){
    printf("generating const string\n");
    int idx = str_const->op.int_exp;
    char* buffer = (char*) malloc(80 * sizeof(char));

    sprintf(buffer, "    leaq   .SC%d(%%rip), %%rax\n", idx);
    return buffer;
}


