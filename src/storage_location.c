/*
 * FILENAME: storage_location.c
 *
 * DESCRIPTION:
 *     Implementation for functions for dealing with storage location
 *     descriptions.
 *
 * AUTHOR: Moritz Röhrich <moritzr@pool.math.tu-berlin.de>
 *
 */

#include "storage_location.h"

slocation* new_slocation(int type, size_t pos){
    slocation* s = malloc(sizeof(slocation));
    assert(s != NULL);
    s->type = type;
    s->pos = pos;
    return s;
}

void delete_slocation(slocation* l){
    if(l == NULL)
        return;

    free(l);
}

