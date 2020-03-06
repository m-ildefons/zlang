/*
 * FILENAME: strop.c
 *
 * DESCRIPTION:
 *     Implementation of functions for string operations.
 *
 * AUTHOR: Moritz Röhrich <moritzr@pool.math.tu-berlin.de>
 *
 */

#include "strop.h"

char* strrep(const char* str, const char* exp, const char* rep){
    char *result;
    const char *ins;
    char *tmp;
    size_t len_exp;
    size_t len_rep;
    size_t len_front;
    size_t count;

    /* sanity checks and initialization */
    if(!str || !exp)
        return NULL;
    len_exp = strlen(exp);
    if(len_exp == 0)
        return NULL;
    if(!rep)
        rep = "";
    len_rep = strlen(rep);

    /* count the number of replacements needed */
    ins = str;
    for(count = 0; (tmp = strstr(ins, exp)); ++count) {
        ins = tmp + len_exp;
    }

	size_t result_len = strlen(str) + (len_rep - len_exp) * count + 1;
    tmp = result = malloc(result_len);

    if(!result)
        return NULL;

    while(count--){
        ins = strstr(str, exp);
        len_front = (size_t) (ins - str);
        tmp = strncpy(tmp, str, len_front) + len_front;
        tmp = strcpy(tmp, rep) + len_rep;
        str += len_front + len_exp;
    }
    strcpy(tmp, str);
    return result;
}

void strrepl(char** str, const char* exp, const char* rep){
    char* src = (*str);

    if(src == NULL)
        return;
    if(exp == NULL)
        return;
    if(rep == NULL)
        return;
    if(strstr(src, exp) == NULL)
        return;

    size_t exp_len = strlen(exp);
    size_t src_len = strlen(src);
    char* dest = strnew();
    char* tok;
    char* tmp = salloc(src_len);
    char* res = src;
    unsigned int i;
    for(tok = strstr(res, exp); tok != NULL; tok = strstr(res, exp)){
        strncpy(tmp, res, (size_t) (tok - res));
        strapp(&dest, tmp);
        strapp(&dest, rep);
        for(i = 0; i < src_len; i++)
            tmp[i] = '\0';
        res = tok + exp_len;
    }

    strapp(&dest, res);

    free((*str));
    (*str) = dest;
    free(tmp);
}

char* strpad(const char* str, size_t len, const char* pad){
    int padlen = len - strlen(str);
    char* new = salloc((len+1));
    sprintf(new, "%*.*s%s", padlen, padlen, pad, str);
    return new;
}

void strapp(char** str, const char* app){
    size_t str_len = strlen((*str));
    size_t app_len = strlen(app);

    (*str) = realloc((*str), (str_len + app_len + 1) * sizeof(char));
    assert((*str) != NULL);

    strcat((*str), app);
}

void strprp(char** str, const char* prp){
    size_t str_len = strlen((*str));
    size_t prp_len = strlen(prp);

    char* new_str = malloc((str_len + prp_len + 1) * sizeof(char));
    assert(new_str != NULL);

    strcpy(new_str, prp);
    strcat(new_str, (*str));
    (*str) = new_str;
}

char* strnew(void){
    char* str = salloc(0);
    return str;
}

char* salloc(size_t len){
    char* str = calloc(len + 1, sizeof(char));
    assert(str != NULL);
    return str;
}

