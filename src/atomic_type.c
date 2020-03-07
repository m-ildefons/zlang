/*
 * FILENAME: atomic_type.c
 *
 * DESCRIPTION:
 *     Definition of structures for data types.
 *
 * AUTHOR: Moritz Röhrich <moritzr@pool.math.tu-berlin.de>
 *
 */

#include "atomic_type.h"

const int atomic_type_size[] = {
    [at_func] = 1,
    [at_struct] = 0,
    [at_void] = 8,
    [at_int] = 8,
    [at_real] = 8,
    [at_char] = 1,
    [at_void_ptr] = 8,
    [at_int_ptr] = 8,
    [at_real_ptr] = 8,
    [at_char_ptr] = 8,
};

const char* atomic_type_cn[] = {
    [at_func] = "func",
    [at_struct] = "struct",
    [at_void] = "void",
    [at_int] = "int",
    [at_real] = "real",
    [at_char] = "char ptr",
    [at_void_ptr] = "void ptr",
    [at_int_ptr] = "int ptr",
    [at_real_ptr] = "real ptr",
    [at_char_ptr] = "char ptr",
};

const atomic_type binary_cast_matrix[10][10] = {
    {at_void, at_void, at_void, at_void, at_void, at_void, at_void, at_void, at_void, at_void}, // lhs func
    {at_void, at_void, at_void, at_void, at_void, at_void, at_void, at_void, at_void, at_void}, // lhs struct
    {at_void, at_void, at_void, at_void, at_void, at_void, at_void, at_void, at_void, at_void}, // lhs void
    {at_void, at_void, at_void, at_int, at_real, at_void, at_void, at_void, at_void, at_void}, // lhs int
    {at_void, at_void, at_void, at_real, at_real, at_void, at_void, at_void, at_void, at_void}, // lhs real
    {at_void, at_void, at_void, at_void, at_void, at_void, at_void, at_void, at_void, at_void}, // lhs char
    {at_void, at_void, at_void, at_void, at_void, at_void, at_void, at_void, at_void, at_void}, // lhs void ptr
    {at_void, at_void, at_void, at_void, at_void, at_void, at_void, at_void, at_void, at_void}, // lhs int ptr
    {at_void, at_void, at_void, at_void, at_void, at_void, at_void, at_void, at_void, at_void}, // lhs real ptr
    {at_void, at_void, at_void, at_void, at_void, at_void, at_void, at_void, at_void, at_void}, // lhs char ptr
};

