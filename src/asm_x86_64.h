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
#include "parse.h"
#include "ast.h"
#include "strop.h"
#include "storage_location.h"
#include "assembly.h"

#define NUM_REGISTERS 0x10

#define RAX 0x00
#define RCX 0x01
#define RDX 0x02
#define RBX 0x03
#define RSP 0x04
#define RBP 0x05
#define RSI 0x06
#define RDI 0x07
#define R8  0x08
#define R9  0x09
#define R10 0x0A
#define R11 0x0B
#define R12 0x0C
#define R13 0x0D
#define R14 0x0E
#define R15 0x0F

#define XMM0  0x10
#define XMM1  0x11
#define XMM2  0x12
#define XMM3  0x13
#define XMM4  0x14
#define XMM5  0x15
#define XMM6  0x16
#define XMM7  0x17
#define XMM8  0x18
#define XMM9  0x19
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
char* asm_x86_64_addr(const quadruple* q);
char* asm_x86_64_from(const quadruple* q);
char* asm_x86_64_into(const quadruple* q);

char* asm_x86_64_label(const quadruple* q);
char* asm_x86_64_jump(const quadruple* q);

char* asm_x86_64_compare(const quadruple* q);

char* asm_x86_64_set(const quadruple* q);

char* asm_x86_64_arith(const quadruple* q);

char* asm_x86_64_bit(const quadruple* q);
char* asm_x86_64_compl(const quadruple* q);

char* push_stack(symbol* sym);
char* pop_stack(symbol* sym);

void emit_push_stack(char** code, symbol* sym);
void emit_pop_stack(char** code, symbol* sym);

int get_register(void);
int get_xmm_register(void);
void set_register(int reg, symbol* s);
void print_registers(void);

#endif

