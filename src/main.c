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

static struct option long_options[] = {
	{"verbose", no_argument, &verbosity, 1},
	{"quiet", no_argument, &verbosity, 0},
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

    print_separator("Lexing");

	unsigned long tnt = 0;
	token* tl = NULL;
    lex(filename, &tl, &tnt);

    print_separator("Parsing");

    asn* expr;
    expr = parse_prog(&tl, &tnt, filename);

    print_separator("Abstract Syntax Tree");

    if(expr != NULL)
        pretty_print(expr);

    print_separator("Generating Source");

    char* asm_source;
    asm_gen_prog(expr, &asm_source);

    printf("%s\n", asm_source);

    char* fn;
    if(argv[2])
        fn = argv[2];
    else {
        fn = (char*) malloc(5 * sizeof(char));
        sprintf(fn, "out.s");
    }

    write_file(fn, asm_source);

    return 0;
}

