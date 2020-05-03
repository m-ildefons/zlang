/*
 * FILENAME: symbol_list.h
 *
 * DESCRIPTION:
 *     Delarations for datastructures and functions for symbol lists.
 *
 *     This implementation is based on 'split headed linked lists'. That means,
 *     it is basically a linked list, but also remembers its own bottom entry,
 *     which then may link to the top entry of another such list. This allowes
 *     mutiple lists to link to the same bottom list, thus making the whole list
 *     construction have a split head. With the top list containing inner scope
 *     symbols and the bottom list conatining more global scoped symbols, this
 *     mirrors the desired scoping behavior. Modifying a bottom list also
 *     'updates' information in the top lists. Shadowing works too, because
 *     being single linked lists, the innermost scope is always searched first.
 *
 *     Saving a pointer to the last entry of the list itself (the last of the
 *     inner most scope), one can easily determine which scope one is currently
 *     looking at.
 *
 *     The only drawback a priori is that the time for searching is linear with
 *     the sum of entries in all lists. If this turns out to be a performance
 *     bottleneck, shortlists saving the last accessed entries can be used to
 *     accellerate this structure a bit.
 *
 * AUTHOR: Moritz Röhrich <moritzr@pool.math.tu-berlin.de>
 *
 */

#ifndef __SYMBOL_LIST_H__
#define __SYMBOL_LIST_H__

#include <stdio.h>
#include <assert.h>

#include "strop.h"
#include "int_stack.h"
#include "storage_location.h"


/* Declarator
 */
typedef struct {
    enum {
        decl_pointer,
        decl_array,
        decl_function,
    } type;
    /* this holds array size/stack frame size; ununsed when type == pointer */
    size_t size;
} declarator;

/* Specifier
 */
typedef struct {
    enum {
        type_void,
        type_bool,
        type_char,
    	type_int,
        type_real,
        type_signed,
        type_unsigned,
        type_complex,
        type_string,
        type_struct,
        type_union,
        type_enum,
    } type;
    enum {
        sclass_auto,
        sclass_register,
        sclass_static,
        sclass_extern,
        sclass_inline,
    } storage_class;
    enum {
        oclass_auto,
        oclass_public,
        oclass_private,
        oclass_extern,
        oclass_common,
    } output_class;
} specifier;

typedef struct s_type_link {
    enum {
        cls_decl,
        cls_spec,
    } cls;
    union {
        declarator* decl;
        specifier* spec;
    } type;
    struct s_type_link* next;
} type_link;

extern const char* decl_type_cn[];
extern const char* sclass_cn[];
extern const char* type_cn[];

/* Symbol
 *
 * All you need to know about something described by an identifier
 */
typedef struct {
    size_t ref_count;
    char* ident;
    size_t scope;
    type_link* stype;
    type_link* etype;
    slocation* loc;
} symbol;

/* Symbol List Entry
 *
 * This is just a management structure ancapsulating the actual symbol list
 */
typedef struct s_symbol_list_entry_t {
    struct s_symbol_list_entry_t* next;
    size_t ref_count;
    symbol* sym;
} symbol_list_entry;

/* Symbol List
 *
 * The symbol list remembers top, bottom and scope of symbol list of the
 * current scope.
 */
typedef struct s_symbol_list_t {
    struct s_symbol_list_entry_t* top;
    struct s_symbol_list_entry_t* bottom;
    size_t scope;
    size_t ref_count;
} symbol_list;

/* imeplemented in symbol_list.c */
declarator* new_declarator(void);
declarator* copy_declarator(const declarator* decl);
specifier* new_specifier(void);
specifier* copy_specifier(const specifier* spec);
type_link* new_type_link(void);
type_link* copy_type_link(const type_link* link);
symbol* new_symbol(const char* id);
symbol_list_entry* new_symbol_list_entry(void);
symbol_list* new_symbol_list(size_t scope);

void delete_declarator(declarator** d);
void delete_specifier(specifier** s);
void delete_type_link(type_link** tl);
void delete_symbol(symbol** s);
void delete_symbol_list_entry(symbol_list_entry** e);
void delete_symbol_list(symbol_list* sl);

void print_type_link(const type_link* t);
void print_symbol_list_entry(const symbol_list_entry* e);
void print_symbol_list(const symbol_list* sl);

int is_function(type_link* l);

void type_link_attach(symbol** s, type_link* l);
void copy_type_list(const symbol* src, symbol** dst);
void copy_return_type(const symbol* func, symbol** dst);
void symbol_list_insert(symbol_list** sl, symbol_list_entry** e);
void symbol_list_append(symbol_list** sl, symbol** s);
void symbol_list_attach(symbol_list** outer, symbol_list** inner);
symbol* search_symbol(const symbol_list* sl, const char* key);

#endif

