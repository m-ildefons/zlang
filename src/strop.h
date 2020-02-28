#ifndef __STROP_H__
#define __STROP_H__

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

char* strrep(const char* str, const char* exp, const char* rep);
char* strpad(const char* str, size_t len, const char* pad);
void strapp(char** str, const char* app);
void strprp(char** str, const char* prp);

#endif

