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
#include "persistent_vector.h"

typedef struct four_address_code {
    enum {
        fac_func_start,
        fac_func_end,
        fac_call,
        fac_ret,
        fac_load,
        fac_store,
        fac_assign,
        fac_add,
        fac_sub,
        fac_mul,
        fac_div,
        fac_mod,
    } op;
    pv_leaf* arg1;
    pv_leaf* arg2;
    pv_leaf* res;
} quadruple;

typedef struct quadruple_list {
    struct quadruple_list* next;
    quadruple* quad;
} quad_list;

extern const char* fac_cn[];

quadruple* make_quad(int op,
                    pv_leaf* arg1,
                    pv_leaf* arg2,
                    pv_leaf* res);
void delete_quad(quadruple* q);
void print_quad(quadruple* q);

void quad_list_app_quad(quad_list** list, quadruple* q);
void quad_list_app_quad_list(quad_list** list, quad_list* apx);
void delete_quad_list(quad_list* list);
void print_quad_list(quad_list* list);

#endif

