/*
 * FILENAME: atomic_type.h
 *
 * DESCRIPTION:
 *     Declaration of structures for data types.
 *
 * AUTHOR: Moritz Röhrich <moritzr@pool.math.tu-berlin.de>
 *
 */

#ifndef __ATOMIC_TYPE_H__
#define __ATOMIC_TYPE_H__

typedef enum {
    at_func,
    at_void,
    at_int,
    at_real,
    at_char,
    at_void_ptr,
    at_int_ptr,
    at_real_ptr,
    at_char_ptr,
} atomic_type;

extern const int atomic_type_size[];
extern const char* atomic_type_cn[];
extern const atomic_type binary_cast_matrix[9][9];

#endif

