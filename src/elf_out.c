#include "elf_out.h"

char* section_names;
size_t section_names_size;
Elf64_Ehdr* elf_header;
Elf64_Shdr* section_header_table;
size_t section_header_table_size;

void write_binary(){
    unsigned int i;
    size_t len = 64;
    unsigned char buffer[len];

    elf_header = malloc(sizeof(Elf64_Ehdr));
    assert(elf_header != NULL);
    elf_header->e_ident[0x0] = ELFMAG0;
    elf_header->e_ident[0x1] = ELFMAG1;
    elf_header->e_ident[0x2] = ELFMAG2;
    elf_header->e_ident[0x3] = ELFMAG3;
    elf_header->e_ident[0x4] = ELFCLASS64;
    elf_header->e_ident[0x5] = ELFDATA2LSB;
    elf_header->e_ident[0x6] = EV_CURRENT;
    elf_header->e_ident[0x7] = ELFOSABI_LINUX;
    elf_header->e_ident[0x8] = EI_ABIVERSION;
    elf_header->e_ident[0x9] = 0x00;
    elf_header->e_ident[0xa] = 0x00;
    elf_header->e_ident[0xb] = 0x00;
    elf_header->e_ident[0xc] = 0x00;
    elf_header->e_ident[0xd] = 0x00;
    elf_header->e_ident[0xe] = 0x00;
    elf_header->e_ident[0xf] = 0x00;
    elf_header->e_type = ET_DYN;
    elf_header->e_machine = EM_X86_64;
    elf_header->e_version = EV_CURRENT;
    elf_header->e_entry = 0x60; // Entry
    elf_header->e_phoff = 0x40; // Offset P-header
    elf_header->e_shoff = 0x90; // Offset S-header
    elf_header->e_flags = 0x00; // Processor Flags
    elf_header->e_ehsize = sizeof(Elf64_Ehdr); // Eh size
    elf_header->e_phentsize = sizeof(Elf64_Phdr); // ph ent size
    elf_header->e_phnum = 0x1; // ph num
    elf_header->e_shentsize = sizeof(Elf64_Shdr);
    elf_header->e_shnum = 0x2;
    elf_header->e_shstrndx = 0x1;

    Elf64_Phdr p_header = {
        PT_LOAD,
        PF_X | PF_R,
        0x00,
        0x80000000,
        0x80000000,
        0xa0,
        0xa0,
        0x00,
    };

    Elf64_Shdr header0 = {
        0x00,
        SHT_NULL,
        0x00,
        0x00,
        0x00,
        0x00,
        0x00,
        0x00,
        0x00,
        0x00
    };

    Elf64_Shdr names_section = {
        0x1,
        SHT_STRTAB,
        0x0,
        0x0,
        0x80,
        strlen(".shstrtab") + 1,
        0x0,
        0x0,
        0x0,
        0x0
    };

    section_header_table_append("\0", header0);
    section_header_table_append(".shstrtab", names_section);

    FILE* fptr = fopen(elf_out, "wb");

    memset(buffer, 0, len);
    memcpy(buffer, elf_header, len);
    fwrite(buffer, len, 1, fptr);

    memset(buffer, 0, len);
    memcpy(buffer, &p_header, sizeof(Elf64_Phdr));
    fwrite(buffer, len, 1, fptr);

    fwrite(section_names, section_names_size, 1, fptr);

    printf("Written Section names: %zu\n", section_names_size);
    for(i = 0; i < (16 - section_names_size % 16); i++){
        printf("Padding\n");
        fwrite("\0", 1, 1, fptr);
    }

    fwrite(section_header_table, section_header_table_size, 1, fptr);
    printf("Written Section Header Table of size: %zu\n",
            section_header_table_size);

    fclose(fptr);
}

void section_header_table_append(const char* name, Elf64_Shdr header){
    if(section_names == NULL){
        section_names = malloc((strlen(name) + 1) * sizeof(char));
        memcpy(section_names, name, strlen(name) + 1);
        section_names_size = strlen(name) + 1;
    } else {
        section_names = realloc(section_names,
            (section_names_size + strlen(name) + 1) * sizeof(char));
        memcpy(section_names+section_names_size, name, strlen(name) + 1);
        section_names_size += strlen(name) + 1;
    }

    if(section_header_table == NULL){
        section_header_table = malloc(sizeof(Elf64_Shdr));
        assert(section_header_table != NULL);
        section_header_table_size = sizeof(Elf64_Shdr);
    } else {
        section_header_table = realloc(section_header_table,
            section_header_table_size + sizeof(Elf64_Shdr));
        assert(section_header_table != NULL);
        section_header_table_size += sizeof(Elf64_Shdr);
    }

    int idx = section_header_table_size / sizeof(Elf64_Shdr) - 1;
    section_header_table[idx] = header;
}

