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

char* asm_gen_unary_minus(asn* u_minus){
    asn* e = u_minus->op.unary_exp.val;
    char* inner = asm_gen(e);

    char* src = strnew();

    if(u_minus->op.unary_exp.type == at_real){
        strapp(&src, inner);
        strapp(&src, "    xorpd  %xmm1, %xmm1\n");
        strapp(&src, "    subsd  %xmm0, %xmm1\n");
        strapp(&src, "    movsd  %xmm1, %xmm0\n");
    } else {
        strapp(&src, inner);
        strapp(&src, "    negq   %rax\n");
    }

    free(inner);
    return src;
}

char* asm_gen_unary_not(asn* u_not){
    char* src = strnew();
    asn* e = u_not->op.unary_exp.val;
    char* inner = asm_gen(e);
    strapp(&src, inner);
    strapp(&src, "    cmpq   $0, %rax\n");
    strapp(&src, "    movq   $0, %rax\n");
    strapp(&src, "    sete   %al\n");
    return src;
}

char* asm_gen_unary_compl(asn* u_compl){
    char* src = strnew();
    asn* e = u_compl->op.unary_exp.val;
    char* inner = asm_gen(e);
    strapp(&src, inner);
    strapp(&src, "    not    %rax\n");
    return src;
}

char* asm_gen_reference(asn* ref){
    printf("generating reference\n");
    asn* var = ref->op.unary_exp.val;
    const char* id = var->op.var_ref_exp.ident;
    pv_leaf* leaf = pv_search(symbol_map_ptr, id);

    char* src = salloc(80);

    sprintf(src, "    leaq   %d(%%rbp), %%rax\n", -(leaf->offset+leaf->size));
    return src;
}

char* asm_gen_dereference(asn* deref){
    printf("generating dereference\n");
    asn* var = deref->op.unary_exp.val;

	int off = 0;
    char* src = salloc(80);

	if(var->tag == var_ref_tag){
	    const char* id = var->op.var_ref_exp.ident;
	    pv_leaf* leaf = pv_search(symbol_map_ptr, id);
		off = -(leaf->offset+leaf->size);
	} else {
	}
    sprintf(src, "    movq   %d(%%rbp), %%rbx\n", off);
    strapp(&src, "    movq   (%rbx), %rax\n");
    return src;
}

char* asm_gen_inc(asn* inc){
    asn* inner = inc->op.unary_exp.val;
    char* inner_src = asm_gen(inner);

    char* src = strnew();
    strapp(&src, inner_src);
    strapp(&src, "    addq   $1, %rax\n");

    if(inner->tag == var_ref_tag){
        const char* id = inner->op.var_ref_exp.ident;
        pv_leaf* leaf = pv_search(symbol_map_ptr, id);
        int off = -(leaf->offset + leaf->size);
        char* line = salloc(80);
        if(leaf->scope != 0)
            sprintf(line, "    movq   %%rax, %d(%%rbp)\n", off);
        else
            sprintf(line, "    movq   %%rax, %s(%%rip)\n", id);
        strapp(&src, line);
        free(line);
    } else if(inner->tag == deref_tag){
        strapp(&src, "    movq   %rax, (%rbx)\n");
    }

    free(inner_src);
    return src;
}

char* asm_gen_dec(asn* dec){
    asn* inner = dec->op.unary_exp.val;
    char* inner_src = asm_gen(inner);

    char* src = strnew();
    strapp(&src, inner_src);
    strapp(&src, "    subq   $1, %rax\n");

    if(inner->tag == var_ref_tag){
        const char* id = inner->op.var_ref_exp.ident;
        pv_leaf* leaf = pv_search(symbol_map_ptr, id);
        int off = -(leaf->offset + leaf->size);
        char* line = salloc(80);
        if(leaf->scope != 0)
            sprintf(line, "    movq   %%rax, %d(%%rbp)\n", off);
        else
            sprintf(line, "    movq   %%rax, %s(%%rip)\n", id);
        strapp(&src, line);
        free(line);
    } else if(inner->tag == deref_tag){
        strapp(&src, "    movq   %rax, (%rbx)\n");
    }

    free(inner_src);
    return src;
}

