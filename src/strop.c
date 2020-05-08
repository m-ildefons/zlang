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

void strrep(char** str, const char* exp, const char* rep){
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

void strapp(char** str, const char* fmt, ...){
    char buffer[1024];
    va_list args;
    va_start(args, fmt);
    vsnprintf(buffer, sizeof(buffer), fmt, args);
    va_end(args);

    __strapp(str, buffer);
}

void __strapp(char** str, const char* fmt){
    size_t str_len = strlen((*str));
    size_t fmt_len = strlen(fmt);

    (*str) = realloc((*str), (str_len + fmt_len + 1) * sizeof(char));
    assert((*str) != NULL);

    strcat((*str), fmt);
}

void strprp(char** str, const char* prp){
    char* src = strnew();
    char* handle = (*str);
    strapp(&src, prp);
    strapp(&src, (*str));
    (*str) = src;
    free(handle);
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

