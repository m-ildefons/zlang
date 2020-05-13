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
static const char* registers[] = {"rax", "rcx", "rdx", "rbx", "rsp", "rbp", "rsi", "rdi"};
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
            case fac_store: snippet = asm_x86_64_store(ql->quad); break;
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
        strapp(&code, "%s", snippet);
        printf("\n%s", snippet);
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
    strapp(&code, "    .global   %s\n", q->arg1->ident);
    strapp(&code, "    .type     %s, @function\n", q->arg1->ident);
    strapp(&code, "%s:\n", q->arg1->ident);
    strapp(&code, "    pushq     %%rbp\n");
    strapp(&code, "    movq      %%rsp, %%rbp\n");

    size_t frame_size = 0;
    symbol_list_entry* e = q->symbol_list_ptr->top;
    for(; e != q->symbol_list_ptr->bottom->next; e = e->next){
        if(e->sym->etype != NULL){
            frame_size += size_of[e->sym->etype->type.spec->type];
            e->sym->mem_loc = frame_size;
        }
        print_symbol_list_entry(e);
    }
    for(e = q->temp_list_ptr->top; e != q->temp_list_ptr->bottom->next; e = e->next){
        if(e->sym->etype != NULL){
//            frame_size += size_of[e->sym->etype->type.spec->type];
//            e->sym->mem_loc = frame_size;
        }
        print_symbol_list_entry(e);
    }
    printf("%s: %zu\n", q->arg1->ident, frame_size);

    strapp(&code, "    subq      $%zu, %%rsp\n", frame_size);
    return code;
}

char* asm_x86_64_func_end(const quadruple* q){
    char* code = strnew();
    strapp(&code, "    movq      $0, %%rax\n");
    strapp(&code, "    movq      %%rbp, %%rsp\n");
    strapp(&code, "    popq      %%rbp\n");
    strapp(&code, "    ret\n");
    strapp(&code, "    .size     %s, .-%s\n", q->arg1->ident, q->arg1->ident);
    return code;
}

char* asm_x86_64_return(const quadruple* q){
    char* code = strnew();
    if(q->arg1 == NULL)
        strapp(&code, "    movq      $0, %%rax\n");

    else if(q->arg1->reg_loc > 0)
        strapp(&code,
            "    movq      %%%s, %%rax\n",
            registers[q->arg1->reg_loc]);

    strapp(&code, "    movq      %%rbp, %%rsp\n");
    strapp(&code, "    popq      %%rbp\n");
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

    int reg = get_register();
    printf("%s --> %s\n", q->arg1->ident, registers[reg]);

    if(usage[reg] != NULL){
        strapp(&code, "    subq      $8, %%rsp\n");
        strapp(&code, "    movq      %%%s, (%%rsp)\n", registers[reg]);

        rel_stack_pos += 8;
        usage[RAX]->mem_loc = rel_stack_pos;
    }
    if(q->arg1->mem_loc < 0){
        strapp(&code,
            "    movq      $%s, %%%s\n",
            q->arg1->ident,
            registers[reg]);
    } else {
        strapp(&code,
            "    movq      -%d(%%rbp), %%%s\n",
            q->arg1->mem_loc,
            registers[reg]);
    }

    set_register(reg, q->res);
    return code;
}

char* asm_x86_64_store(const quadruple* q){
    char* code = strnew();
    strapp(&code, "    movq      %%rax, -%d(%%rbp)\n", q->res->mem_loc);
    set_register(q->arg1->reg_loc, NULL);
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
    strapp(&code, "    movq      $0, %%rax\n");
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
    strapp(&code,
        "    addq      %%%s, %%%s\n",
        registers[q->arg2->reg_loc],
        registers[q->arg1->reg_loc]);
    set_register(q->arg1->reg_loc, q->res);
    return code;
}

char* asm_x86_64_sub(const quadruple* q){
    char* code = strnew();
    strapp(&code,
        "    subq      %%%s, %%%s\n",
        registers[q->arg2->reg_loc],
        registers[q->arg1->reg_loc]);
    set_register(q->arg1->reg_loc, q->res);
    return code;
}

char* asm_x86_64_mul(const quadruple* q){
    char* code = strnew();
    strapp(&code,
        "    imulq     %%%s, %%%s\n",
        registers[q->arg2->reg_loc],
        registers[q->arg1->reg_loc]);
    set_register(q->arg1->reg_loc, q->res);
    return code;
}

int get_register(){
    int i;
    for(i = 0; i < NUM_REGISTERS; i++){
        if(i == RSP || i == RBP)
            continue;

        if(usage[i] == NULL)
            return i;
    }
    return -1; // No register found --> Error
}

void set_register(int reg, symbol* s){
    if(usage[reg] != NULL)
        usage[reg]->reg_loc = -1;

    usage[reg] = s;
    if(s != NULL)
        s->reg_loc = reg;

    //print_registers();
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

