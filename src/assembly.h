/*
 * FILENAME: assembly.h
 *
 * DESCRIPTION
 *     Declarations of helper functions for assembly generation.
 *
 * AUTHOR: Moritz Röhrich <moritzr@pool.math.tu-berlin.de>
 *
 */

#ifndef __ASSEMBLY_H__
#define __ASSEMBLY_H__

#include "strop.h"
#include "zlang.h"


/* Emit
 *
 * Emits an instruction at the end of the code. Does automatic formatting.
 */
#define emit(code, inst, arg1, arg2) \
  __emit(code, inst, arg1, arg2, __FILE__, __LINE__)
void __emit(char** code,
            const char* inst,
            const char* arg1,
            const char* arg2,
            const char* file,
            unsigned int line);

#endif
