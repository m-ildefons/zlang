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
    at_float,
    at_char,
    at_void_ptr,
    at_int_ptr,
    at_float_ptr,
    at_char_ptr,
} atomic_type;

static const int atomic_type_size[] = {
    [at_func] = 1,
    [at_void] = 8,
    [at_int] = 8,
    [at_float] = 8,
    [at_char] = 1,
    [at_void_ptr] = 8,
    [at_int_ptr] = 8,
    [at_float_ptr] = 8,
    [at_char_ptr] = 8,
};

static const char* atomic_type_cn[] = {
    [at_func] = "func",
    [at_void] = "void",
    [at_int] = "int",
    [at_float] = "float",
    [at_char] = "char ptr",
    [at_void_ptr] = "void ptr",
    [at_int_ptr] = "int ptr",
    [at_float_ptr] = "float ptr",
    [at_char_ptr] = "char ptr",
};

#endif

