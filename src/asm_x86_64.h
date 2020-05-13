/*
 * FILENAME: asm_x86_64.h
 *
 * DESCRIPTION
 *     Declarations of functions for assembly generation for x86_64 platform.
 *
 * AUTHOR: Moritz Röhrich <moritzr@pool.math.tu-berlin.de>
 *
 */

#ifndef __ASM_X86_64_H__
#define __ASM_X86_64_H__

#include "util.h"
#include "zlang.h"
#include "ic.h"
#include "strop.h"
#include "storage_location.h"

#define RAX 0
#define RCX 1
#define RDX 2
#define RBX 3
#define RSP 4
#define RBP 5
#define RSI 6
#define RDI 7
#define NUM_REGISTERS 8


/* implemented in asm_x86_64.c */
char* gen_asm_x86_64(const quad_list* IC);

char* asm_x86_64_func_start(const quadruple* q);
char* asm_x86_64_func_end(const quadruple* q);
char* asm_x86_64_return(const quadruple* q);
char* asm_x86_64_label(const quadruple* q);
char* asm_x86_64_load(const quadruple* q);
char* asm_x86_64_store(const quadruple* q);

char* asm_x86_64_jump(const quadruple* q);

char* asm_x86_64_compare(const quadruple* q);

char* asm_x86_64_set(const quadruple* q);

char* asm_x86_64_add(const quadruple* q);
char* asm_x86_64_sub(const quadruple* q);
char* asm_x86_64_mul(const quadruple* q);

int get_register(void);
void set_register(int reg, symbol* s);
void print_registers(void);

#endif

