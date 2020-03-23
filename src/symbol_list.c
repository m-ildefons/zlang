/*
 * FILENAME: symbol_list.c
 *
 * DESCRIPTION:
 *     Implementations of functions handling the symbol lists.
 *
 * AUTHOR: Moritz Röhrich <moritzr@pool.math.tu-berlin.de>
 *
 */

#include "symbol_list.h"

symbol* new_symbol(const char* id, atomic_type type){
    symbol* s = malloc(sizeof(symbol));
    s->ref_count = 1;
    s->ident = strdup(id);
    s->scope = 0;
    s->type = type;
    return s;
}

symbol_list_entry* new_symbol_list_entry(){
    symbol_list_entry* e = malloc(sizeof(symbol_list_entry));
    assert(e != NULL);
    e->next = NULL;
    e->ref_count = 1;
    e->sym = NULL;
    return e;
}

symbol_list* new_symbol_list(size_t scope){
    symbol_list* sl = malloc(sizeof(symbol_list));
    assert(sl != NULL);
    sl->top = NULL;
    sl->bottom = NULL;
    sl->scope = scope;

    symbol* ph = new_symbol(".placeholder", at_void);
    symbol_list_append(&sl, &ph);
    delete_symbol(&ph);

    return sl;
}

void delete_symbol(symbol** s){
    if((*s) == NULL)
        return;

    (*s)->ref_count--;
    if((*s)->ref_count > 0)
        return;

    free((*s)->ident);
    free((*s));
    (*s) = NULL;
}

void delete_symbol_list_entry(symbol_list_entry** e){
    if((*e) == NULL)
        return;

    (*e)->ref_count--;
    if((*e)->ref_count > 0)
        return;

    if((*e)->sym != NULL)
        delete_symbol(&((*e)->sym));

    free((*e));
    (*e) = NULL;
}

void delete_symbol_list(symbol_list* sl){
    if(sl == NULL)
        return;

    if(sl->top == NULL && sl->bottom == NULL){
        free(sl);
        return;
    }

    if(sl->bottom->next != NULL)
        sl->bottom->next->ref_count--;

    sl->bottom->next = NULL;
    delete_symbol_list_entry(&(sl->bottom));

    symbol_list_entry* e = NULL;
    symbol_list_entry* next = NULL;

    e = sl->top;
    while(e != NULL){
        next = e->next;
        delete_symbol_list_entry(&e);
        e = next;
    }

    free(sl);
}

void print_symbol_list_entry(const symbol_list_entry* e){
    printf("[%zu] %s %s\n",
        e->sym->scope,
        atomic_type_cn[e->sym->type],
        e->sym->ident);
}

void print_symbol_list(const symbol_list* sl){
    printf("--\n");
    symbol_list_entry* e = sl->top;
    for(; e != NULL; e = e->next){
        if(e == sl->top)
            printf("t ->");
        else if(e == sl->bottom)
            printf("b ->");
        else
            printf("    ");
        print_symbol_list_entry(e);
    }
    printf("--\n");
}

void symbol_list_insert(symbol_list** sl, symbol_list_entry** e){
    if((*sl)->top == NULL){
        (*sl)->top = (*e);
        (*e)->ref_count++;
    } else if(strcmp((*sl)->top->sym->ident, ".placeholder") == 0){
        symbol_list_entry* ph = (*sl)->top;
        (*sl)->top = (*e);
        (*e)->ref_count++;
        (*sl)->bottom = (*e);
        (*e)->ref_count++;
        delete_symbol_list_entry(&ph);
        delete_symbol_list_entry(&ph);
        return;
    }

    if((*sl)->bottom != NULL){
        (*e)->next = (*sl)->bottom->next;
        (*sl)->bottom->next = (*e);
        (*sl)->bottom->next->ref_count++;
        (*sl)->bottom->ref_count--;
    }

    (*sl)->bottom = (*e);
    (*e)->ref_count++;
}

void symbol_list_append(symbol_list** sl, symbol** s){
    symbol_list_entry* e = new_symbol_list_entry();
    e->sym = (*s);
    e->sym->ref_count++;
    symbol_list_insert(sl, &e);
    delete_symbol_list_entry(&e);
}

symbol* search_symbol(const symbol_list* sl, const char* key){
    symbol_list_entry* ptr = sl->top;
    for(; ptr != NULL; ptr = ptr->next){
        if(strcmp(ptr->sym->ident, key) == 0)
            return ptr->sym;
    }
    return NULL;
}

