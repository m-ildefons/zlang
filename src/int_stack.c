/*
 * FILENAME: int_stack.c
 *
 * DESCRIPTION:
 *     Implementation of a stack of integers.
 *
 * AUTHOR: Moritz Röhrich <moritzr@pool.math.tu-berlin.de>
 *
 */

#include "int_stack.h"

void push(int_stack** s, int i){
    int_stack* top = (int_stack*) malloc(sizeof(int_stack));
    top->next = (*s);
    top->val = i;
    (*s) = top;
}

void push_back(int_stack** s, int i){
    int_stack* new_bottom = (int_stack*) malloc(sizeof(int_stack));
    new_bottom->next = NULL;
    new_bottom->val = i;
    int_stack* bottom = (*s);
    while(bottom->next != NULL)
        bottom = bottom->next;
    bottom->next = new_bottom;
}

int pop(int_stack** s){
    int_stack* old = (*s);
    int val = old->val;

    if((*s)->next)
        (*s) = (*s)->next;
    else
        (*s) = NULL;

    free(old);
    return val;
}

int pop_back(int_stack** s){
    int_stack* bottom = (*s);
    int_stack* new_bottom = (*s);
    while(bottom->next != NULL)
        bottom = bottom->next;
    while(new_bottom->next != bottom)
        new_bottom = new_bottom->next;
    int ret = bottom->val;
    free(bottom);
    new_bottom->next = NULL;
    bottom = NULL;
    return ret;
}

int peek(int_stack** s){
    if((*s) != NULL)
        return (*s)->val;
    return 0;
}

