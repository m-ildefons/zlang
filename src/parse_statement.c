/*
 * FILENAME: parse_statement.h
 *
 * DESCRIPTION:
 *     Implementations of parsing functions for statements.
 *
 * AUTHOR: Moritz Röhrich <moritzr@pool.math.tu-berlin.de>
 *
 */

#include "parse.h"

/*
 * <compound-statement> ::= {<declaration>}* {<statement>}*
 */
void parse_compound_statement(token** tl,
                            size_t* tnt,
                            asn_list** body,
                            int top_level){
    printf("[%zu (%s)] parsing compound statement\n", (*tnt), (*tl)->str);

    token* tlp = (*tl);
    asn* node = NULL;

    while((*tnt) > 0 && tlp != NULL && tlp->level > top_level){
        type_link* decl_spec = parse_declaration_specifier(tl, tnt);
        node = parse_declaration(tl, tnt, decl_spec);
        if(node != NULL){
            append_exp_list(body, node);
            tlp = (*tl);
            continue;
        }

        node = parse_statement(tl, tnt);
        if(node != NULL){
            append_exp_list(body, node);
            tlp = (*tl);
            continue;
        }

        if(node == NULL){
            parse_error("Not a Statment or a Declaration.", (*tl));
            abort();
        }
    }
}

/*
 * <statement> ::= <expression-statement>
 *               | <selection-statement>
 *               | <iteration-statement>
 *               | <jump-statement>
 */
asn* parse_statement(token** tl, size_t* tnt){
    printf("[%zu (%s)] parsing statement\n", (*tnt), (*tl)->str);

    if((*tnt) <= 0)
        return NULL;

    asn* statement = NULL;
    token* tlp = (*tl);

    switch(tlp->type){
    case token_cond_if:
        statement = parse_selection_statement(tl, tnt);
        if(statement != NULL){
            printf("\033[92mfound\033[39m selection statement\n");
            return statement;
        }
        break;
    case token_loop_for:
    case token_loop_while:
        statement = parse_iteration_statement(tl, tnt);
        if(statement != NULL){
            printf("\033[92mfound\033[39m iteration statement\n");
            return statement;
        }
        break;
    case break_kw:
    case continue_kw:
    case return_kw:
        statement = parse_jump_statement(tl, tnt);
        if(statement != NULL){
            printf("\033[92mfound\033[39m jump statement\n");
            return statement;
        }
        break;
    default:
        statement = parse_expression_statement(tl, tnt);
        if(statement != NULL){
            printf("\033[92mfound\033[39m expresssion statement\n");
            return statement;
        }
    }
    parse_error("Invalid Statement.", (*tl));
    return NULL;
}

/*
 * <expression-statement> ::= {<expression>}? ;?
 */
asn* parse_expression_statement(token** tl, size_t* tnt){
    printf("[%zu (%s)] parsing expression statement\n", (*tnt), (*tl)->str);
    token* tlp = (*tl);
    asn* exp = parse_expression(tl, tnt);

    tlp = (*tl);
    if(tlp->type == token_semi_colon)
        pop_token(&tlp, tl, tnt);

    return exp;
}

/*
 * <selection-statement> ::= if ( <expression> ) :
 *                               <compound-statement>
 *                         | if ( <expression> ) :
 *                               <compound-statement>
 *                           else :
 *                               <compound-statement>
 */
asn* parse_selection_statement(token** tl, size_t* tnt){
    printf("[%zu (%s)] parsing selection statement\n", (*tnt), (*tl)->str);
    token* tlp = (*tl);
    int scope = tlp->level;

    if(tlp->type != token_cond_if && tlp->type != token_cond_else){
        return NULL;
	}
	pop_token(&tlp, tl, tnt);

    asn* condition = parse_bin_exp(tl, tnt);
    asn* cond_exp = make_cond_exp(condition, NULL, NULL, scope);

    tlp = (*tl);
    if(tlp->type != token_colon){
        return NULL;
	}
	pop_token(&tlp, tl, tnt);

    symbol_list* if_symbols = new_symbol_list((size_t) scope + 1);
    symbol_list_attach(&symbol_list_ptr, &if_symbols);

    parse_compound_statement(tl,
							tnt,
							&(cond_exp->op.cond_exp.if_body),
                            scope);
    cond_exp->op.cond_exp.if_symbols = if_symbols;

	tlp = (*tl);
	if(tlp != NULL && tlp->type == token_cond_else && tlp->level == scope){
		pop_token(&tlp, tl, tnt);

        if(tlp->type == token_colon){
			pop_token(&tlp, tl, tnt);
		} else if(tlp->type != token_cond_if){
			return NULL;
		}

        symbol_list* else_symbols = new_symbol_list((size_t) scope + 1);
        symbol_list_attach(&symbol_list_ptr, &else_symbols);

    	parse_compound_statement(tl,
								tnt,
								&(cond_exp->op.cond_exp.else_body),
								scope);
        cond_exp->op.cond_exp.else_symbols = else_symbols;
	}
    return cond_exp;
}

/*
 * <iteration-statement> ::= while <expression> :
 *                               <compound-statement>
 *                         | for {<declaration> | <expression>}? ;
 *                               {<expression>}? ;
 *                               {<expression>}? :
 *                               <compound-statement>
 */
asn* parse_iteration_statement(token** tl, size_t* tnt){
    printf("[%zu (%s)] parsing iteration statement\n", (*tnt), (*tl)->str);

    if((*tnt) <= 0)
        return NULL;

    asn* statement = NULL;
    statement = parse_for_loop_exp(tl, tnt);
    if(statement != NULL){
        printf("\033[92mfound\033[39m for loop at\n");
        return statement;
    }
    statement = parse_while_loop_exp(tl, tnt);
    if(statement != NULL){
        printf("\033[92mfound\033[39m while loop\n");
        return statement;
    }

    parse_error("Invalid Iteration Statement.", (*tl));
    return NULL;
}

/*
 * <jump-statement> ::= continue ;?
 *                    | break ;?
 *                    | return {<expression>}? ;?
 */
asn* parse_jump_statement(token** tl, size_t* tnt){
    printf("[%zu (%s)] parsing jump statement\n", (*tnt), (*tl)->str);
	token* tlp = (*tl);
	asn* jump = NULL;

	if(tlp->type == break_kw || tlp->type == continue_kw){
		jump = make_jump_exp(tlp->type);
		pop_token(&tlp, tl, tnt);
		if(tlp != NULL && tlp->type == token_semi_colon)
			pop_token(&tlp, tl, tnt);
	} else if(tlp->type == return_kw){
        pop_token(&tlp, tl, tnt);
        asn* inner = NULL;
        if(tlp != NULL && tlp->type != token_semi_colon){
            inner = parse_bin_exp(tl, tnt);
			tlp = (*tl);
        }
        if(tlp != NULL && tlp->type == token_semi_colon){
            pop_token(&tlp, tl, tnt);
        }
        jump = make_ret_exp(inner);
    }

	return jump;
}

