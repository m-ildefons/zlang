/*
 * FILENAME: int_stack.h
 *
 * DESCRIPTION:
 *     Declarations for a stack of integers.
 *
 * AUTHOR: Moritz Röhrich <moritzr@pool.math.tu-berlin.de>
 *
 */

#ifndef __INT_STACK_H__
#define __INT_STACK_H__

#include <stdlib.h>

typedef struct int_stack_s {
    struct int_stack_s* next;
    int val;
} int_stack;

int_stack* make_int_stack(void);
void delete_int_stack(int_stack* s);

void push(int_stack** s, int i);
void push_back(int_stack** s, int i);
int pop(int_stack** s);
int pop_back(int_stack** s);
int peek(int_stack** s);

#endif

