/*
 * FILENAME: parse.c
 *
 * DESCRIPTION:
 *     Implementation of general parsing functions. These are general
 *     purpose functions for generating the abstract syntax tree.
 *
 * AUTHOR: Moritz Röhrich <moritzr@pool.math.tu-berlin.de>
 *
 */

#include "parse.h"

/*
 * <translation-unit> ::= {<external-declaration>}*
 */
asn* parse_translation_unit(token** tl, size_t* tnt, const char* filename){
    printf("[%zu (%s)] parsing translation unit\n", (*tnt), (*tl)->str);

    char* bname = basename(filename);
    asn* prog = make_prog_exp(bname);

    symbol_list_ptr = prog->op.prog_exp.symbols;

    while(*tnt > 0){
        asn* e = parse_external_declaration(tl, tnt);
        if(e == NULL)
            break;

        append_exp_list(&(prog->op.prog_exp.prog), e);
    }
    free(bname);
    return prog;
}

asn* parse_ident(token** tl, size_t* tnt){
    token* tlp = *tl;
	asn* var = NULL;

	if(tlp == NULL || tlp->type != ident){
		return NULL;
    }

    printf("[%zu (%s)] parsing ident expr\n", (*tnt), (*tl)->str);
	var = make_ident_exp(tlp->str);

	pop_token(&tlp, tl, tnt);

    printf("found var ref expr\n");
    return var;
}

void pop_token(token** tlp, token** tl, size_t* tnt){
	(*tlp)++;
	(*tnt)--;
	if((*tnt) == 0)
		(*tlp) = NULL;
	(*tl) = (*tlp);
}

void parse_error(const char* err, token* tok){
    fprintf(stderr,
            "Line %d: \033[91mError\033[39m: %s: %s\n",
            tok->line,
            err,
            tok->str);
}

