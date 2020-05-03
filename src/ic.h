/*
 * FILENAME: ic.h
 *
 * DESCRIPTION
 *     Declarations for intermediate code.
 *
 * AUTHOR: Moritz Röhrich <moritzr@pool.math.tu-berlin.de>
 *
 */

#ifndef __IC_H__
#define __IC_H__

#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <string.h>

#include "strop.h"
#include "symbol_list.h"


typedef struct four_address_code {
    enum {
        fac_func_start,
        fac_func_end,
        fac_call,
        fac_arg,
        fac_return,

        fac_label,
        fac_jump,

        fac_je,
        fac_jne,
        fac_load,
        fac_store,

        fac_compare,

        fac_setl,
        fac_setle,
        fac_setg,
        fac_setge,
        fac_sete,
        fac_setne,

        fac_assign,

        fac_add,
        fac_sub,
        fac_mul,
        fac_div,
        fac_mod,
        fac_neg,
    } op;
    symbol* arg1;
    symbol* arg2;
    symbol* res;
    symbol_list* symbol_list_ptr;
    symbol_list* temp_list_ptr;
} quadruple;

typedef struct quadruple_list {
    struct quadruple_list* next;
    quadruple* quad;
} quad_list;

extern const char* fac_cn[];

quadruple* make_quad(int op,
                    symbol* arg1,
                    symbol* arg2,
                    symbol* res);
void delete_quad(quadruple* q);
void print_quad(quadruple* q);

void quad_list_app_quad(quad_list** list, quadruple* q);
void quad_list_app_quad_list(quad_list** list, quad_list* apx);
void delete_quad_list(quad_list* list);
void print_quad_list(quad_list* list);

#endif

