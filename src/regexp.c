/*
 * FILENAME regexp.c
 *
 * DESCRIPTION:
 *     Implementation of utilities with regex.
 *
 * AUTHOR: Moritz Röhrich <moritzr@pool.math.tu-berlin.de>
 *
 */

#include "regexp.h"

regex_t int_regex;
regex_t real_regex;
regex_t real_regex_2;
regex_t complex_regex;
const char* int_regex_str = "^([1-9][0-9]*|0){1}(([eE]([+-])?[0-9]+)?)$";
const char* real_regex_str = "^([0-9]*[.][0-9]+){1}(([eE]([+-])?[0-9]+)?)$";
const char* real_regex_str_2 = "^([0-9]*[.][0-9]+){1}(([eE][ ][+-][ ])[0-9]+)$";
const char* complex_regex_str = "^(([1-9][0-9]*|0){1}|([0-9]*[.][0-9]+){1})(([eE]([+-])?[0-9]+)?)i$";

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

    regret = regcomp(&real_regex_2, real_regex_str_2, REG_EXTENDED);
    if(regret != 0){
        fprintf(stderr, "Could not compile regex\n");
        regerror(regret, &real_regex_2, err, 1024);
        printf("%s\n", err);
        exit(1);
    }
    regret = regcomp(&complex_regex, complex_regex_str, REG_EXTENDED);
    if(regret != 0){
        fprintf(stderr, "Could not compile regex\n");
        regerror(regret, &complex_regex, err, 1024);
        printf("%s\n", err);
        exit(1);
    }
}

void regrep(char** str, const char* reg, const char* sub, const char* rep){
    regex_t regex;
    char err[1024];
    int regret = regcomp(&regex, reg, REG_EXTENDED);
    if(regret != 0){
        fprintf(stderr, "Error: Could not compile regex: %s\n", reg);
        regerror(regret, &regex, err, 1024);
        printf("%s\n", err);
        exit(1);
    }

    size_t nmatch = 1;
    regmatch_t matches[nmatch];
    char* match = NULL;
    char* repl = NULL;
    size_t len_match = 0;

    while(regexec(&regex, (*str), nmatch, matches, 0) == 0){
        printf("%s\n", (*str));
            if(matches[0].rm_so == -1){
                break;
            } else {
                len_match = (size_t) (matches[0].rm_eo - matches[0].rm_so);
                repl = salloc(len_match);
                match = salloc(len_match);
                memset(match, 0, len_match);
                memset(repl, 0, len_match);
                strncpy(match,
                        ((*str)+matches[0].rm_so),
                        len_match);

                printf("match: \"%s\"\n", match);
                memcpy(repl, match, len_match);
                strrep(&repl, sub, rep);
                printf("repl: \"%s\"\n", repl);
                strrep(str, match, repl);

                free(repl);
                free(match);
            }
        printf("%s\n", (*str));
    }
    regfree(&regex);
}

