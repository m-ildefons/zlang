/*
 * FILENAME: storage_location.h
 *
 * DESCRIPTION:
 *     Delarations for datastructures and functions for storage location
 *     descriptions. These are needed during assembly generation to select
 *     registers an decide whether or not a temporary variable should be saved
 *     on the stack.
 *
 * AUTHOR: Moritz Röhrich <moritzr@pool.math.tu-berlin.de>
 *
 */

#ifndef __STORAGE_LOCATION_H__
#define __STORAGE_LOCATION_H__

#include <stdlib.h>
#include <stdio.h>
#include <assert.h>


/* Storage Location
 *
 * This will be needed later during assembly generation. It will remember where
 * a certain symbol can be found and whether or not it can safely be deleted
 * from a register or has to be saven on the stack first.
 */
typedef struct {
    enum {
        mem_register,
        mem_stack,
    } type;
    size_t pos;
} slocation;

slocation* new_slocation(int type, size_t pos);
void delete_slocation(slocation* l);

#endif

