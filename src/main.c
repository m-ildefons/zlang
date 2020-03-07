/*
 * FILENAME: main.c
 *
 * DESCRIPTION:
 *     Entry point for the Boa compiler. Also various global declarations and
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

#include "lex.h"
#include "fileop.h"
#include "strop.h"
#include "ast.h"
#include "parse.h"
#include "asm_gen.h"


static int verbosity;
static int verbosity_lex;
static int verbosity_parse;
static int verbosity_ast;
static int verbosity_asmgen;
static int verbosity_asm;

static struct option long_options[] = {
	{"verbose", no_argument, &verbosity, 1},
	{"quiet", no_argument, &verbosity, 0},
    {"Vlex", no_argument, &verbosity_lex, 0},
    {"Vparse", no_argument, &verbosity_parse, 0},
    {"Vast", no_argument, &verbosity_ast, 0},
    {"Vasmgen", no_argument, &verbosity_asmgen, 0},
    {"Vasm", no_argument, &verbosity_asm, 0},
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
	while((c = getopt_long(argc, argv, "f:", long_options, &opt_idx))){
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
		} else {
            abort();
        }
	}

    if(verbosity)
        printf("verbose flag is set\n");

    if(optind < argc){
        printf ("non-option ARGV-elements: ");
        while(optind < argc)
            printf ("%s ", argv[optind++]);
        printf("\n");
    }

    init_regex();

    const char* filename = argv[1];
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

    print_separator("Generating Source");

    char* asm_source = asm_gen_prog(expr);

    print_separator("Assembly Source");

    fprintf(stdout, "%s\n", asm_source);
    write_file("out.s", asm_source);

    delete_prog_exp(expr);
    free(asm_source);
    token* tlp = tlh;
    while(tlhnt > 0){
        free(tlp->str);
        tlp++;
        tlhnt--;
    }
    free(tlh);
    return 0;
}

