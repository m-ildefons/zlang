
#ifndef __ELF_H__
#define __ELF_H__

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <elf.h>
#include <assert.h>

#include "strop.h"
#include "zlang.h"


typedef struct elf_section {
    Elf64_Shdr* header;
    unsigned char* data;
    size_t size;
} elf_section;

void write_binary(void);
void section_header_table_append(const char* name, Elf64_Shdr header);

#endif

