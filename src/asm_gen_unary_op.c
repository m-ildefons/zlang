/*
 * FILENAME: asm_gen_unary_op.c
 *
 * DESCRIPTION:
 *     Implementation of assembly generation of unary operators.
 *
 * AUTHOR: Moritz Röhrich <moritzr@pool.math.tu-berlin.de>
 *
 */

#include "asm_gen.h"

const char* asm_gen_unary_minus(asn* u_minus){
    char* line = (char*) malloc(80 * sizeof(char));
    asn* e = u_minus->op.unary_exp.expr;
    const char* inner = asm_gen(e);
    sprintf(line, "    negq   %%rax\n");
    size_t inner_len = strlen(inner);
    char* ret = (char*) malloc((80 + inner_len) * sizeof(char));
    strcpy(ret, inner);
    strcat(ret, line);
    return ret;
}

const char* asm_gen_unary_not(asn* u_not){
    char* lines = (char*) malloc(240 * sizeof(char));
    asn* e = u_not->op.unary_exp.expr;
    const char* inner = asm_gen(e);
    strcpy(lines, "    cmpq   $0, %rax\n");
    strcat(lines, "    movq   $0, %rax\n");
    strcat(lines, "    sete   %al\n");
    size_t inner_len = strlen(inner);
    char* ret = (char*) malloc((240 + inner_len) * sizeof(char));
    strcpy(ret, inner);
    strcat(ret, lines);
    return ret;
}

const char* asm_gen_unary_compl(asn* u_compl){
    char* line = (char*) malloc(80 * sizeof(char));
    asn* e = u_compl->op.unary_exp.expr;
    const char* inner = asm_gen(e);
    sprintf(line, "    not    %%rax\n");
    size_t inner_len = strlen(inner);
    char* ret = (char*) malloc((80 + inner_len) * sizeof(char));
    strcpy(ret, inner);
    strcat(ret, line);
    return ret;
}

const char* asm_gen_reference(asn* ref){
    printf("generating reference\n");
    asn* var = ref->op.unary_exp.expr;
    const char* id = var->op.var_ref_exp.ident;
    pv_leaf* leaf = pv_search(symbol_map_ptr, id);

    char* src = (char*) malloc(80 * sizeof(char));
    assert(src != NULL);

    sprintf(src, "    leaq   %d(%%rbp), %%rax\n", -(leaf->offset+leaf->size));
    return src;
}

const char* asm_gen_dereference(asn* deref){
    printf("generating dereference\n");
    asn* var = deref->op.unary_exp.expr;
    const char* id = var->op.var_ref_exp.ident;
    pv_leaf* leaf = pv_search(symbol_map_ptr, id);

    char* src = (char*) malloc(160 * sizeof(char));
    assert(src != NULL);

    sprintf(src, "    movq   %d(%%rbp), %%rbx\n", -(leaf->offset+leaf->size));
    strcat(src, "    movq   (%rbx), %rax\n");
    printf("%s\n", src);
    return src;
}

const char* asm_gen_inc(asn* inc){
    asn* inner = inc->op.unary_exp.expr;
    const char* inner_src = asm_gen(inner);
    size_t inner_len = strlen(inner_src);

    char* src = (char*) malloc((inner_len + 80) * sizeof(char));
    strcpy(src, inner_src);
    strcat(src, "    addq   $1, %rax\n");

    if(inner->tag == var_ref_tag){
        const char* id = inner->op.var_ref_exp.ident;
        pv_leaf* leaf = pv_search(symbol_map_ptr, id);
        int off = -(leaf->offset + leaf->size);
        char* line = (char*) malloc(80 * sizeof(char));
        if(leaf->scope != 0)
            sprintf(line, "    movq   %%rax, %d(%%rbp)\n", off);
        else
            sprintf(line, "    movq   %%rax, %s(%%rip)\n", id);
        src = realloc(src, (strlen(src) + 81) * sizeof(char));
        strcat(src, line);
    } else if(inner->tag == deref_tag){
        strapp(&src, "    movq   %rax, (%rbx)\n");
    }

    return src;
}

const char* asm_gen_dec(asn* dec){
    printf("generating decrement\n");
    const char* id = "";
    pv_leaf* leaf = NULL;
    int off = 0;
    char* line = NULL;
    asn* inner = dec->op.unary_exp.expr;
    const char* inner_src = asm_gen(inner);
    size_t inner_len = strlen(inner_src);

    char* src = (char*) malloc((inner_len + 80) * sizeof(char));
    strcpy(src, inner_src);
    strcat(src, "    subq   $1, %rax\n");

    if(inner->tag == var_ref_tag){
        id = inner->op.var_ref_exp.ident;
        leaf = pv_search(symbol_map_ptr, id);
        off = -(leaf->offset + leaf->size);
        line = (char*) malloc(80 * sizeof(char));
        if(leaf->scope != 0)
            sprintf(line, "    movq   %%rax, %d(%%rbp)\n", off);
        else
            sprintf(line, "    movq   %%rax, %s(%%rip)\n", id);
        src = realloc(src, (strlen(src) + 81) * sizeof(char));
        strcat(src, line);
    } else if(inner->tag == deref_tag){
        strapp(&src, "    movq   %rax, (%rbx)\n");
    }

    return src;
}

