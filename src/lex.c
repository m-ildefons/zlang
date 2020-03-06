/*
 * FILENAME lex.c
 *
 * DESCRIPTION:
 *     Implementation of functions for lexical analysis.
 *
 * AUTHOR: Moritz Röhrich <moritzr@pool.math.tu-berlin.de>
 *
 */

#include "lex.h"

static regex_t int_regex;
static regex_t real_regex;
static const char* int_regex_str = "^([1-9][0-9]*|0)$";
static const char* real_regex_str = "^([0-9]*[.][0-9]+)$";

static void tokenize_line(char* line,
                int indent_depth,
                token** tokens,
                unsigned long* num,
                int line_number);
static void pre_lex_src(char** src);
static void pre_lex_line(char** src);


token* make_token(int t, char* str, int level, int line){
    token* tok = malloc(sizeof(token));
    tok->type = t;
    tok->str = str;
    tok->level = level;
    tok->line = line;
    return tok;
}

void delete_token(token* t){
    if(t == NULL)
        return;

//    if(t->str != NULL)
//        free(t->str);

    free(t);
    t = NULL;
}

void init_regex(){
    char err[1024];
    int regret = regcomp(&int_regex, int_regex_str, REG_EXTENDED);
    if(regret != 0){
        fprintf(stderr, "Could not compile regex\n");
        regerror(regret, &int_regex, err, 1024);
        printf("%s\n", err);
        exit(1);
    }
    regret = regcomp(&real_regex, real_regex_str, REG_EXTENDED);
    if(regret != 0){
        fprintf(stderr, "Could not compile regex\n");
        regerror(regret, &real_regex, err, 1024);
        printf("%s\n", err);
        exit(1);
    }
}


static void tokenize_line(char* line,
                int indent_depth,
                token** tokens,
                unsigned long* num,
                int line_number){
	enum token_type t;
    const char* sep = " ";
    char* tok;
    line = strdup(line);
    pre_lex_line(&line);

    unsigned long num_tokens;
    static unsigned int idx = 0;
    if(*tokens == NULL)
        *tokens = malloc(sizeof(token));

    for(tok = strtok(line, sep), num_tokens = 1;
        tok != NULL;
        tok = strtok(NULL, sep), num_tokens++){

        if(num_tokens > 1)
            *tokens = realloc(*tokens, num_tokens * sizeof(token));

		if(strcmp(tok, "NULL") == 0){
            t = token_null;
        } else if(strcmp(tok, "void") == 0){
            t = type_void_kw;
        } else if(strcmp(tok, "int") == 0){
            t = type_int_kw;
        } else if(strcmp(tok, "real") == 0){
            t = type_real_kw;
        } else if(strcmp(tok, "char") == 0){
            t = type_char_kw;
        } else if(strcmp(tok, "(") == 0){
            t = open_p;
        } else if(strcmp(tok, ")") == 0){
            t = close_p;
        } else if(strcmp(tok, "[") == 0){
            t = open_square_bracket;
        } else if(strcmp(tok, "]") == 0){
            t = close_square_bracket;
        } else if(strcmp(tok, "return") == 0){
            t = return_kw;
        } else if(strcmp(tok, "continue") == 0){
            t = continue_kw;
        } else if(strcmp(tok, "break") == 0){
            t = break_kw;
        } else if(strcmp(tok, ":") == 0){
            t = token_colon;
        } else if(strcmp(tok, ";") == 0){
            t = token_semi_colon;
        } else if(strcmp(tok, "&") == 0){
            t = token_ampersand;
        } else if(strcmp(tok, "*") == 0){
            t = token_asterisk;
        } else if(regexec(&int_regex, tok, 0, NULL, 0) == 0){
            t = const_int;
        } else if(regexec(&real_regex, tok, 0, NULL, 0) == 0){
            t = const_real;
        } else if(strcmp(tok, "%s") == 0){
            idx++;
            t = const_string;
        } else if(strcmp(tok, ",") == 0){
            t = token_comma;
        } else if(strcmp(tok, "-") == 0){
            t = token_minus;
        } else if(strcmp(tok, "!") == 0 || strcmp(tok, "not") == 0){
            t = not_kw;
        } else if(strcmp(tok, "~") == 0){
            t = complement;
        } else if(strcmp(tok, "+") == 0){
            t = token_plus;
        } else if(strcmp(tok, "/") == 0){
            t = token_div;
        } else if(strcmp(tok, "%") == 0){
            t = token_mod;
        } else if(strcmp(tok, "**") == 0 || strcmp(tok, "exp") == 0){
            t = token_exp;
        } else if(strcmp(tok, "=") == 0){
            t = token_assign;
        } else if(strcmp(tok, "&&") == 0 || strcmp(tok, "and") == 0){
            t = token_log_and;
        } else if(strcmp(tok, "><") == 0 || strcmp(tok, "xor") == 0){
            t = token_log_xor;
        } else if(strcmp(tok, "||") == 0 || strcmp(tok, "or") == 0){
            t = token_log_or;
        } else if(strcmp(tok, "==") == 0 || strcmp(tok, "equal") == 0){
            t = token_equal;
        } else if(strcmp(tok, "!=") == 0 || strcmp(tok, "unequal") == 0){
            t = token_not_equal;
        } else if(strcmp(tok, "<") == 0){
            t = token_less;
        } else if(strcmp(tok, "<=") == 0){
            t = token_less_or_equal;
        } else if(strcmp(tok, ">") == 0){
            t = token_greater;
        } else if(strcmp(tok, ">=") == 0){
            t = token_greater_or_equal;
        } else if(strcmp(tok, "^") == 0){
            t = token_bit_xor;
        } else if(strcmp(tok, "|") == 0){
            t = token_bit_or;
        } else if(strcmp(tok, "++") == 0){
            t = token_inc;
        } else if(strcmp(tok, "--") == 0){
            t = token_dec;
        } else if(strcmp(tok, "<<") == 0){
            t = token_bit_shift_left;
        } else if(strcmp(tok, ">>") == 0){
            t = token_bit_shift_right;
        } else if(strcmp(tok, "+=") == 0){
            t = token_add_assign;
        } else if(strcmp(tok, "-=") == 0){
            t = token_sub_assign;
        } else if(strcmp(tok, "*=") == 0){
            t = token_mul_assign;
        } else if(strcmp(tok, "/=") == 0){
            t = token_div_assign;
        } else if(strcmp(tok, "%=") == 0){
            t = token_mod_assign;
        } else if(strcmp(tok, "&=") == 0){
            t = token_and_assign;
        } else if(strcmp(tok, "^=") == 0){
            t = token_xor_assign;
        } else if(strcmp(tok, "|=") == 0){
            t = token_or_assign;
        } else if(strcmp(tok, "<<=") == 0){
            t = token_bsl_assign;
        } else if(strcmp(tok, ">>=") == 0){
            t = token_bsr_assign;
        } else if(strcmp(tok, "if") == 0){
            t = token_cond_if;
        } else if(strcmp(tok, "else") == 0){
            t = token_cond_else;
        } else if(strcmp(tok, "for") == 0){
            t = token_loop_for;
        } else if(strcmp(tok, "while") == 0){
            t = token_loop_while;
        } else if(strcmp(tok, "struct") == 0){
            t = token_struct;
        } else if(strcmp(tok, "union") == 0){
            t = token_union;
        } else {
            t = ident;
        }

        if(t != const_string){
            (*tokens)[num_tokens - 1].str = strdup(tok);
        } else {
            (*tokens)[num_tokens - 1].str = (char*) malloc((idx / 10 + 2) * sizeof(char));
            sprintf((*tokens)[num_tokens - 1].str, "%u", (idx-1));
        }
        (*tokens)[num_tokens - 1].type = t;
        (*tokens)[num_tokens - 1].level = indent_depth;
        (*tokens)[num_tokens - 1].line = line_number;

        printf("- token[%lu]: %s\ttype: %u\tdepth: %d\n",
            num_tokens,
            (*tokens)[num_tokens - 1].str,
            (*tokens)[num_tokens - 1].type,
            (*tokens)[num_tokens - 1].level);
    }

    *num = num_tokens - 1;
    free(line);
}

static void pre_lex_src(char** src){
    char* buffer = strdup((*src));

    char* tok;
    unsigned int i;
    for(i = 0, tok = strtok(buffer, "\"");
        tok != NULL;
        i++, tok = strtok(NULL, "\"")){
        if(i % 2 != 0){
            if(string_count == 0){
                string_index = (char**) malloc(sizeof(char*));
            } else {
                string_index = (char**) realloc(string_index, (string_count + 1) * sizeof(char*));
            }
            assert(string_index != NULL);

            string_index[string_count] = (char*) malloc((strlen(tok) + 1) * sizeof(char));
            assert(string_index[string_count] != NULL);
            strcpy(string_index[string_count], tok);
            string_count++;
            strrepl(src, tok, "%s");
        }
    }
    if(buffer != NULL)
        free(buffer);
}

static void pre_lex_line(char** src){
    strrepl(src, "(", " ( ");
    strrepl(src, ")", " ) ");
    strrepl(src, "[", " [ ");
    strrepl(src, "]", " ] ");
    strrepl(src, ":", " : ");
    strrepl(src, ",", " , ");
    strrepl(src, ";", " ; ");
    strrepl(src, "-", " - ");
    strrepl(src, "!", " ! ");
    strrepl(src, "+", " + ");
    strrepl(src, "*", " * ");
    strrepl(src, "/", " / ");
    strrepl(src, "%", " % ");
    strrepl(src, "~", " ~ ");
    strrepl(src, "=", " = ");
    strrepl(src, "<", " < ");
    strrepl(src, ">", " > ");
	strrepl(src, "&", " & ");
	strrepl(src, "|", " | ");
	strrepl(src, "^", " ^ ");
    strrepl(src, "<<", " << ");
    strrepl(src, ">>", " >> ");
    strrepl(src, "+  +", " ++ ");
    strrepl(src, "-  -", " -- ");
    strrepl(src, "*  *", " ** ");
	strrepl(src, "<  <", " << ");
	strrepl(src, ">  >", " >> ");
    strrepl(src, "=  =", " == ");
    strrepl(src, "!  =", " != ");
    strrepl(src, "<  =", " <= ");
    strrepl(src, ">  =", " >= ");
    strrepl(src, ">  <", " >< ");
    strrepl(src, "+  =", " += ");
    strrepl(src, "-  =", " -= ");
    strrepl(src, "*  =", " *= ");
    strrepl(src, "/  =", " /= ");
    strrepl(src, "%  =", " %= ");
    strrepl(src, "<<   =", " <<= ");
    strrepl(src, ">>   =", " >>= ");
    strrepl(src, "&  =", " &= ");
    strrepl(src, "|  =", " |= ");
    strrepl(src, "^  =", " ^= ");
    strrepl(src, "&  &", " && ");
    strrepl(src, "|  |", " || ");
    strrepl(src, "\" % s\"", "%s");
}

void lex(const char* filename,
        token** full_token_list,
        unsigned long* total_num_tokens){
    string_count = 0;
    real_count = 0;
    const char* tokens = "\n";
    char* buffer = read_file(filename);
    pre_lex_src(&buffer);
    char* tok;
    char* rest = strdup(buffer);
    char* rest_handle = rest;

    unsigned long num_t = 0;
    unsigned long* num_tp = &num_t;
    token* token_list = NULL;
    (*full_token_list) = malloc(sizeof(token));
    assert((*full_token_list) != NULL);

    int indent_depth = 0;
    int num_ws = 0;
    unsigned long i;
    int line_num = 1;
    int_stack* indent_stack = make_int_stack();
    assert(indent_stack != NULL);
    indent_stack->val = 0;

    for(tok = strtok_r(rest, tokens, &rest);
        tok != NULL;
        tok = strtok_r(NULL, tokens, &rest)){
//    for(tok = strtok(rest, tokens);
//        tok != NULL;
//        tok = strtok(NULL, tokens)){
        line_num++;
        num_ws = 0;
        while(tok[0] == ' ' || tok[0] == '\t'){
            num_ws++;
            tok++;
        }
        if(num_ws > peek(&indent_stack)){
            indent_depth++;
            push(&indent_stack, num_ws);
        } else while (num_ws < peek(&indent_stack)){
            indent_depth--;
            pop(&indent_stack);
        }

        if(num_ws != peek(&indent_stack)){
            fprintf(stderr,
                    "%s: line %d: Indentation Error.\n",
                    filename,
                    line_num);
            exit(101);
        }

        printf("line[%d]: %s\n", indent_depth, tok);
        tokenize_line(tok, indent_depth, &token_list, num_tp, line_num);
        (*total_num_tokens) += num_t;
        (*full_token_list) = realloc((*full_token_list), (*total_num_tokens) * sizeof(token));

        for(i = 0; i < num_t; i++){
            (*full_token_list)[((*total_num_tokens)-1-i)] = token_list[(num_t-1-i)];
        }
        printf("recognized tokens: %lu (%lu total)\n", num_t, (*total_num_tokens));
    }

    delete_int_stack(indent_stack);

    if(buffer != NULL){
        printf("free buffer\n");
        free(buffer);
        buffer = NULL;
    }
    free(rest_handle);
    free(token_list);
}

