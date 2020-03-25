/*
 * FILENAME: ic.c
 *
 * DESCRIPTION
 *     Implementations for intermediate code.
 *
 * AUTHOR: Moritz Röhrich <moritzr@pool.math.tu-berlin.de>
 *
 */

#include "ic.h"


const char* fac_cn[] = {
    [fac_func_start] = "func start",
    [fac_func_end] = "func end",
    [fac_call] = "call",
    [fac_ret] = "return",
    [fac_label] = "label",
    [fac_jump] = "jump",
	[fac_je] = "jmp if eq",
	[fac_jne] = "jmp if neq",
    [fac_load] = "load",
    [fac_store] = "store",
	[fac_compare] = "compare",
    [fac_setl] = "set less",
    [fac_setle] = "set le or eq",
    [fac_setg] = "set greater",
    [fac_setge] = "set ge or eq",
    [fac_sete] = "set eq",
    [fac_setne] = "set neq",
    [fac_assign] = "assign",
    [fac_add] = "add",
    [fac_sub] = "sub",
    [fac_mul] = "mul",
    [fac_div] = "div",
    [fac_mod] = "mod",
    [fac_neg] = "neg",
};

quadruple* make_quad(int op,
                    symbol* arg1,
                    symbol* arg2,
                    symbol* res){
    quadruple* q = malloc(sizeof(quadruple));
    assert(q != NULL);
    q->op = op;

    q->arg1 = arg1;
    if(q->arg1 != NULL) // take ownership of the pointer since we are deleting.
        q->arg1->ref_count++;

    q->arg2 = arg2;
    if(q->arg2 != NULL)
        q->arg2->ref_count++;

    q->res = res;
    if(q->res != NULL)
        q->res->ref_count++;

    return q;
}

void delete_quad(quadruple* q){
    if(q == NULL)
        return;

    delete_symbol(&(q->arg1));
    delete_symbol(&(q->arg2));
    delete_symbol(&(q->res));

    free(q);
}

void print_quad(quadruple* q){
    if(q == NULL)
        return;

    int padlen;

    printf("\u2502");
    padlen = 12 - strlen(fac_cn[q->op]);
    printf("%*.*s%s", padlen, padlen, " ", fac_cn[q->op]);
    printf("\u2551");
    if(q->arg1 !=  NULL){
        padlen = 7 - strlen(atomic_type_cn[q->arg1->type]);
        printf("%*.*s%s", padlen, padlen, " ", atomic_type_cn[q->arg1->type]);
        printf("\u2502");
        padlen = 12 - strlen(q->arg1->ident);
        printf("%*.*s%s", padlen, padlen, " ", q->arg1->ident);
    } else {
        printf("       \u2502            ");
    }
    printf("\u2502");
    if(q->arg2 !=  NULL){
        padlen = 7 - strlen(atomic_type_cn[q->arg2->type]);
        printf("%*.*s%s", padlen, padlen, " ", atomic_type_cn[q->arg2->type]);
        printf("\u2502");
        padlen = 12 - strlen(q->arg2->ident);
        printf("%*.*s%s", padlen, padlen, " ", q->arg2->ident);
    } else {
        printf("       \u2502            ");
    }
    printf("\u2502");
    if(q->res !=  NULL){
        padlen = 7 - strlen(atomic_type_cn[q->res->type]);
        printf("%*.*s%s", padlen, padlen, " ", atomic_type_cn[q->res->type]);
        printf("\u2502");
        padlen = 12 - strlen(q->res->ident);
        printf("%*.*s%s", padlen, padlen, " ", q->res->ident);
    } else {
        printf("       \u2502            ");
    }
    printf("\u2502\n");
}

void quad_list_app_quad(quad_list** list, quadruple* q){
    quad_list* lp = (*list);
    quad_list* le = malloc(sizeof(quad_list));
    assert(le != NULL);
    le->next = NULL;
    le->quad = q;

    if(lp == NULL){
        (*list) = le;
        return;
    } else {
        while(lp->next != NULL)
            lp = lp->next;

        lp->next = le;
    }
}

void quad_list_app_quad_list(quad_list** list, quad_list* apx){
    quad_list* lp = (*list);

    if(lp == NULL){
        (*list) = apx;
        return;
    } else {
        while(lp->next != NULL)
            lp = lp->next;

        lp->next = apx;
    }
}

void delete_quad_list(quad_list* list){
    if(list == NULL)
        return;

    if(list->next != NULL)
        delete_quad_list(list->next);

    if(list->quad != NULL)
        delete_quad(list->quad);

    free(list);
}

void print_quad_list(quad_list* list){
    printf("\u2552");
    printf("\u2550\u2550\u2550\u2550\u2550\u2550");
    printf("\u2550\u2550\u2550\u2550\u2550\u2550");
    printf("\u2566");
    printf("\u2550\u2550\u2550\u2550\u2550\u2550");
    printf("\u2550\u2550\u2550\u2550\u2550\u2550");
    printf("\u2550\u2550\u2550\u2550\u2550\u2550");
    printf("\u2550\u2550");
    printf("\u2564");
    printf("\u2550\u2550\u2550\u2550\u2550\u2550");
    printf("\u2550\u2550\u2550\u2550\u2550\u2550");
    printf("\u2550\u2550\u2550\u2550\u2550\u2550");
    printf("\u2550\u2550");
    printf("\u2564");
    printf("\u2550\u2550\u2550\u2550\u2550\u2550");
    printf("\u2550\u2550\u2550\u2550\u2550\u2550");
    printf("\u2550\u2550\u2550\u2550\u2550\u2550");
    printf("\u2550\u2550");
    printf("\u2555\n");

    printf("\u2502");
    printf("%5.5s%s%5.5s", " ", "op", " ");
    printf("\u2551");
    printf("%8.8s%s%8.8s", " ", "arg1", " ");
    printf("\u2502");
    printf("%8.8s%s%8.8s", " ", "arg2", " ");
    printf("\u2502");
    printf("%9.9s%s%8.8s", " ", "res", " ");
    printf("\u2502\n");

    printf("\u255e");
    printf("\u2550\u2550\u2550\u2550\u2550\u2550");
    printf("\u2550\u2550\u2550\u2550\u2550\u2550");
    printf("\u256c");
    printf("\u2550\u2550\u2550\u2550\u2550\u2550");
    printf("\u2550\u2564\u2550\u2550\u2550\u2550");
    printf("\u2550\u2550\u2550\u2550\u2550\u2550");
    printf("\u2550\u2550");
    printf("\u256a");
    printf("\u2550\u2550\u2550\u2550\u2550\u2550");
    printf("\u2550\u2564\u2550\u2550\u2550\u2550");
    printf("\u2550\u2550\u2550\u2550\u2550\u2550");
    printf("\u2550\u2550");
    printf("\u256a");
    printf("\u2550\u2550\u2550\u2550\u2550\u2550");
    printf("\u2550\u2564\u2550\u2550\u2550\u2550");
    printf("\u2550\u2550\u2550\u2550\u2550\u2550");
    printf("\u2550\u2550");
    printf("\u2561\n");

    quad_list* qlp = NULL;
    for(qlp = list; qlp != NULL; qlp = qlp->next){
        print_quad(qlp->quad);
        if(qlp->quad->op == fac_func_end && qlp->next != NULL){
            printf("\u2502");
            printf("            ");
            printf("\u2551");
            printf("       \u2502            ");
            printf("\u2502");
            printf("       \u2502            ");
            printf("\u2502");
            printf("       \u2502            ");
            printf("\u2502\n");
        }
    }
    printf("\u2558");
    printf("\u2550\u2550\u2550\u2550\u2550\u2550");
    printf("\u2550\u2550\u2550\u2550\u2550\u2550");
    printf("\u2569");
    printf("\u2550\u2550\u2550\u2550\u2550\u2550");
    printf("\u2550\u2567\u2550\u2550\u2550\u2550");
    printf("\u2550\u2550\u2550\u2550\u2550\u2550");
    printf("\u2550\u2550");
    printf("\u2567");
    printf("\u2550\u2550\u2550\u2550\u2550\u2550");
    printf("\u2550\u2567\u2550\u2550\u2550\u2550");
    printf("\u2550\u2550\u2550\u2550\u2550\u2550");
    printf("\u2550\u2550");
    printf("\u2567");
    printf("\u2550\u2550\u2550\u2550\u2550\u2550");
    printf("\u2550\u2567\u2550\u2550\u2550\u2550");
    printf("\u2550\u2550\u2550\u2550\u2550\u2550");
    printf("\u2550\u2550");
    printf("\u255b\n");
}

