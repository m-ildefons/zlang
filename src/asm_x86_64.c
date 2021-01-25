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
static symbol* xmm_usage[NUM_REGISTERS];

static const char* registers[] = {
  "rax", "rcx", "rdx", "rbx", "rsp", "rbp", "rsi", "rdi",
  "r8", "r9", "r10", "r11", "r12", "r13", "r14", "r15",
  "xmm0", "xmm1", "xmm2", "xmm3", "xmm4", "xmm5", "xmm6", "xmm7",
  "xmm8", "xmm9", "xmm10", "xmm11", "xmm12", "xmm13", "xmm14", "xmm15"};
static size_t rel_stack_pos;
static size_t frame_size;
static size_t size_of[type_enum + 1] = {
  [type_void] = 8,
  [type_bool] = 1,
  [type_char] = 1,
  [type_int] = 8,
  [type_real] = 8,
  [type_string] = 8,
};

char* gen_asm_x86_64(const quad_list* IC) {
  memset(usage, 0, 8 * sizeof(symbol*));
  char* code = strnew();
  const quad_list* ql = IC;
  char* snippet = NULL;
  unsigned int idx;

  print_symbol_list(symbol_list_ptr);

  char* bname = basename(filename);
  strapp(&code, "    .file     \"%s\"\n", bname);
  strapp(&code,
         "    .ident    \"Zlang %d.%d.%s\"\n",
         __ZLANG_MAJ__,
         __ZLANG_MIN__,
         __ZLANG_SUB__);

  for (; ql != NULL; ql = ql->next) {
    switch (ql->quad->op) {
      case fac_func_start: snippet = asm_x86_64_func_start(ql->quad); break;
      case fac_func_end: snippet = asm_x86_64_func_end(ql->quad); break;
      case fac_call: snippet = asm_x86_64_func_call(ql->quad); break;
      case fac_return: snippet = asm_x86_64_return(ql->quad); break;
      case fac_label: snippet = asm_x86_64_label(ql->quad); break;
      case fac_jump:
      case fac_je:
      case fac_jne: snippet = asm_x86_64_jump(ql->quad); break;
      case fac_load: snippet = asm_x86_64_load(ql->quad); break;
      case fac_store: snippet = asm_x86_64_store(ql->quad); break;
      case fac_addr: snippet = asm_x86_64_addr(ql->quad); break;
      case fac_from: snippet = asm_x86_64_from(ql->quad); break;
      case fac_into: snippet = asm_x86_64_into(ql->quad); break;
      case fac_compare: snippet = asm_x86_64_compare(ql->quad); break;
      case fac_setl:
      case fac_setle:
      case fac_setg:
      case fac_setge:
      case fac_sete:
      case fac_setne: snippet = asm_x86_64_set(ql->quad); break;
      case fac_add:
      case fac_sub:
      case fac_mul:
      case fac_div:
      case fac_mod:
      case fac_neg: snippet = asm_x86_64_arith(ql->quad); break;
      case fac_shl: snippet = asm_x86_64_bit(ql->quad); break;
      case fac_shr: snippet = asm_x86_64_bit(ql->quad); break;
      case fac_and: snippet = asm_x86_64_bit(ql->quad); break;
      case fac_xor: snippet = asm_x86_64_bit(ql->quad); break;
      case fac_or: snippet = asm_x86_64_bit(ql->quad); break;
      case fac_compl: snippet = asm_x86_64_compl(ql->quad); break;
      default:
        printf("Warning: Encountered unimplemented intermediary code");
        printf(" during x86_64 asm generation.\n");
        continue;
    }
    strapp(&code, "%s", snippet);
    printf("%s\n", snippet);
    free(snippet);
  }

  symbol_list_entry* e;
  char* data = strnew();
  strapp(&data, "\n");
  strapp(&data, "    .section  .data\n");
  for (e = symbol_list_ptr->top; e != NULL; e = e->next) {
    if (!is_function(e->sym->stype)) {
      strapp(&data, "    .global    %s\n", e->sym->ident);
      strapp(&data, "%s:\n", e->sym->ident);
      strapp(&data, "    .long      %d\n", e->sym->reg_loc);
      strapp(&data, "    .align     8\n");
      strapp(&data, "    .type      %s, @object\n", e->sym->ident);
    }
  }
  strapp(&code, data);
  free(data);

  if (string_count > 0 || real_count > 0) {
    strapp(&code, "\n");
    strapp(&code, "    .section  .rodata\n");

    for (idx = 0; idx < string_count; idx++) {
      strapp(&code, ".SC%u:\n", idx);
      strapp(&code, "    .string   \"%s\"\n", string_index[idx]);
    }

    for (idx = 0; idx < real_count; idx++) {
      unsigned long bits = *((unsigned long*) &(real_index[idx]));
      unsigned int upper = ((unsigned int*) &bits)[0];
      unsigned int lower = ((unsigned int*) &bits)[1];
      strapp(&code, ".RC%u:\n", idx);
      strapp(&code, "    .align    8\n");
      strapp(&code, "    .long     %u\n", upper);
      strapp(&code, "    .long     %u\n", lower);
    }
  }

  free(bname);
  return code;
}

char* asm_x86_64_func_start(const quadruple* q) {
  char* code = strnew();
  strapp(&code, "\n");
  strapp(&code, "    .section  .text\n");
  strapp(&code, "    .global   %s\n", q->arg1->ident);
  strapp(&code, "    .type     %s, @function\n", q->arg1->ident);
  strapp(&code, "%s:\n", q->arg1->ident);
  strapp(&code, "    pushq     %%rbp\n");
  strapp(&code, "    movq      %%rsp, %%rbp\n");

  frame_size = 0;
  symbol_list_entry* e = q->symbol_list_ptr->top;
  for (; e != q->symbol_list_ptr->bottom->next; e = e->next) {
    if (e->sym->etype != NULL) {
      frame_size += size_of[e->sym->etype->type.spec->type];
      e->sym->mem_loc = frame_size;
    }
    print_symbol_list_entry(e);
    printf("\n");
  }
  for (e = q->temp_list_ptr->top;
       e != q->temp_list_ptr->bottom->next;
       e = e->next) {
//      if(e->sym->etype != NULL){
//          frame_size += size_of[e->sym->etype->type.spec->type];
//          e->sym->mem_loc = frame_size;
//      }
    print_symbol_list_entry(e);
    printf("\n");
  }
  printf("%s: %zu\n", q->arg1->ident, frame_size);

  if (frame_size > 0)
    strapp(&code, "    subq      $%zu, %%rsp\n", frame_size);

  const char* reg = NULL;
  char* src;
  int i;
  for (i = 0, e = q->args->top; e != NULL; e = e->next, i++) {
//printf("%d %s, %d %d\n", i, e->sym->ident, e->sym->mem_loc, e->sym->reg_loc);
    src = strnew();
    switch (i) {
      case 0: reg = "rdi"; set_register(RDI, NULL);  break;
      case 1: reg = "rsi"; set_register(RSI, NULL);  break;
      case 2: reg = "rdx"; set_register(RDX, NULL);  break;
      case 3: reg = "rcx"; set_register(RCX, NULL);  break;
      case 4: reg = "r8"; set_register(R8, NULL); break;
      case 5: reg = "r9"; set_register(R9, NULL); break;
      default:
        reg = NULL;
        strapp(&src, "%d(%%rbp)", 8 * (i - 4)); // rbp + 16 is the first argument
        break;
    }
    if(reg != NULL && e->sym->mem_loc > 0){
      strapp(&code,
        "    movq      %%%s, -%d(%%rbp)\n",
        reg,
        e->sym->mem_loc);
    } else if(e->sym->mem_loc > 0){
      strapp(&code,
        "    movq      %s, %%rbx\n", src);
      strapp(&code,
        "    movq      %%rbx, -%d(%%rbp)\n",
        e->sym->mem_loc);
    }
    free(src);
  }

  set_register(RAX, NULL);
    return code;
}

char* asm_x86_64_func_end(const quadruple* q) {
  char* code = strnew();
  emit(&code, "movq", "$0", "%rax");
  emit(&code, "movq", "%rbp", "%rsp");
  emit(&code, "popq", "%rbp", NULL);
  emit(&code, "ret", NULL, NULL);
  strapp(&code, "    .size     %s, .-%s\n", q->arg1->ident, q->arg1->ident);
  return code;
}

char* asm_x86_64_func_call(const quadruple* q){
  char* code = strnew();

  if(usage[RAX] != NULL){
    emit_push_stack(&code, usage[RAX]);
    set_register(RAX, NULL);
  }

  int i;
  int num_args;
  char* src;
  char* dest;

  symbol_list_entry* e = q->args->top;
  for(num_args = 0, e = q->args->top; e != NULL; e = e->next, num_args++){
    int reg = 0;
    switch(num_args){
      case 1: reg = RDI; break;
      case 2: reg = RSI; break;
      case 3: reg = RDX; break;
      case 4: reg = RCX; break;
      case 5: reg = R8; break;
      case 6: reg = R9; break;
      default: reg = 0; break;
    }

    if(reg != 0 && usage[reg] != NULL){
      emit_push_stack(&code, usage[reg]);
      set_register(reg, NULL);
    }
  }

  int padding;
  if (num_args > 6)
    padding = (8 * (num_args - 6) + (int) (frame_size % 16)) % 16;
  else
    padding = (int) (frame_size % 16);

  if(padding != 0)
    strapp(&code,
      "    subq      $%d, %%rsp  # Frame padding to 16 byte boundary\n",
      padding);
  printf("Found %d arguments, frame size: %zu\n", num_args, frame_size);
  int k;
  for(k = num_args; k > 0; k--){
    for(i = 0, e = q->args->top; i < k-1; i++, e = e->next){}

    if(strcmp(e->sym->ident, ".placeholder") == 0)
      continue;

    dest = strnew();
    src = strnew();

    if(e->sym->reg_loc < 0 && e->sym->mem_loc >= 0){
      strapp(&src, "-%d(%%rbp)", e->sym->mem_loc);
    } else if (e->sym->data_loc >= 0) {
      strapp(&src, "%s(%%rip)", e->sym->ident);
    } else if(e->sym->reg_loc >= 0){
      strapp(&src, "%%%s", registers[e->sym->reg_loc]);
//      set_register(e->sym->reg_loc, NULL);
    }

    switch(k){
      case 1: strapp(&dest, "%%rdi"); break;
      case 2: strapp(&dest, "%%rsi"); break;
      case 3: strapp(&dest, "%%rdx"); break;
      case 4: strapp(&dest, "%%rcx"); break;
      case 5: strapp(&dest, "%%r8"); break;
      case 6: strapp(&dest, "%%r9"); break;
      default:
        strapp(&code, "    subq      $8, %%rsp\n");
        strapp(&dest, "(%%rsp)");
    }

    if(k > 6){
      strapp(&code, "    movq      %s, %%rax  # load arg no %d\n", src, k);
      strapp(&code, "    movq      %%rax, %s  #\n", dest);
    } else {
      strapp(&code, "    movq      %s, %s  # argument no %d\n", src, dest, k);
    }

    printf("Argument %d: %s (%s --> %s)\n", k, e->sym->ident, src, dest);
    free(dest);
    free(src);
  }

  print_registers();
  printf("Frame size: %zu (%zu)\n", frame_size, frame_size % 16);
  printf("Misalignment: %d bytes\n", (8 * (num_args - 6)) % 16);
  strapp(&code, "    movq      $0, %%rax\n");
  strapp(&code, "    call      %s\n", q->arg1->ident);

  if(padding != 0)
    strapp(&code,
      "    addq      $%d, %%rsp  # remove frame padding\n",
      padding);

  set_register(RAX, q->res);
  set_register(RDI, NULL);
  set_register(RSI, NULL);
  set_register(RDX, NULL);
  set_register(RCX, NULL);
  set_register(R8, NULL);
  set_register(R9, NULL);
  return code;
}

char* asm_x86_64_return(const quadruple* q){
    char* code = strnew();
    if(q->arg1 == NULL){
        strapp(&code, "    movq      $0, %%rax\n");
    } else if(q->arg1->reg_loc > 0){
        strapp(&code,
            "    movq      %%%s, %%rax\n",
            registers[q->arg1->reg_loc]);
    }

    strapp(&code, "    movq      %%rbp, %%rsp\n");
    strapp(&code, "    popq      %%rbp\n");
    strapp(&code, "    ret\n");
  set_register(RAX, NULL);
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
  //int xmm_reg = get_xmm_register();
  //printf("%s --> %s/%s\n", q->arg1->ident, registers[reg], registers[NUM_REGISTERS + xmm_reg]);

  if(usage[reg] != NULL){
    strapp(&code, "    subq      $8, %%rsp\n");
    strapp(&code, "    movq      %%%s, (%%rsp)\n", registers[reg]);

    rel_stack_pos += 8;
    usage[RAX]->mem_loc = rel_stack_pos;
  }
  if(q->arg1 == NULL){
    strapp(&code, "    movq      (%%rsp), %%%s\n", registers[reg]);
    strapp(&code, "    addq      $8, %%rsp\n");
  } else if(q->arg1->mem_loc < 0 && q->arg1->data_loc < 0){
    strapp(&code,
           "    movq      $%s, %%%s\n",
           q->arg1->ident,
           registers[reg]);
  } else if(q->arg1->mem_loc < 0 && q->arg1->data_loc >= 0) {
    if (q->arg1->stype->cls == cls_decl &&
        q->arg1->stype->type.decl->type == decl_pointer) {
      strapp(&code,
             "    leaq      %s(%%rip), %%%s\n",
             q->arg1->ident,
             registers[reg]);
    } else if (q->arg1->stype->cls == cls_spec &&
               q->arg1->stype->type.spec->type == type_string) {
      strapp(&code,
             "    leaq      %s(%%rip), %%%s\n",
             q->arg1->ident,
             registers[reg]);
    } else {
      strapp(&code,
             "    movq      %s(%%rip), %%%s\n",
             q->arg1->ident,
             registers[reg]);
    }
  } else {
    strapp(&code,
           "    movq      -%d(%%rbp), %%%s\n",
           q->arg1->mem_loc,
           registers[reg]);
  }

  set_register(reg, q->res);
  return code;
}

char* asm_x86_64_store(const quadruple* q) {
  char* code = strnew();
  if(q->res == NULL){
    strapp(&code, "    subq      $8, %%rsp\n");
    strapp(&code, "    movq      %%%s, (%%rsp)\n", registers[q->arg1->reg_loc]);
  } else if (q->arg1->reg_loc < 0) {
    strapp(&code, "    movq      %%rax, -%d(%%rbp)\n", q->res->mem_loc);
  } else if (q->res->mem_loc < 0) {
    frame_size += 8;
    strapp(&code, "    subq      $8, %%rsp\n");
    strapp(&code, "    movq      %%%s, (%%rsp)\n", registers[q->arg1->reg_loc]);
    q->res->mem_loc = frame_size;
  } else {
    strapp(&code,
           "    movq      %%%s, -%d(%%rbp)  # store\n",
           registers[q->arg1->reg_loc],
           q->res->mem_loc);
  }
  set_register(q->arg1->reg_loc, NULL);
  return code;
}

char* asm_x86_64_addr(const quadruple* q) {
  char* code = strnew();
  int reg = get_register();
  strapp(&code,
         "    leaq      -%d(%rbp), %%%s\n",
         q->arg1->mem_loc,
         registers[reg]);
  set_register(reg, q->res);
  return code;
}

char* asm_x86_64_from(const quadruple* q) {
  char* code = strnew();
  int reg = get_register();
  strapp(&code,
         " movq (%%%s), %%%s\n",
         registers[q->arg1->reg_loc],
         registers[reg]);
  set_register(reg, q->res);
  return code;
}

char* asm_x86_64_into(const quadruple* q) {
  char* code = strnew();
  int reg = get_register();
  if (q->res->mem_loc <= 0) {
    strapp(&code,
           " movq (%%%s), %%%s\n",
           registers[q->res->reg_loc],
           registers[reg]);
  } else if (q->res->reg_loc <= 0) {
    strapp(&code,
           " movq -%d(%rbp), %%%s\n",
           q->res->mem_loc,
           registers[reg]);

  }
  strapp(&code,
         " movq %%%s, (%%%s)\n",
         registers[q->arg1->reg_loc],
         registers[reg]);
  set_register(reg, q->res);
  set_register(q->arg1->reg_loc, NULL);
  return code;
}

char* asm_x86_64_jump(const quadruple* q) {
  char* code = strnew();
  const char* jmp;
  switch (q->op) {
    case fac_jump: jmp = "jmp "; break;
    case fac_je: jmp = "je  "; break;
    case fac_jne: jmp = "jne "; break;
    default: break;
  }
  strapp(&code, "    %s      %s\n", jmp, q->arg1->ident);
  set_register(RAX, NULL);
  return code;
}

char* asm_x86_64_compare(const quadruple* q) {
  char* code = strnew();
  if (q->arg1->reg_loc < 0) {
    strapp(&code,
      "    cmpq      $%s, %%%s\n",
      q->arg1->ident,
      registers[q->arg2->reg_loc]);
  } else {
    strapp(&code,
      "    cmpq      %%%s, %%%s\n",
      registers[q->arg2->reg_loc],
      registers[q->arg1->reg_loc]);
  }
  return code;
}

char* asm_x86_64_set(const quadruple* q){
  char* code = strnew();
  const char* instr = NULL;
  strapp(&code, "    movq      $0, %%rax\n");
  switch(q->op){
    case fac_setl: instr = "setl "; break;
    case fac_setle: instr = "setle"; break;
    case fac_setg: instr = "setg "; break;
    case fac_setge: instr = "setge"; break;
    case fac_sete: instr = "sete "; break;
    case fac_setne: instr = "setne"; break;
    default: break;
  }
  strapp(&code, "    %s     %%al\n", instr);
  set_register(RAX, q->res);
  return code;
}

char* asm_x86_64_arith(const quadruple* q){
  char* code = strnew();
  const char* op;
  switch(q->op){
    case fac_add: op = "addq "; break;
    case fac_sub: op = "subq "; break;
    case fac_mul: op = "imulq"; break;
    case fac_div: op = "idivq"; break;
    case fac_mod: op = "idivq"; break;
    case fac_neg: op = "negq "; break;
    default: abort();
  }

  if(q->arg1 != NULL && q->arg1->reg_loc < 0){
    char* ps = pop_stack(q->arg1);
    strapp(&code, "%s", ps);
    free(ps);
  }

  if(q->arg2 != NULL && q->arg2->reg_loc < 0){
    char* ps = pop_stack(q->arg2);
    strapp(&code, "%s", ps);
    free(ps);
  }

  if(q->op== fac_div || q->op == fac_mod)
    strapp(&code, "    cqo\n");

  if(q->op == fac_neg){
    strapp(&code,
      "    %s     %%%s\n",
      op,
      registers[q->arg1->reg_loc]);
  } else {
    strapp(&code,
      "    %s     %%%s, %%%s\n",
      op,
      registers[q->arg2->reg_loc],
      registers[q->arg1->reg_loc]);
  }

  if(q->op == fac_div){
    set_register(RAX, q->res);
  } else if(q->op == fac_mod){
    set_register(RDX, q->res);
  } else if(q->op == fac_neg){
    set_register(q->arg1->reg_loc, q->res);
  } else {
    set_register(q->arg1->reg_loc, q->res);
    set_register(q->arg2->reg_loc, NULL);
  }

  return code;
}

char* asm_x86_64_bit(const quadruple* q){
  char* code = strnew();
  const char* instr = NULL;
  const char* reg = NULL;
  switch(q->op){
    case fac_shl: instr = "shlq"; reg = "cl"; break;
    case fac_shr: instr = "shrq"; reg = "cl"; break;
    case fac_and: instr = "andq"; reg = registers[q->arg2->reg_loc]; break;
    case fac_xor: instr = "xorq"; reg = registers[q->arg2->reg_loc]; break;
    case fac_or: instr = "orq "; reg = registers[q->arg2->reg_loc]; break;
    default: instr = "#";
  }

  strapp(&code,
         "    %s      %%%s, %%%s\n",
         instr,
         reg,
         registers[q->arg1->reg_loc]);
  set_register(q->arg1->reg_loc, q->res);
  return code;
}

char* asm_x86_64_compl(const quadruple* q){
  char* code = strnew();
  strapp(&code, "    not       %%%s\n", registers[q->arg1->reg_loc]);
  set_register(q->arg1->reg_loc, q->res);
  return code;
}

char* push_stack(symbol* sym){
  char* code = strnew();
  strapp(&code, "    subq      $8, %%rsp\n");
  strapp(&code, "    movq      %%%s, (%%rsp)\n", registers[sym->reg_loc]);
  frame_size += 8;
  sym->mem_loc = frame_size;
  return code;
}

char* pop_stack(symbol* sym){
  char* code = strnew();
  int reg = get_register();
  strapp(&code, "    movq      (%%rsp), %%%s\n", registers[reg]);
  strapp(&code, "    addq      $8, %%rsp\n");
  frame_size -= 8;
  set_register(reg, sym);
  return code;
}

void emit_push_stack(char** code, symbol* sym){
  char* ps = push_stack(sym);
  strapp(code, "%s", ps);
  free(ps);
}

void emit_pop_stack(char** code, symbol* sym){
  char* ps = pop_stack(sym);
  strapp(code, "%s", ps);
  free(ps);
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

int get_xmm_register(){
  int i;
  for(i = 0; i < NUM_REGISTERS; i++){
    if(xmm_usage[i] == NULL)
      return i;
  }
  return -1;
}

void set_register(int reg, symbol* s){
    if(usage[reg] != NULL)
        usage[reg]->reg_loc = -1;

    usage[reg] = s;

    if(s != NULL)
        s->reg_loc = reg;

//    print_registers();
}

void print_registers(){
    int i;
  for(i = 0; i < NUM_REGISTERS; i++){
    if(usage[i] != NULL)
      printf("> %s: %s\n", registers[i], usage[i]->ident);
    if(xmm_usage[i] != NULL)
      printf("> %s: %s\n", registers[i+0x10], xmm_usage[i]->ident);
  }
}

