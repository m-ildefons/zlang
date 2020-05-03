/*
 * FILENAME: util.c
 *
 * DESCRIPTION:
 *     Implementation of utilities that don't fit anywhere else.
 *
 * AUTHOR: Moritz Röhrich <moritzr@pool.math.tu-berlin.de>
 *
 */

#include "util.h"

int max(int l, int r){
    return (l > r ? l : r);
}

char* basename(const char* path){
    char* ptr = strrchr(path, '/');
    if(ptr == NULL){
        if((*path) == '/')
            return strdup("/");
        else
            return strdup(path);
    } else {
        return strdup(ptr + 1);
    }
}

