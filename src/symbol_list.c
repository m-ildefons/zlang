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


const char* decl_type_cn[] = {
    [decl_pointer] = "ptr",
    [decl_array] = "array",
    [decl_function] = "func",
};

const char* sclass_cn[] = {
    [sclass_auto] = "auto",
};

const char* type_cn[] = {
    [type_void] = "void",
    [type_bool] = "bool",
    [type_char] = "char",
    [type_int] = "int",
    [type_real] = "real",
    [type_signed] = "signed",
    [type_unsigned] = "unsigned",
    [type_complex] = "complex",
    [type_string] = "string",
	[type_struct] = "struct",
	[type_union] = "union",
	[type_enum] = "enum",
};


declarator* new_declarator(){
    declarator* d = malloc(sizeof(declarator));
    assert(d != NULL);
    d->size = 0;
    return d;
}

declarator* copy_declarator(const declarator* decl)
{
    declarator* d = new_declarator();
    d->type = decl->type;
    d->size = decl->size;
    return d;
}

specifier* new_specifier(){
    specifier* s = malloc(sizeof(specifier));
    assert(s != NULL);
    return s;
}

specifier* copy_specifier(const specifier* spec)
{
    specifier* s = new_specifier();
    s->type = spec->type;
    s->storage_class = spec->storage_class;
    s->output_class = spec->output_class;
    return s;
}

type_link* new_type_link(){
    type_link* tl = malloc(sizeof(type_link));
    assert(tl != NULL);
    tl->next = NULL;
    return tl;
}

type_link* copy_type_link(const type_link* link)
{
    type_link* l = new_type_link();
    l->cls = link->cls;
    if (link->cls == cls_decl) {
        l->type.decl = copy_declarator(link->type.decl);
    } else if (link->cls == cls_spec) {
        l->type.spec = copy_specifier(link->type.spec);
    }
    l->next = NULL;
    return l;
}

symbol* new_symbol(const char* id){
    symbol* s = malloc(sizeof(symbol));
    assert(s != NULL);
    s->ref_count = 1;
    s->ident = strdup(id);
    s->scope = 0;
    s->stype = NULL;
    s->etype = NULL;

    // negative location ==> no valid location
	s->data_loc = -1;
    s->mem_loc = -1;
    s->reg_loc = -1;
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
    sl->ref_count = 1;

    symbol* ph = new_symbol(".placeholder");
    symbol_list_append(&sl, &ph);
    delete_symbol(&ph);

    return sl;
}

void delete_declarator(declarator** d){
    if((*d) == NULL)
        return;

    free((*d));
}

void delete_specifier(specifier** s){
    if((*s) == NULL)
        return;

    free((*s));
}

void delete_type_link(type_link** tl){
    if((*tl) == NULL)
        return;

    if((*tl)->cls == cls_decl){
        delete_declarator(&((*tl)->type.decl));
    } else if((*tl)->cls == cls_spec){
        delete_specifier(&((*tl)->type.spec));
    }

    free((*tl));
    (*tl) = NULL;
}

void delete_symbol(symbol** s){
    if((*s) == NULL)
        return;

    (*s)->ref_count--;
    if((*s)->ref_count > 0)
        return;

    type_link* link = (*s)->stype;
    type_link* prev = NULL;
    while(link != NULL){
        prev = link;
        link = link->next;
        delete_type_link(&prev);
    }

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


    sl->ref_count--;
    if(sl->ref_count > 0)
        return;

    if(sl->top == NULL && sl->bottom == NULL){
        free(sl);
        return;
    }

    if(sl->bottom->next != NULL){
        sl->bottom->next->ref_count--;
        sl->bottom->next = NULL;
    }

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

void print_type_link(const type_link* t){
    if(t->cls == cls_decl){
        printf("%s", decl_type_cn[t->type.decl->type]);
    } else if(t->cls == cls_spec){
        printf("%s %s",
            sclass_cn[t->type.spec->storage_class],
            type_cn[t->type.spec->type]);
    }
}

void print_symbol_list_entry(const symbol_list_entry* e){
    printf("[%zu (%zu)] %s",
        e->ref_count,
        e->sym->scope,
        e->sym->ident);
    type_link* link = e->sym->stype;
    if(link == NULL){
        printf(" (NULL)");
    } else {
        printf(" [");
        for(; link != NULL; link = link->next){
            print_type_link(link);
            if(link->next != NULL)
                printf(" ");
        }
        printf("]");
    }

    printf(" d/%d(%%rip) m/%d(%%rbp) r/%d",
		e->sym->data_loc,
		e->sym->mem_loc,
		e->sym->reg_loc);
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
		printf("\n");
    }
    printf("--\n");
}

int is_function(type_link* l){
    if(l == NULL)
        return 0;

    if(l->cls == cls_decl && l->type.decl->type == decl_function)
        return 1;

    return 0;
}

void type_link_attach(symbol** s, type_link* l){
    type_link* last = (*s)->stype;
    while(last != NULL && last->next != NULL)
        last = last->next;

    if(last == NULL)
        (*s)->stype = l;
    else
        last->next = l;

    (*s)->etype = l;
}

void copy_type_list(const symbol* src, symbol** dst)
{
    type_link* l = src->stype;
    for (; l != NULL; l = l->next) {
        type_link* c = copy_type_link(l);
        type_link_attach(dst, c);
        (*dst)->etype = c;
    }
}

void copy_return_type(const symbol* func, symbol** dst)
{
    type_link* l = func->stype;
    for (; l != NULL; l = l->next) {
        if (l->cls != cls_decl || l->type.decl->type != decl_function) {
            type_link* c = copy_type_link(l);
            type_link_attach(dst, c);
            (*dst)->etype = c;
        }
    }
}

void symbol_list_insert(symbol_list** sl, symbol_list_entry** e){
    if((*sl)->top == NULL){
        (*sl)->top = (*e);
        (*e)->ref_count++;
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
    if((*sl)->top != NULL && strcmp((*sl)->top->sym->ident, ".placeholder") == 0){
        delete_symbol(&((*sl)->top->sym));
        (*sl)->top->sym = (*s);
        (*s)->ref_count++;
        return;
    }

    symbol_list_entry* e = new_symbol_list_entry();
    e->sym = (*s);
    e->sym->ref_count++;
    symbol_list_insert(sl, &e);
    delete_symbol_list_entry(&e);
}

void symbol_list_attach(symbol_list** outer, symbol_list** inner){
    (*inner)->bottom->next = (*outer)->top;
    if((*outer)->top != NULL)
        (*outer)->top->ref_count++;
}

symbol* search_symbol(const symbol_list* sl, const char* key){
    symbol_list_entry* ptr = sl->top;
    for(; ptr != NULL; ptr = ptr->next){
        if(strcmp(ptr->sym->ident, key) == 0)
            return ptr->sym;
    }
    return NULL;
}

