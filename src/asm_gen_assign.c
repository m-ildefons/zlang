/*
 * FILENAME: asm_gen_assign.c
 *
 * DESCRIPTION:
 *     Implementation of assembly generation of assignment operators.
 *
 * AUTHOR: Moritz Röhrich <moritzr@pool.math.tu-berlin.de>
 *
 */

#include "asm_gen.h"

char* asm_gen_assign(asn* assign){
    const char* id;
    if(assign->op.assign_exp.lval->tag == var_ref_tag)
        id = assign->op.assign_exp.lval->op.var_ref_exp.ident;
    else if(assign->op.assign_exp.lval->tag == deref_tag)
        id = assign->op.assign_exp.lval->op.unary_exp.val->op.var_ref_exp.ident;
    pv_leaf* leaf = pv_search(symbol_map_ptr, id);
    asn* val = assign->op.assign_exp.rval;

    int off = -(leaf->offset + leaf->size);
    char* rhs = asm_gen(val);

    char* put_line = salloc(80);
    assert(put_line != NULL);
    if(assign->op.assign_exp.lval->tag == var_ref_tag){
        if(leaf->scope != 0)
            sprintf(put_line, "    movq   %%rax, %d(%%rbp)\n", off);
        else
            sprintf(put_line, "    movq   %%rax, %s(%%rip)\n", id);
    } else if(assign->op.assign_exp.lval->tag == deref_tag){
        sprintf(put_line, "    movq   %%rax, (%%rbx)\n");
    }

    char* get_line = asm_gen(assign->op.assign_exp.lval);

    char* code = strnew();

    strapp(&code, rhs);
    if(assign->tag != assign_tag){
        strapp(&code, "    movq   %rax, %rcx\n");
        strapp(&code, get_line);
    }
    if(assign->op.assign_exp.lval->tag == deref_tag)
        strprp(&code, get_line);

    switch(assign->tag){
        case add_assign_tag: strapp(&code, "    addq   %rcx, %rax\n"); break;
        case sub_assign_tag: strapp(&code, "    subq   %rcx, %rax\n"); break;
        case mul_assign_tag: strapp(&code, "    imulq  %rcx, %rax\n"); break;
        case div_assign_tag: strapp(&code, "    cqo\n");
                             strapp(&code, "    idivq  %rcx, %rax\n"); break;
        case mod_assign_tag: strapp(&code, "    cqo\n");
                             strapp(&code, "    idivq  %rcx, %rax\n");
                             strapp(&code, "    movq   %rdx, %rax\n"); break;
        case and_assign_tag: strapp(&code, "    andq   %rcx, %rax\n"); break;
        case xor_assign_tag: strapp(&code, "    xorq   %rcx, %rax\n"); break;
        case or_assign_tag: strapp(&code, "    orq    %rcx, %rax\n"); break;
        case bsl_assign_tag: strapp(&code, "    shlq   %cl, %rax\n"); break;
        case bsr_assign_tag: strapp(&code, "    shrq   %cl, %rax\n"); break;
        default: break;
    }
    strapp(&code, put_line);

    free(rhs);
    free(get_line);
    free(put_line);
    return code;
}

