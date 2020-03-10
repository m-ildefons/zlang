/*
 * FILENAME regexp.h
 *
 * DESCRIPTION:
 *     Declaration of utilities with regex.
 *
 * AUTHOR: Moritz Röhrich <moritzr@pool.math.tu-berlin.de>
 *
 */

#ifndef __REGEXP_H__
#define __REGEXP_H__

#include <stdio.h>
#include <stdlib.h>
#include <regex.h>
#include <string.h>

#include "strop.h"

/* implemented in regexp.c */
extern regex_t int_regex;
extern regex_t real_regex;
extern regex_t real_regex_2;
extern regex_t complex_regex;
extern const char* int_regex_str;
extern const char* real_regex_str;
extern const char* real_regex_str_2;
extern const char* complex_regex_str;

void init_regex(void);

/*
 * regrep - String Replacement by Regex
 *
 * Replaces substring 'sub' in every match of 'reg' with 'rep' in the string
 * pointed to by 'str'.
 */
void regrep(char** str, const char* reg, const char* sub, const char* rep);

#endif

