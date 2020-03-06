/*
 * FILENAME: persistent_vector.c
 *
 * DESCRIPTION:
 *     Implementation of persistent vector data structure. Used for
 *     symbol maps.
 *
 * AUTHOR: Moritz Röhrich <moritzr@pool.math.tu-berlin.de>
 *
 */

#include "persistent_vector.h"

static void _pv_print_tabs(int_stack* ws_stack);
static void _pv_pretty_print(pv_node* tree, int_stack* ws_stack);

static void _pv_print_tabs(int_stack* ws_stack){
    int_stack* sp;
    for(sp = ws_stack; sp->next != NULL; sp = sp->next){
        if(sp->val)
            printf(" \u2502");
        else
            printf("  ");
    }

    if(sp == ws_stack)
        return;

    if(sp->val)
        printf(" \u251c\u2500");
    else
        printf(" \u2570\u2500");
}

static void _pv_pretty_print(pv_node* tree, int_stack* ws_stack){
    int i;
    int last_idx;

    _pv_print_tabs(ws_stack);
    printf("[ \"%c\" %lx (%zu) ]", tree->ident, (size_t) tree, tree->ref_count);
    if(tree->leaf){
        printf("\u2500[ (%d) %s (%d/%d) (%zu) ]\n",
                tree->leaf->scope,
                tree->leaf->ident,
                tree->leaf->size,
                tree->leaf->offset,
                tree->leaf->ref_count);
    } else {
        printf("\n");
    }

    for(i = PV_BRANCHING_FACTOR-1; i >= 0; i--){
        if(tree->children[i] != NULL){
            last_idx = i;
            break;
        }
    }

    for(i = 0; i < PV_BRANCHING_FACTOR; i++){
        if(tree->children[i] != NULL){
            if(i != last_idx)
                push_back(&ws_stack, 1);
            else
                push_back(&ws_stack, 0);
            _pv_pretty_print(tree->children[i], ws_stack);
            pop_back(&ws_stack);
        }
    }
}

void pv_pretty_print(pv_root* trie){
    int_stack* ws_stack = (int_stack*) malloc(sizeof(int_stack));
    assert(ws_stack != NULL);
    ws_stack->next = NULL;
    ws_stack->val = 0;

    printf("[ s %u / d %u / o %d ] \n",
            trie->size,
            trie->depth,
            trie->mem_offset);

    ca_list* l;
    for(l = trie->key_list; l != NULL; l = l->next){
        printf("[ %s ]\n", l->key);
    }

    push_back(&ws_stack, 0);
    _pv_pretty_print(trie->trie, ws_stack);
    pop_back(&ws_stack);
    free(ws_stack);
    ws_stack = NULL;
}

pv_root* new_trie(){
    pv_root* root = (pv_root*) malloc(sizeof(pv_root));
    assert(root != NULL);
    root->trie = new_pv_node();
    root->size = 0;
    root->depth = 0;
    root->mem_offset = 0;
    root->key_list = NULL;
    root->scope = 0;
    return root;
}

pv_root* copy_trie(pv_root* root){
    pv_root* trie = new_trie();
    delete_trie_node(trie->trie);
    trie->trie = new_pv_node();
    size_t i;
    for(i = 0; i < PV_BRANCHING_FACTOR; i++){
        if(root->trie->children[i] != NULL){
            trie->trie->children[i] = root->trie->children[i];
            root->trie->children[i]->ref_count++;
        }
    }
    trie->size = root->size;
    trie->depth = root->depth;
    trie->mem_offset = root->mem_offset;
    ca_list* l;
    for(l = root->key_list; l != NULL; l = l->next){
        cal_append(&(trie->key_list), l->key);
    }
    trie->scope = root->scope;
    return trie;
}

pv_node* new_pv_node(){
    unsigned int i;
    pv_node* n = (pv_node*) malloc(sizeof(pv_node));
    assert(n != NULL);
    n->ident = ' ';
    n->ref_count = 1;
    for(i = 0; i < PV_BRANCHING_FACTOR; i++)
        n->children[i] = NULL;
    n->leaf = NULL;
    return n;
}

pv_leaf* new_pv_leaf(char* ident,
                    atomic_type type,
                    int size,
                    int offset,
                    int scope){
    pv_leaf* l = (pv_leaf*) malloc(sizeof(pv_leaf));
    assert(l != NULL);
    l->ref_count = 1;
    l->ident = ident;
    l->type = type;
    l->size = size;
    l->offset = offset;
    l->scope = scope;
    return l;
}

pv_root* pv_insert(pv_root* old_trie, const char* key, pv_leaf* leaf){
    pv_root* trie = copy_trie(old_trie);
    pv_node* node = trie->trie;
    const char* next = key;
    int cidx;
    unsigned int i;

    for(next = key; (*next) != '\0'; next++){
        cidx = (int) (*next);
        if(node->children[cidx] == NULL){
            node->children[cidx] = new_pv_node();
            node->children[cidx]->ident = cidx;
        } else {
            pv_node* old_node = node->children[cidx];
            node->children[cidx] = new_pv_node();
            node->children[cidx]->ident = cidx;
            for(i = 0; i < PV_BRANCHING_FACTOR; i++){
                if(old_node->children[i] != NULL){
                    node->children[cidx]->children[i] = old_node->children[i];
                    old_node->children[i]->ref_count++;
                }
            }
            if(old_node->leaf != NULL){
                node->children[cidx]->leaf = old_node->leaf;
                old_node->leaf->ref_count++;
            }
            old_node->ref_count--;
        }
        node = node->children[cidx];

        if((*(next+1)) == '\0'){
            if(node->leaf != NULL)
                delete_trie_leaf(node->leaf);
            else
                cal_append(&(trie->key_list), key);

            if(leaf->size > 0)
                trie->mem_offset += leaf->size;

            node->leaf = leaf;
        }
    }

    return trie;
}

pv_leaf* pv_search(pv_root* root, const char* key){
    const char* next;
    size_t idx;
    pv_node* trie = root->trie;
    pv_node* pos = trie;

    for(next = key; (*next) != '\0'; next++){
        idx = (size_t) (*next);
        if(*(next+1) == '\0'){
            if(pos->children[idx] != NULL && pos->children[idx]->leaf != NULL){
                return pos->children[idx]->leaf;
            }
            return NULL;
        }
        if(pos->children[idx] == NULL){
            return NULL;
        }
        pos = pos->children[idx];
    }

    return NULL;
}

void delete_trie(pv_root* t){
    if(t == NULL)
        return;

    pv_pretty_print(t);

    if(t->trie != NULL && t->trie->ref_count < 2)
        delete_trie_node(t->trie);

    delete_ca_list(t->key_list);
    free(t);
    t = NULL;
}

void delete_trie_node(pv_node* n){
    if(n == NULL)
        return;

    unsigned int i;
    for(i = 0; i < PV_BRANCHING_FACTOR; i++){
        if(n->children[i] != NULL && n->children[i]->ref_count > 1){
            n->children[i]->ref_count--;
            continue;
        }
        delete_trie_node(n->children[i]);
    }

    delete_trie_leaf(n->leaf);
    free(n);
    n = NULL;
}

void delete_trie_leaf(pv_leaf* l){
    if(l == NULL)
        return;

    if(l->ref_count > 1){
        l->ref_count--;
        return;
    }

    free(l);
    l = NULL;
}

ca_list* new_ca_list(const char* key){
    ca_list* l = (ca_list*) malloc(sizeof(ca_list));
    assert(l != NULL);
    l->next = NULL;
    l->key = key;
    return l;
}

void delete_ca_list(ca_list* l){
    if(l == NULL)
        return;

    if(l->next != NULL)
        delete_ca_list(l->next);

    free(l);
}

void cal_append(ca_list** list, const char* key){
    ca_list* node = new_ca_list(key);
    if((*list) == NULL){
        (*list) = node;
        return;
    }
    ca_list* last = (*list);
    while(last->next != NULL)
        last = last->next;

    last->next = node;
}

