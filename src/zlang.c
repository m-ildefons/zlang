/*
 * FILENAME: zlang.c
 *
 * DESCRIPTION:
 *     Entry point for the ZLang compiler. Also various global declarations and
 *     miscelanious functions.
 *
 * AUTHOR: Moritz Röhrich <moritzr@pool.math.tu-berlin.de>
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <getopt.h>
#include <unistd.h>
#include <assert.h>

#include "zlang.h"
#include "lex.h"
#include "fileop.h"
#include "strop.h"
#include "ast.h"
#include "parse.h"
#include "ic_gen.h"
#include "asm_x86_64.h"
#include "elf_out.h"


char* filename = NULL;
char* asm_out = NULL;
char* elf_out = NULL;

int asm_intel = 0; // off - emit gnu asm by default.

static void print_separator(const char* title);

static void print_separator(const char* title){
    unsigned int i;
    printf("\n");
    printf("====[ %s ]", title);
    for(i = 0; i < (70-strlen(title)); i++)
        printf("=");
    printf("\n\n");
}

int main(int argc, char* argv[]){
#define OPTION(s, l, n, d)                                                    \
  assert(n == 0 || n == 1);
#include "options.inc"
  const char* const arg_is_set = "set";
#define OPTION(s, l, n, d) const char *arg_##s = 0;
#include "options.inc"
  int positional_arguments = 0;
  int i;
  for(i = 0; i < argc; i++){
#define OPTION(s, l, n, d)                                                    \
    {                                                                         \
      if(strcmp("-" #s, argv[i]) == 0){                                       \
        assert(arg_##s == 0);                                                 \
        if(n == 0){                                                           \
          arg_##s = arg_is_set;                                               \
        } else {                                                              \
          assert(i + 1 < argc);                                               \
        }                                                                     \
      } else if(strncmp((n == 0) ? "--" #l : "--" #l "=",                     \
            argv[i], strlen("--" #l "=")) == 0){                              \
        assert(arg_##s == 0);                                                 \
        if(n == 0){                                                           \
          arg_##s = arg_is_set;                                               \
        } else {                                                              \
        }                                                                     \
      }                                                                       \
    }
#include "options.inc"
    argv[positional_arguments++] = argv[i];
  }

  if(arg_v){
#define OPTION(s, l, n, d)                                                    \
    fprintf(stderr, "%s = %s\n", #s, (arg_##s != 0) ? arg_##s : "not set");
#include "options.inc"
    fprintf(stderr, "Positional arguments:\n");
    for(i = 0; i < positional_arguments; i++){
      fprintf(stderr, "%s\n", argv[i]);
    }
  }

  if(arg_h){
    printf("Usage ZLang [ options ] file...\n\n");
    printf("Options:\n\n");
#define OPTION(s, l, n, d)                                                    \
    if(n == 0){                                                               \
      printf("    %s       %s        \t%s\n", "-" #s, "--" #l, #d);           \
    } else {                                                                  \
      printf("    %s <arg> %s=<arg>  \t%s\n", "-" #s, "--" #l, #d);           \
    }
#include "options.inc"
    printf("\nZLang Compiler version %d.%d.%s\n",
      __ZLANG_MAJ__,
      __ZLANG_MIN__,
      __ZLANG_SUB__);
    exit(0);
  }

  if(arg_i){
    asm_intel = 1;
  } else {
    asm_intel = 0;
  }

    if(optind < positional_arguments){
        while(optind < positional_arguments){
            if(filename != NULL)
                free(filename);
      if(asm_out != NULL)
        free(asm_out);
      if(elf_out != NULL)
        free(elf_out);
            filename = strdup(argv[optind]);
            asm_out = basename(filename);
            elf_out = basename(filename);
            strrep(&asm_out, ".x", ".s");
            strrep(&elf_out, ".x", "");
            optind++;
        }
    }

    printf("In/Asm/Elf: %s %s %s\n", filename, asm_out, elf_out);

    init_regex();

    if(access(filename, F_OK) == -1){
        printf("\033[91mError\033[39m: File not found: %s\n", filename);
        exit(1);
    } else if(access(filename, X_OK) != -1){
        printf("\033[91mError\033[39m: Can not compile directory: %s\n",
        filename);
        exit(1);
    }

    print_separator("Lexing");

  size_t tnt = 0;
  token* tl = NULL;
    lex(filename, &tl, &tnt);
    token* tlh = tl; // save token list for free later
    size_t tlhnt = tnt;

    print_separator("Parsing");

    asn* expr;
    expr = parse_translation_unit(&tl, &tnt, filename);

    print_separator("Abstract Syntax Tree");

    if(expr != NULL)
        pretty_print(expr);

    print_separator("Generating Intermediate Code");

    quad_list* IC = ic_gen_translation_unit(expr);
    print_quad_list(IC);

    print_separator("Generating Assembly Code");

    char* asm_code = gen_asm_x86_64(IC);

  print_separator("Final Assembly Code");
    fprintf(stdout, "%s", asm_code);
    write_file(asm_out, asm_code);

//    print_separator("Writing Binary File");

//    write_binary();

    delete_quad_list(IC);
    delete_prog_exp(expr);
    free(asm_code);
    token* tlp = tlh;
    while(tlhnt > 0){
        free(tlp->str);
        tlp++;
        tlhnt--;
    }
    free(tlh);
    return 0;
}

