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
    printf("[ \"%c\" %X (%zu) ]", tree->ident, tree, tree->ref_count);
    if(tree->leaf){
        printf("\u2500[ (%d) %s (%d/%d) ]\n",
                tree->leaf->scope,
                tree->leaf->ident,
                tree->leaf->size,
                tree->leaf->offset);
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
    root->trie->ref_count++;
    root->size = 0;
    root->depth = 0;
    root->mem_offset = 0;
    root->key_list = NULL;
    root->scope = 0;
    return root;
}

pv_node* new_pv_node(){
    unsigned int i;
    pv_node* n = (pv_node*) malloc(sizeof(pv_node));
    assert(n != NULL);
    n->ident = ' ';
    n->ref_count = 0;
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
    l->ident = ident;
    l->type = type;
    l->size = size;
    l->offset = offset;
    l->scope = scope;
    return l;
}

pv_root* pv_insert(pv_root* old_trie, char* key, pv_leaf* leaf){
    pv_root* trie = new_trie();
    pv_node* node = trie->trie;
    char* next = key;
    char* next_handle = next;
    unsigned int idx;
    unsigned int i;
    pv_node* child;
    unsigned int new_depth = 0;

    if(strlen(next) < old_trie->depth){
        next = strpad(key, old_trie->depth, " ");
        next_handle = next;
    } else if(strlen(next) > old_trie->depth){
        idx = ' ';
        for(i = 1; i < (strlen(key) - old_trie->depth); i++){
            if(node->children[idx] == NULL){
                node->children[idx] = new_pv_node();
                node->children[idx]->ref_count++;
                node->children[idx]->ident = ' ';
                new_depth++;
            }
            node = node->children[idx];
        }
        node->children[idx] = old_trie->trie;
        old_trie->trie->ref_count++;

        trie->depth = new_depth;
        new_depth = 0;
        node = trie->trie;
    }

    if(strlen(next) <= old_trie->depth){
        char* ptr = next;
        idx = (unsigned int) (*ptr);
        for(i = 0; i < PV_BRANCHING_FACTOR; i++){
            trie->trie->children[i] = old_trie->trie->children[i];
            if(old_trie->trie->children[i] != NULL && i != idx)
                old_trie->trie->children[i]->ref_count++;
        }
        trie->depth = old_trie->depth;
    }

    trie->size = old_trie->size;
    trie->mem_offset = old_trie->mem_offset;
    trie->scope = old_trie->scope;

    ca_list* old_list = old_trie->key_list;
    if(old_list){
        for(; old_list != NULL; old_list = old_list->next){
            cal_append(&(trie->key_list), old_list->key);
        }
    }
    cal_append(&(trie->key_list), key);

    node = trie->trie;
    while(*next){
        idx = (unsigned int) (*next);
        child = node->children[idx];

        node->children[idx] = new_pv_node();
        node->children[idx]->ident = (*next);
        node->children[idx]->ref_count++;

        for(i = 0; i < PV_BRANCHING_FACTOR; i++){
            if(child != NULL){
                node->children[idx]->children[i] = child->children[i];
                if(child->children[i] != NULL && child->ref_count > 1)
                    child->children[i]->ref_count++;
            }
        }

        child = node->children[idx];
        new_depth++;

        if(*(next+1) == '\0'){
            if(child != NULL && child->leaf == NULL)
                trie->size++;
            else
                printf("creating leaf\n");
            child->leaf = leaf;
            if(leaf->size > 0)
                trie->mem_offset += leaf->size;
        }

        node = child;
        next++;
    }
    if(new_depth > trie->depth)
        trie->depth = new_depth;

    if(next_handle != key)
        free(next_handle);
    return trie;
}

pv_leaf* pv_search(pv_root* root, const char* key){
    char* next = salloc(strlen(key));
    char* next_handle = next;
    strcpy(next, key);
    size_t idx;
    pv_node* trie = root->trie;
    pv_node* pos = trie;

    if(strlen(next) < root->depth){
        free(next_handle);
        next = strpad(key, root->depth, " ");
        next_handle = next;
    } else if(strlen(next) > root->depth){
        free(next_handle);
        return NULL;
    }

    while(*next){
        idx = (size_t) (*next);
        if(*(next+1) == '\0'){
            if(pos->children[idx] != NULL && pos->children[idx]->leaf != NULL){
                free(next_handle);
                return pos->children[idx]->leaf;
            }
            free(next_handle);
            return NULL;
        }
        if(pos->children[idx] == NULL){
            free(next_handle);
            return NULL;
        }
        pos = pos->children[idx];
        next++;
    }

    free(next_handle);
    return NULL;
}

void delete_trie(pv_root* t){
    if(t == NULL)
        return;

    pv_pretty_print(t);

    if(t->trie != NULL){
        if(t->trie->ref_count < 2)
            delete_trie_node(t->trie);
        else
            t->trie->ref_count--;
    }

//    delete_ca_list(t->key_list);
    free(t);
    t = NULL;
}

void delete_trie_node(pv_node* n){
    if(n == NULL)
        return;

    printf("node \"%c\"\n", n->ident);
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

