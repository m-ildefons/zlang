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
#include "ast.h"
#include "strop.h"
#include "storage_location.h"

#define RAX 0x0
#define RCX 0x1
#define RDX 0x2
#define RBX 0x3
#define RSP 0x4
#define RBP 0x5
#define RSI 0x6
#define RDI 0x7
#define NUM_REGISTERS 8

#define R8 0x8
#define R9 0x9
#define R10 0xA
#define R11 0xB
#define R12 0xC
#define R13 0xD
#define R14 0xE
#define R15 0xF

#define XMM0 0x10
#define XMM1 0x11
#define XMM2 0x12
#define XMM3 0x13
#define XMM4 0x14
#define XMM5 0x15
#define XMM6 0x16
#define XMM7 0x17
#define XMM8 0x18
#define XMM9 0x19
#define XMM10 0x1A
#define XMM11 0x1B
#define XMM12 0x1C
#define XMM13 0x1D
#define XMM14 0x1E
#define XMM15 0x1F

/* implemented in asm_x86_64.c */
char* gen_asm_x86_64(const quad_list* IC);

char* asm_x86_64_func_start(const quadruple* q);
char* asm_x86_64_func_end(const quadruple* q);
char* asm_x86_64_func_call(const quadruple* q);
char* asm_x86_64_return(const quadruple* q);

char* asm_x86_64_load(const quadruple* q);
char* asm_x86_64_store(const quadruple* q);

char* asm_x86_64_label(const quadruple* q);
char* asm_x86_64_jump(const quadruple* q);

char* asm_x86_64_compare(const quadruple* q);

char* asm_x86_64_set(const quadruple* q);

char* asm_x86_64_arith(const quadruple* q);

char* asm_x86_64_bit(const quadruple* q);

char* push_stack(symbol* sym);
char* pop_stack(symbol* sym);

int get_register(void);
void set_register(int reg, symbol* s);
void print_registers(void);

#endif

