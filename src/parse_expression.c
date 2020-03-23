/*
 * FILENAME: parse_expression.h
 *
 * DESCRIPTION:
 *     Implementations of parsing functions for expressions.
 *
 * AUTHOR: Moritz Röhrich <moritzr@pool.math.tu-berlin.de>
 *
 */

#include "parse.h"


asn* parse_exp(token** tl, size_t* tnt, pv_root* symbol_map){
    printf("[%zu (%s)] parsing expression\n", (*tnt), (*tl)->str);

    if(*tnt <= 0){
        return NULL;
    }
    asn* expr;
    int level = (*tl)->level;
    expr = parse_declaration(tl, tnt, symbol_map);
    if(expr != NULL){
        printf("\033[92mfound\033[39m declaration at level %d\n", level);
        return expr;
    }
    expr = parse_assign_exp(tl, tnt, symbol_map);
    if(expr != NULL){
        printf("\033[92mfound\033[39m assignment at level %d\n", level);
        return expr;
    }

    parse_error("Invalid Expression", (*tl));
    return NULL;
}

/*
 * <expression> ::= <assignment-expression>
 *                | <expression> , <assignment-expression>
 *
 * Comma separated expressions currently not supported in AST.
 */
asn* parse_expression(token** tl, size_t* tnt, pv_root* symbol_map){
    printf("[%zu (%s)] parsing expression\n", (*tnt), (*tl)->str);

    if((*tnt) <= 0)
        return NULL;

    int level = (*tl)->level;
    asn* expr = parse_assign_exp(tl, tnt, symbol_map);
    if(expr != NULL){
        printf("\033[92mfound\033[39m assignment at level %d\n", level);
        return expr;
    }
    return NULL;
}

/*
 * <cast-expression> ::= <unary-expression>
 *                     | ( <type-name> ) <cast-expression>
 */
asn* parse_cast_exp(token** tl, size_t* tnt, pv_root* symbol_map){
    printf("[%zu (%s)] parsing cast expr\n", (*tnt), (*tl)->str);
    asn* cast = parse_unary_exp(tl, tnt, symbol_map);
    return cast;
}

/*
 * <postfix-expression> ::= <primary-expression>
 *                        | <postfix-expression> [ <expression> ]
 *                        | <postfix-expression> ( {<assignment-expression>}* )
 *                        | <postfix-expression> . <identifier>
 *                        | <postfix-expression> ++
 *                        | <postfix-expression> --
 */
asn* parse_postfix_exp(token** tl, size_t* tnt, pv_root* symbol_map){
    printf("[%zu (%s)] parsing postfix expr\n", (*tnt), (*tl)->str);

    token* tlp = (*tl);

    asn* primary_exp = parse_primary_exp(tl, tnt, symbol_map);
    asn* postfix_exp = primary_exp;
    tlp = (*tl);

	while(primary_exp != NULL && tlp != NULL && tlp->type == open_square_bracket){
		printf("Parsing Array Index\n");
		if(tlp != NULL && tlp->type == open_square_bracket)
			pop_token(&tlp, tl, tnt);

		asn* exp = parse_expression(tl, tnt, symbol_map);

		if(tlp != NULL && tlp->type == close_square_bracket){
			pop_token(&tlp, tl, tnt);
		} else {
			parse_error("Unbalances Square Brackets", (*tl));
			abort();
		}
	}
	while(primary_exp != NULL && tlp != NULL && tlp->type == open_p){
        printf("[%zu (%s)] parsing function call\n", (*tnt), (*tl)->str);
        char* id = primary_exp->op.var_ref_exp.ident;
        asn* arg = NULL;
        asn_list* args = NULL;

        if(tlp->type == open_p)
            pop_token(&tlp, tl, tnt);
        else
            abort();

        while(tlp->type != close_p){
            arg = parse_assign_exp(tl, tnt, symbol_map);
            tlp = (*tl);

            if(tlp->type == token_comma)
                pop_token(&tlp, tl, tnt);

            append_exp_list(&args, arg);
        }

        if(tlp->type == close_p)
            pop_token(&tlp, tl, tnt);
        else
            abort();

        postfix_exp = make_call_exp(id, args);
        return postfix_exp;
	}
	while(primary_exp != NULL && tlp != NULL && tlp->type == token_dot){
        printf("Parsing Struct Member Access\n");
	}
	while(primary_exp != NULL && tlp != NULL && tlp->type == token_inc){
        pop_token(&tlp, tl, tnt);
        postfix_exp = make_unary_exp(at_int, primary_exp, token_inc);
	}
	while(primary_exp != NULL && tlp != NULL && tlp->type == token_dec){
        pop_token(&tlp, tl, tnt);
        postfix_exp = make_unary_exp(at_int, primary_exp, token_dec);
	}

    printf("found postfix expr.\n");
    return postfix_exp;
}

/*
 * <primary-expression> ::= <identifier>
 *                        | <constant>
 *                        | ( <expression> )
 */
asn* parse_primary_exp(token** tl, size_t* tnt, pv_root* symbol_map){
    printf("[%zu (%s)] parsing primary expr\n", (*tnt), (*tl)->str);

    token* tlp = *tl;
    int tok_type = tlp->type;
    asn* primary = NULL;

    switch(tok_type){
        case const_int:
        case const_real:
        case const_char:
        case const_string:
            primary = parse_const_exp(tl, tnt);
            break;
        case ident:
            primary = parse_var_ref(tl, tnt, symbol_map);
            break;
        case open_p:
		    pop_token(&tlp, tl, tnt);
            primary = parse_expression(tl, tnt, symbol_map);
            tlp = (*tl);
            if(tlp->type != close_p){
                parse_error("Unbalanced Parenthesis", (*tl));
                abort();
            }
		    pop_token(&tlp, tl, tnt);
            break;
        case close_p:
            pop_token(&tlp, tl, tnt);
            break;
        default:
            return NULL;
    }
    if(primary != NULL){
        printf("found primary expr\n");
    }

    return primary;
}
