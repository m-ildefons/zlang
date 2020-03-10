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
    unsigned int i;
    regex_t regex;
    char err[1024];
    int regret = regcomp(&regex, reg, REG_EXTENDED);
    if(regret != 0){
        fprintf(stderr, "Error: Could not compile regex: %s\n", reg);
        regerror(regret, &regex, err, 1024);
        printf("%s\n", err);
        exit(1);
    }

    size_t nmatch = 64;
    regmatch_t matches[nmatch];
    char match[64];
    char* repl = salloc(64);

    if(regexec(&regex, (*str), nmatch, matches, 0) != 0){
        free(repl);
        regfree(&regex);
        return;
    } else {
        printf("%s\n", (*str));
        for(i = 0; i < nmatch; i++){
            if(matches[i].rm_so == -1)
                continue;

            memset(match, 0, 64);
            memset(repl, 0, 64);
            strncpy(match,
                    ((*str)+matches[i].rm_so),
                    (size_t) (matches[i].rm_eo - matches[i].rm_so));

            printf("match: \"%s\"\n", match);
            memcpy(repl, match, 64);
            strrep(&repl, sub, rep);
            printf("repl: \"%s\"\n", repl);
            strrep(str, match, repl);
        }
        printf("%s\n", (*str));
    }
    free(repl);
    regfree(&regex);
}

