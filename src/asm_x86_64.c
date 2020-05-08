/*
 * FILENAME: asm_x86_64.h
 *
 * DESCRIPTION
 *     Implementation of functions for assembly generation for x86_64 platform.
 *
 * AUTHOR: Moritz Röhrich <moritzr@pool.math.tu-berlin.de>
 *
 */

#include "asm_x86_64.h"


static symbol* usage[NUM_REGISTERS];
static size_t rel_stack_pos;
static size_t size_of[type_enum + 1] = {
    [type_void] = 8,
    [type_bool] = 1,
    [type_char] = 1,
    [type_int] = 8,
    [type_real] = 8,
};

char* gen_asm_x86_64(const quad_list* IC){
    memset(usage, 0, 8 * sizeof(symbol*));
    char* code = strnew();
    const quad_list* ql = IC;
    char* snippet = NULL;

    char* bname = basename(filename);
    strapp(&code, "    .file     \"%s\"\n", bname);

    for(; ql != NULL; ql = ql->next){
        switch(ql->quad->op){
            case fac_func_start: snippet = asm_x86_64_func_start(ql->quad); break;
            case fac_func_end: snippet = asm_x86_64_func_end(ql->quad); break;
            //case fac_call: break;
            case fac_return: snippet = asm_x86_64_return(ql->quad); break;
            case fac_label: snippet = asm_x86_64_label(ql->quad); break;
            case fac_jump:
            case fac_je:
            case fac_jne: snippet = asm_x86_64_jump(ql->quad); break;
            case fac_load: snippet = asm_x86_64_load(ql->quad); break;
            //case fac_store: break;
            case fac_compare: snippet = asm_x86_64_compare(ql->quad); break;
            case fac_setl:
            case fac_setle:
            case fac_setg:
            case fac_setge:
            case fac_sete:
            case fac_setne: snippet = asm_x86_64_set(ql->quad); break;
            //case fac_assign: break;
            case fac_add: snippet = asm_x86_64_add(ql->quad); break;
            case fac_sub: snippet = asm_x86_64_sub(ql->quad); break;
            case fac_mul: snippet = asm_x86_64_mul(ql->quad); break;
            //case fac_div: break;
            //case fac_mod: break;
            //case fac_neg: break;
            default:
                printf("Warning: Encountered unimplemented intermediary code");
                printf(" during x86_64 asm generation.\n");
                continue;
        }
        strapp(&code, snippet);
        free(snippet);
    }

    strapp(&code,
        "    .ident    \"Zlang %d.%d.%s\"\n",
        __ZLANG_MAJ__,
        __ZLANG_MIN__,
        __ZLANG_SUB__);

    free(bname);
    return code;
}

char* asm_x86_64_func_start(const quadruple* q){
    char* code = strnew();
    strapp(&code, "    .text\n");
    strapp(&code, "    .global   ");
    strapp(&code, q->arg1->ident);
    strapp(&code, "\n");
    strapp(&code, "    .type     ");
    strapp(&code, q->arg1->ident);
    strapp(&code, ", @function\n");
    strapp(&code, q->arg1->ident);
    strapp(&code, ":\n");
    strapp(&code, "    pushq     %%rbp\n");
    strapp(&code, "    movq      %%rsp, %%rbp\n");

    size_t frame_size = 0;
    symbol_list_entry* e = q->symbol_list_ptr->top;
    for(; e != q->symbol_list_ptr->bottom->next; e = e->next){
        if(e->sym->etype != NULL){
            frame_size += size_of[e->sym->etype->type.spec->type];
            delete_slocation(e->sym->loc);
            e->sym->loc = new_slocation(mem_stack, frame_size);
        }

        print_symbol_list_entry(e);
    }
    for(e = q->temp_list_ptr->top; e != q->temp_list_ptr->bottom->next; e = e->next){
        if(e->sym->etype != NULL){
            frame_size += size_of[e->sym->etype->type.spec->type];
            delete_slocation(e->sym->loc);
            e->sym->loc = new_slocation(mem_stack, frame_size);
        }
        print_symbol_list_entry(e);

    }
    printf("%s: %zu\n", q->arg1->ident, frame_size);

    strapp(&code, "    subq      $%zu, %%rsp\n", frame_size);
    return code;
}

char* asm_x86_64_func_end(const quadruple* q){
    char* code = strnew();
    strapp(&code, "    movq      $0, %rax\n");
    strapp(&code, "    movq      %rbp, %rsp\n");
    strapp(&code, "    popq      %rbp\n");
    strapp(&code, "    ret\n");
    strapp(&code, "    .size     %s, .-%s\n", q->arg1->ident, q->arg1->ident);
    return code;
}

char* asm_x86_64_return(const quadruple* q){
    char* code = strnew();
    if(q->arg1 == NULL)
        strapp(&code, "    movq      $0, %rax\n");

    strapp(&code, "    movq      %rbp, %rsp\n");
    strapp(&code, "    popq      %rbp\n");
    strapp(&code, "    ret\n");
    return code;
}

char* asm_x86_64_label(const quadruple* q){
    char* code = strnew();
    strapp(&code, "%s:\n", q->arg1->ident);
    return code;
}

char* asm_x86_64_load(const quadruple* q){
    char* code = strnew();
    if(usage[RAX] != NULL){
        strapp(&code, "    subq      $8, %rsp\n");
        strapp(&code, "    movq      %rax, (%rsp)\n");

        rel_stack_pos += 8;
        usage[RAX]->loc->type = mem_stack;
        usage[RAX]->loc->pos = rel_stack_pos;
    }
    strapp(&code, "    movq      $%s, %%rax\n", q->arg1->ident);

    set_register(RAX, q->res);
    return code;
}

char* asm_x86_64_jump(const quadruple* q){
    char* code = strnew();
    switch(q->op){
        case fac_jump: strapp(&code, "    jmp       "); break;
        case fac_je: strapp(&code, "    je        "); break;
        case fac_jne: strapp(&code, "    jne       "); break;
        default: break;
    }
    strapp(&code, "%s\n", q->arg1->ident);
    return code;
}

char* asm_x86_64_compare(const quadruple* q){
    char* code = strnew();
    strapp(&code, "    cmpq      $%s, %%rax\n", q->arg1->ident);
    return code;
}

char* asm_x86_64_set(const quadruple* q){
    char* code = strnew();
    strapp(&code, "    movq      $0, %rax\n");
    switch(q->op){
        case fac_setl: strapp(&code, "    setl      "); break;
        case fac_setle: strapp(&code, "    setle     "); break;
        case fac_setg: strapp(&code, "    setg      "); break;
        case fac_setge: strapp(&code, "    setge     "); break;
        case fac_sete: strapp(&code, "    sete      "); break;
        case fac_setne: strapp(&code, "    setne     "); break;
        default: break;
    }
    strapp(&code, "%al\n");
    return code;
}

char* asm_x86_64_add(const quadruple* q){
    char* code = strnew();
    if(q->arg1->loc != NULL && q->arg1->loc->type != mem_register){
        strapp(&code, "    movq      -%zu(%%rbp), %%rdx\n", q->arg1->loc->pos);
    }
    if(q->arg2->loc != NULL && q->arg2->loc->type != mem_register){
    }
    strapp(&code, "    addq      %rdx, %rax\n");
    set_register(RAX, q->res);
    return code;
}

char* asm_x86_64_sub(const quadruple* q){
    char* code = strnew();
    if(q->arg1->loc != NULL && q->arg1->loc->type != mem_register){
        strapp(&code, "    movq      -%zu(%%rbp), %%rdx\n", q->arg1->loc->pos);
    }
    if(q->arg2->loc != NULL && q->arg2->loc->type != mem_register){
    }
    strapp(&code, "    subq      %rdx, %rax\n");
    set_register(RAX, q->res);
    return code;
}

char* asm_x86_64_mul(const quadruple* q){
    char* code = strnew();
    if(q->arg1->loc->type != mem_register){
        strapp(&code, "    movq      -%zu(%%rbp), %%rdx\n", q->arg1->loc->pos);
    }
    if(q->arg2->loc->type != mem_register){
    }
    strapp(&code, "    imulq     %rdx, %rax\n");
    set_register(RAX, q->res);
    return code;
}

void set_register(int reg, symbol* s){
    usage[reg] = s;
    if(s->loc != NULL){
        s->loc->type = mem_register;
        s->loc->pos = RAX;
    } else {
        s->loc = new_slocation(mem_register, RAX);
    }
    print_registers();
}

void print_registers(){
    int i;
    printf("|   RAX   |   RBX   |   RCX   |   RDX   |");
    printf("   RBP   |   RSP   |   RDI   |   RSI   |\n");
    for(i = 0; i < NUM_REGISTERS; i++){
        if(usage[i] != NULL){
            int padlen = 8 - strlen(usage[i]->ident);
            printf("|%*.*s%s ",
                padlen,
                padlen,
                " ",
                usage[i]->ident);
        }
        else
            printf("|%*.*s ", 8, 8, " ");
    }
    printf("|\n");
}

