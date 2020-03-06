/*
 * FILENAME: persistent_vector.h
 *
 * DESCRIPTION:
 *     Declarations for persistent vector data structure used for symbol maps.
 *
 * AUTHOR: Moritz Röhrich <moritzr@pool.math.tu-berlin.de>
 *
 */

#ifndef __PERSISTENT_VECTOR_H__
#define __PERSISTENT_VECTOR_H__

#define PV_BITS 8
#define PV_BRANCHING_FACTOR 256

#include <stdio.h>
#include <string.h>
#include <assert.h>

#include "int_stack.h"
#include "strop.h"
#include "atomic_type.h"

typedef struct cal_node {
    struct cal_node* next;
    const char* key;
} ca_list;

typedef struct trie_leaf {
    size_t ref_count;
    char* ident;
    atomic_type type;
    int size;
    int offset;
    int scope;
} pv_leaf;

typedef struct trie_node {
    struct trie_node* children[PV_BRANCHING_FACTOR];
    size_t ref_count;
    char ident;
    pv_leaf* leaf;
} pv_node;

typedef struct trie_root_node {
    pv_node* trie;
    unsigned int size;
    unsigned int depth;
    int mem_offset;
    ca_list* key_list;
    int scope;
} pv_root;

void pv_pretty_print(pv_root* trie);
pv_root* new_trie(void);
pv_root* copy_trie(pv_root* root);
pv_node* new_pv_node(void);
pv_leaf* new_pv_leaf(char* ident,
                    atomic_type type,
                    int size,
                    int offset,
                    int scope);
pv_root* pv_insert(pv_root* old_trie, const char* key, pv_leaf* leaf);
pv_leaf* pv_search(pv_root* root, const char* key);
void delete_trie(pv_root* t);
void delete_trie_node(pv_node* n);
void delete_trie_leaf(pv_leaf* l);

ca_list* new_ca_list(const char* key);
void delete_ca_list(ca_list* l);
void cal_append(ca_list** list, const char* key);

#endif

