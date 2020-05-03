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

#include "zlang.h"
#include "lex.h"
#include "fileop.h"
#include "strop.h"
#include "ast.h"
#include "parse.h"
#include "ic_gen.h"
#include "asm_x86_64.h"
#include "elf_out.h"


static int verbosity;
static int verbosity_lex;
static int verbosity_parse;
static int verbosity_ast;
static int verbosity_asmgen;
static int verbosity_asm;
char* filename = NULL;
char* asm_out = NULL;
char* elf_out = NULL;

static struct option long_options[] = {
	{"verbose", no_argument, &verbosity, 1},
	{"quiet", no_argument, &verbosity, 1},
    {"Vlex", no_argument, &verbosity_lex, 1},
    {"Vparse", no_argument, &verbosity_parse, 1},
    {"Vast", no_argument, &verbosity_ast, 1},
    {"Vic", no_argument, &verbosity_asmgen, 1},
    {"Vasm", no_argument, &verbosity_asm, 1},
	{"output", required_argument, 0, 'o'},
    {NULL, 0, NULL, 0},
};

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
	int c;
	int opt_idx;
	while((c = getopt_long_only(argc, argv, "foqv:", long_options, &opt_idx))){
        if(c == -1){
            break;
        } else if(c == 0){
            /* If this option set a flag, do nothing else now. */
        	if(long_options[opt_idx].flag != 0)
        		break;
        	printf("option %s", long_options[opt_idx].name);
        	if(optarg)
        		printf(" with arg %s", optarg);
        	printf("\n");
        } else if(c == 'f'){
        	  printf("option -f with value `%s'\n", optarg);
        } else if(c == 'o'){
        	  printf("option -o with value `%s'\n", optarg);
		} else {
            fprintf(stderr, "Invalid argument: %c\n", c);
            abort();
        }
	}

    if(verbosity)
        printf("verbose flag is set\n");

    if(optind < argc){
        printf ("non-option ARGV-elements: ");
        while(optind < argc){
            if(filename != NULL)
                free(filename);
            filename = strdup(argv[optind]);
            asm_out = basename(filename);
            elf_out = basename(filename);
            strrep(&asm_out, ".x", ".s");
            strrep(&elf_out, ".x", "");
            printf ("%s", filename);
            optind++;
        }
        printf("\n");
    }

    printf("In/Asm/Elf: %s %s %s\n", filename, asm_out, elf_out);

    init_regex();

    if(access(filename, F_OK) == -1){
        printf("\033[91mError\033[39m: File not found: %s\n", filename);
        exit(1);
    } else if(access(filename, X_OK) != -1){
        printf("\033[91mError\033[39m: Can not compile directory: %s\n", filename);
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
    fprintf(stdout, "%s", asm_code);
    write_file(asm_out, asm_code);

    print_separator("Writing Binary File");

    write_binary();

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

