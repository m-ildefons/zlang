/*
 * FILENAME: strop.h
 *
 * DESCRIPTION:
 *     Declarations for simple string operations. These are not included in the
 *     standard C library.
 *
 * AUTHOR: Moritz Röhrich <moritzr@pool.math.tu-berlin.de>
 *
 */

#ifndef __STROP_H__
#define __STROP_H__

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include <assert.h>

/* Implemented in strop.c */

/*
 * strrep - String Replacement
 *
 * Replaces every occurrence of 'exp' with 'rep' in the string pointed to
 * by 'str'. Only exact matches are replaced.
 */
void strrep(char** str, const char* exp, const char* rep);

/*
 * strpad - String Padding
 *
 * Returns a string of length 'len' ending in string 'str' and beginning with
 * repititions of 'pad'. Does not change 'str'.
 */
char* strpad(const char* str, size_t len, const char* pad);

/*
 * strapp - String Append
 *
 * Appends string 'fmt' to string 'str'. Changes 'str'.
 */
void strapp(char** str, const char* fmt, ...);
void __strapp(char** str, const char* fmt);

/*
 * strprp - String Prepend
 *
 * Prepends string 'str' with string 'prp'. Changes 'str'.
 */
void strprp(char** str, const char* prp);

/*
 * strnew - create new empty string
 *
 * Allocates a new string containing nothing but '\0'
 */
char* strnew(void);

/*
 * salloc - create new string of given length
 *
 * Allocates new string of length 'len'.
 */
char* salloc(size_t len);

#endif

